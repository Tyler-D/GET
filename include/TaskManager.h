/*************************************************************************
    > File Name: TaskManager.h
    > Author: crows
    > Mail: 136211494@qq.com 
    > Created Time: Mon 23 Mar 2015 11:58:32 PM EDT
 ************************************************************************/
#ifndef GET_TASK_MANAGER_HEADER
#define GET_TASK_MANAGER_HEADER
#include<iostream>
#include<map>
#include<vector>
#include"TaskDispatcher.h"
#include"DeviceManager.h"
using namespace std;

template <typename Dtype>
class TaskManager
{
    public:
    ~TaskManager() {};
    TaskManager():
    tasks_(),tasks_status_(),device_manager_(),task_num_(0) {};

    int TaskRequestLocal(GET::TaskParam param);
    int TaskRequestLocal(const char* filename);
    int TaskOn();
    void TaskRequestRemote();
    void Init();
    int GetTaskDispatcher(GET::TaskParam param);

    typedef enum {TASKWAIT,TASKON,TASKFINISHED} TaskStatus;

    protected:
    map<int, TaskDispatcher<Dtype>* > tasks_;
    //map<int, GET::TaskParam> tasks_params_;
    map<int, TaskStatus> tasks_status_;
    DeviceManager device_manager_;
    int task_num_;
};
#endif
