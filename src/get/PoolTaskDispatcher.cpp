/*************************************************************************
	> File Name: PoolTaskDispatcher.cpp
	> Author: 
	> Mail: 
	> Created Time: Mon 13 Apr 2015 03:43:08 AM EDT
 ************************************************************************/

#include<iostream>
using namespace std;
#include"TaskDispatcher.h"

template <typename Dtype>
void 
PoolTaskDispatcher<Dtype>::PreTaskDispatch()
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
    blob.ReshapeLike(1, 1, pool_param_.data_h(), pool_param_.data_w());
    this->datablobs_input_.push_back(blob);
    //prepare B
    //prepare input
    this->PrepareBlob(input_blob_num, source_pos,(this->datablobs_input_), SOURCE);
        //prepare output
    unsigned int output_height = (pool_param_.data_h() - pool_param_.kernel_h()) / pool_param_.stride_h() + 1;
    unsigned int output_width = (pool_param_.data_w() - pool_param_.kernel_w()) / pool_param_.stride_w() + 1;
    blob.ReshapeLike(1, 1, output_height, output_width);
    this->datablobs_output_.push_back(blob);
    this->PrepareBlob(output_blob_num, result_pos,(this->datablobs_output_), RESULT);
    
}

template <typename Dtype>
void 
PoolTaskDispatcher<Dtype>::TaskDispatch()
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
        int heightgoback = 0;
        int width_filter = pool_param_.kernel_w();
        int stride = pool_param_.stride_h();
        int i= 0;
        int height_each = pool_param_.data_h() * weight[0];
        int height_out_each = 0;
        int input_width = pool_param_.data_w();
        int output_width = (input_width - width_filter)/stride + 1;
        Dtype* input_data = (Dtype*)this->datablobs_input_[0].host_data();
        Dtype* output_data = (Dtype*)this->datablobs_output_[0].host_data();
        for( i = 0; i < this->devices_needed_.size()-1;i++)
        {
            
	        heightgoback = height_each - (height_each - width_filter + stride)/stride * stride;
            height_current += (height_each - heightgoback);
            PoolTask<Dtype>* temp_task = new PoolTask<Dtype>(this->devices_needed_[i]);
            temp_task->SetParams(1, height_each, pool_param_.data_w(), pool_param_.kernel_h(), pool_param_.kernel_w(), pool_param_.stride_h(), pool_param_.stride_w(), pool_param_.pad_h(), pool_param_.pad_w());
            //create inner
            //prepare in  
            DataBlob<Dtype> inner_i((void*)input_data, 1, 1 , height_each, input_width);
            temp_task->AddtoDatas(inner_i);
            //prepare filter 
            //create inner_results
            //prepare out 
		    height_out_each = (height_each - width_filter + stride)/stride;
            DataBlob<Dtype> inner_o((void*)output_data,1, 1, height_out_each, output_width);
            temp_task->AddtoResults(inner_o);

            this->ordinary_tasks_.push_back(temp_task);
            cout<<"create " << i << " convtask" <<endl;

            input_data += (height_each - heightgoback)*input_width;
		    output_data += (height_out_each)*output_width;
            
            height_each = pool_param_.data_h() * weight[i+1]+heightgoback;
            

        } // end for 

            height_each = pool_param_.data_h() - height_current;
            PoolTask<Dtype>* temp_task = new PoolTask<Dtype>(this->devices_needed_[i]);
            temp_task->SetParams(1, height_each, pool_param_.data_w(), pool_param_.kernel_h(), pool_param_.kernel_w(), pool_param_.stride_h(), pool_param_.stride_w(), pool_param_.pad_h(), pool_param_.pad_w());
            //create inner
            //prepare in  
            DataBlob<Dtype> inner_i((void*)input_data, 1, 1 , height_each, input_width);
            temp_task->AddtoDatas(inner_i);
            //prepare filter 
            //create inner_results
            //prepare out 
		    height_out_each = (height_each - width_filter + stride)/stride;
            DataBlob<Dtype> inner_o((void*)output_data,1, 1, height_out_each, output_width);
            temp_task->AddtoResults(inner_o);

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
        
        PoolStreamTask<Dtype>* temp_adst = new PoolStreamTask<Dtype>(stream_device);
        temp_adst->SetParams(1, pool_param_.data_h(), pool_param_.data_w(), pool_param_.kernel_h(), pool_param_.kernel_w(), pool_param_.stride_h(), pool_param_.stride_w(), pool_param_.pad_h(), pool_param_.pad_w());
        temp_adst->AddtoDatas(this->datablobs_input_[0]);
        temp_adst->AddtoResults(this->datablobs_output_[0]);
        this->stream_tasks_.push_back(temp_adst);
        
        cout<< "create stream tasks" <<endl;
        this->process_type_ =STREAM;
          
         
    }

}

template class PoolTaskDispatcher<int>;
template class PoolTaskDispatcher<long>;
template class PoolTaskDispatcher<float>;
template class PoolTaskDispatcher<double>;

