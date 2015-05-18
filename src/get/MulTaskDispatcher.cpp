/*************************************************************************
	> File Name: MulTaskDispatcher.cpp
	> Author: 
	> Mail: 
	> Created Time: Mon 13 Apr 2015 03:43:08 AM EDT
 ************************************************************************/

#include<iostream>
using namespace std;
#include"TaskDispatcher.h"

template <typename Dtype>
void 
MulTaskDispatcher<Dtype>::PreTaskDispatch()
{
    //create input blobs and output blobs
    GET::TaskParam_DataPosition source_pos = this->task_param_.source_pos();
    int input_blob_num = 0;
    if (source_pos == 1)
         input_blob_num = this->task_param_.sourcem_size();
    else if (source_pos == 0)
         input_blob_num = this->task_param_.sourcef_size();

    cout <<"input_blob_num = "<<input_blob_num<<endl;

    GET::TaskParam_DataPosition result_pos = this->task_param_.result_pos();
    int output_blob_num = 0;
    if (result_pos == 1)
         output_blob_num = this->task_param_.resultm_size();
    else if (result_pos == 0)
         output_blob_num = this->task_param_.resultf_size();
    
    DataBlob<Dtype> blob;
    //prepare A
    blob.ReshapeLike(1, 1, mul_param_.m(), mul_param_.k());
    this->datablobs_input_.push_back(blob);
    //prepare B
    blob.ReshapeLike(1, 1, mul_param_.k(), mul_param_.n());
    this->datablobs_input_.push_back(blob);
    //prepare input
    this->PrepareBlob(input_blob_num, source_pos,(this->datablobs_input_), SOURCE);
        //prepare output
    blob.ReshapeLike(1, 1, mul_param_.m(), mul_param_.n());
    this->datablobs_output_.push_back(blob);
    this->PrepareBlob(output_blob_num, result_pos,(this->datablobs_output_), RESULT);
    
}

template <typename Dtype>
void 
MulTaskDispatcher<Dtype>::TaskDispatch()
{
    //get device : mem_used
    long mem_used = 0;
    for(int i = 0; i< this->datablobs_input_.size(); i++)
        mem_used += this->datablobs_input_[i].count() * sizeof(Dtype);
    for(int i = 0; i<this->datablobs_output_.size(); i++)
        mem_used += this->datablobs_output_[i].count() * sizeof(Dtype);
    this->mem_used_ =mem_used;

    bool device_enough = false;
    while (!device_enough)
    {
        BaseDevice* device;
        if ((device = this->device_manager_->GetAvailableDevice()) != NULL)
        {
            this->devices_needed_.push_back(device);
            if(mem_used < (device->GlobalMemory()))
            {
                device_enough = true;
            }
            else
            {
                mem_used -= device->GlobalMemory();
            }
        }
        else
        {
            break;           
        }
    }

    cout << "Need Devices = "<< this->devices_needed_.size() <<endl;
    /*create tasks and attach them to the device
     here is a simple strategy for balancing load:
     attach equal work load to each device.
    */ 
    if (device_enough)
    {
        //int height_each = add_param_.height() / (this->devices_needed_.size()); 
        //compute the weight for load banlancing
        unsigned int height_each = 0;
        unsigned int offset = 0;
        int denum = this->devices_needed_.size();
        unsigned long sum_gl_mem = 0;
        float left_part =0;
        float weight[denum];
        
        for(int i = 0; i <denum ; i++)
        {
            sum_gl_mem += this->devices_needed_[i]->GlobalMemory();
        }

        for(int i =0; i<denum-1; i++)
        {
            weight[i] = ((float)this->devices_needed_[i]->GlobalMemory())/((float)sum_gl_mem);
            left_part += weight[i];
        }
        weight[denum-1] = 1 - left_part;

        unsigned int height_current = 0;

        int i= 0;
        for( i = 0; i < this->devices_needed_.size()-1;i++)
        {
            
            height_each = mul_param_.m() * weight[i];
            height_current += height_each;
            MulTask<Dtype>* temp_task = new MulTask<Dtype>(this->devices_needed_[i]);
            temp_task->SetParams(1, height_each, mul_param_.k(), mul_param_.n());
            //create inner
            DataBlob<Dtype> inner;
            //prepare A inner 
            inner.ReshapeLike(1, 1, height_each, mul_param_.k());
            inner.CopyFromMemory((void*)((this->datablobs_input_[0].host_data())+offset*mul_param_.k()));
            temp_task->AddtoDatas(inner);
            //prepare B inner 
            inner.ReshapeLike(1, 1, mul_param_.k(), mul_param_.n());
            inner.CopyFromMemory((void*)((this->datablobs_input_[1].host_data())));
            temp_task->AddtoDatas(inner);
            //create inner_results
            //prepare C inner 
            inner.ReshapeLike(1, 1, height_each, mul_param_.k());
            inner.CopyFromMemory((void*)((this->datablobs_output_[0].host_data())+offset*mul_param_.n()));
            temp_task->AddtoResults(inner);

            this->ordinary_tasks_.push_back(temp_task);
            cout<<"create " << i << " multask" <<endl;  
            offset += height_each*sizeof(Dtype);
        } // end for 

            height_each = mul_param_.m() - height_current;
            MulTask<Dtype>* temp_task = new MulTask<Dtype>(this->devices_needed_[i]);
            temp_task->SetParams(1, height_each, mul_param_.k(), mul_param_.n());
            //create inner
            DataBlob<Dtype> inner;
            inner.ReshapeLike(1, 1, height_each, mul_param_.k());
            inner.CopyFromMemory((void*)((this->datablobs_input_[0].host_data())+offset*mul_param_.k()));
            temp_task->AddtoDatas(inner);
            inner.ReshapeLike(1, 1, mul_param_.k(), mul_param_.n());
            inner.CopyFromMemory((void*)((this->datablobs_input_[1].host_data())));
            temp_task->AddtoDatas(inner);
            //create inner_results
            inner.ReshapeLike(1, 1, height_each, mul_param_.n());
            inner.CopyFromMemory((void*)((this->datablobs_output_[0].host_data())+offset*mul_param_.n()));
            temp_task->AddtoResults(inner);

            //attach the blob to the task
             
            this->ordinary_tasks_.push_back(temp_task);
        
        this->process_type_ = ORDINARY;
    }
    else
    {
         // stream task
        //decide to compute on one or more devices
        
        //compute on the device with the biggest global memory 
        unsigned long  max_glmem = 0;
        BaseDevice* stream_device = NULL ;
        for(int i = 0; i < this->devices_needed_.size(); i++)
        {
             if(max_glmem < this->devices_needed_[i]->GlobalMemory())
            {
                stream_device = this->devices_needed_[i];
                max_glmem = this->devices_needed_[i]->GlobalMemory(); 
            }
        }
        cout<< "stream device determined"<<endl;
        vector<BaseDevice*>::iterator l_it = this->devices_needed_.begin();
        
        while(l_it != this->devices_needed_.end())
        {
            if((*l_it) != stream_device)
            {
                this->device_manager_->FreeDevice((*l_it)->DeviceIndex());
                l_it = this->devices_needed_.erase(l_it);
            }
            else
            {
                l_it++;
            }
        }
        
        MulStreamTask<Dtype>* temp_adst = new MulStreamTask<Dtype>(stream_device);
        temp_adst->SetParams(1, mul_param_.m(), mul_param_.k(), mul_param_.n());
        temp_adst->AddtoDatas(this->datablobs_input_[0]);
        temp_adst->AddtoDatas(this->datablobs_input_[1]);
        temp_adst->AddtoResults(this->datablobs_output_[0]);
        this->stream_tasks_.push_back(temp_adst);
        
        cout<< "create stream tasks" <<endl;
        this->process_type_ =STREAM;
          
         
    }

}

template class MulTaskDispatcher<int>;
template class MulTaskDispatcher<long>;
template class MulTaskDispatcher<float>;
template class MulTaskDispatcher<double>;

