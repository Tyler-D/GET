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
int 
TaskManager<Dtype>::TaskRequestLocal(GET::TaskParam param)
{
    TaskDispatcher<Dtype> task(param, &device_manager_);
    tasks_.insert(make_pair(++task_num_, task));
    //tasks_params_.insert(make_pair(task_num_, param));
    tasks_status_.insert(make_pair(task_num_, TASKWAIT));
    //send the task to threadpool if there is any available thread
    cout << "TaskRequest get" <<endl;
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

template class TaskManager<int>;
template class TaskManager<long>;
template class TaskManager<float>;
template class TaskManager<double>;

