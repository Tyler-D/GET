/*************************************************************************
	> File Name: TaskDispatcher.cpp
	> Author: 
	> Mail: 
	> Created Time: Thu 09 Apr 2015 10:01:55 PM EDT
 ************************************************************************/

#include<iostream>
using namespace std;
#include"TaskDispatcher.h"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

template <typename Dtype>
TaskDispatcher<Dtype>:: ~TaskDispatcher()
{
    for(int i = 0; i < ordinary_tasks_.size(); i++)
        delete ordinary_tasks_[i];
    for(int i = 0; i < stream_tasks_.size(); i++)
        delete stream_tasks_[i];
    ordinary_tasks_.clear();

    datablobs_input_.clear();
    datablobs_output_.clear();
    devices_needed_.clear(); 
    //datablobs_inner_results_.clear();
    //datablobs_inner_inputs_.clear();
}

template <typename Dtype>
int 
TaskDispatcher<Dtype>::TaskOn()
{
    PreTaskDispatch();
    TaskDispatch();
    ComputeAll();
    PostCompute();
    cout<<"task finished"<<endl; 
    return 0;
}

template <typename Dtype>
void
TaskDispatcher<Dtype>::PrepareBlob(int blobs_num, GET::TaskParam_DataPosition& data_pos, vector<DataBlob<Dtype> >& blobs, BlobType bt)
{
    switch (data_pos)
    {
        //FILE
        case 0:
        {
            break;
        }
        //HOSTMEM
        case 1:
        {
            if (bt == SOURCE)
            { 
                cout<<"source_blobs_num = "<< blobs_num <<endl;
                for(int i = 0; i< blobs_num ; i++)
                {
                    blobs[i].CopyFromMemory((void*)task_param_.sourcem(i));
                }
            }
            else if (bt == RESULT)
            {
                for(int i = 0; i< blobs_num ; i++)
                {
                    blobs[i].CopyFromMemory((void*)task_param_.resultm(i));
                }    
            }
            break;
        }
        default:
           break;
    }
}

/*
template <typename Dtype>
void
TaskDispatcher::GetDevices(long mem_used)
{

}
*/


template <typename Dtype>
void 
TaskDispatcher<Dtype>::ComputeAll()
{
    int thread_error; 
    switch (process_type_)
    {
        case ORDINARY :
        {   
            int tasks_num = ordinary_tasks_.size();
            pthread_t tid[tasks_num];
            for(int i = 0; i < tasks_num; i++)
            {
                cout<<"task "<< i << " begin" <<endl;
                thread_error = pthread_create(&tid[i], NULL, BaseTaskOn<Dtype>,(void*)ordinary_tasks_[i]);

            }
            
            for(int i = 0; i < tasks_num; i++ )
            {
                pthread_join(tid[i], NULL);
            }
            break;
        }
        case STREAM :
        {
            for(int i = 0; i < stream_tasks_.size(); i++)
            {
                cout<<"task "<< i << " begin "<<endl;
                stream_tasks_[i]->TaskOn();
            }
        
            break;
        }
        case HYBRID :
        {
            break;
        }
        default:
        {
            break;
        }
    }
}

template <typename Dtype>
void 
TaskDispatcher<Dtype>::PostCompute()
{
    //free devices
    for(int i = 0; i < devices_needed_.size(); i++)
        device_manager_->FreeDevice(devices_needed_[i]->DeviceIndex());
}

template class TaskDispatcher<int>;
template class TaskDispatcher<long>;
template class TaskDispatcher<float>;
template class TaskDispatcher<double>;
