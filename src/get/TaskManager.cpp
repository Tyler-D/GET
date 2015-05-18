/*************************************************************************
    > File Name: TaskManager.cpp
    > Author: crows
    > Mail: 136211494@qq.com 
    > Created Time: Tue 24 Mar 2015 02:52:50 AM EDT
 ************************************************************************/
#include"TaskManager.h"
#include<iostream>
using namespace std;


template <typename Dtype>
TaskDispacher<Dtype>*
TaskManager<Dtype>::GetTaskDispatcher(GET::TaskParam param)
{
    GET::TaskParam_TaskType type = param.type();
    switch(type)
    {
        case TaskParam_TaskType_ADD : return new AddTaskDispatcher(param, &device_manager_);
        case TaskParam_TaskType_SUB : return new SubTaskDispatcher(param, &device_manager_);
        case TaskParam_TaskType_MULTI : return new MulTaskDispatcher(param, &device_manager_);
        case TaskParam_TaskType_CONVOLUTION : return new ConvTaskDispatcher(param, &device_manager_);
        case TaskParam_TaskType_POOL : return new PoolTaskDispatcher(param, &device_manager_);
        default:
        {
            cout<<"Unknown Computing Type"<<endl;
            exit(0);
        }
    }
}

template <typename Dtype>
int 
TaskManager<Dtype>::TaskRequestLocal(GET::TaskParam param)
{
    TaskDispatcher<Dtype>* task = GetTaskDispatcher();
    tasks_.insert(make_pair(++task_num_, task));
    //tasks_params_.insert(make_pair(task_num_, param));
    tasks_status_.insert(make_pair(task_num_, TASKWAIT));
    //send the task to threadpool if there is any available thread
    cout << "TaskRequest get" <<endl;
    task.TaskOn(); 
    /*
    if (0 == TaskOn())
    {
        return 0; 
    }  
    else
    {
        cout<<"the task failed"<<endl;
        return 1;
    }
    */
}

template <typename Dtype>
int
TaskManager<Dtype>::TaskRequestLocal(const char* filename)
{
    
}

template <typename Dtype>
void
TaskManager<Dtype>::TaskRequestRemote()
{
    
}

template <typename Dtype>
void
TaskManager<Dtype>::Init()
{
    //Init the platforms and devices on the machine
    device_manager_.Init();
    //Init the main Epoll
}

template <typename Dtype>
int 
TaskManager<Dtype>::TaskOn()
{
}

template class TaskManager<int>;
template class TaskManager<long>;
template class TaskManager<float>;
template class TaskManager<double>;

