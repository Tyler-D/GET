/*************************************************************************
	> File Name: ADD_TaskDispatcher.h
	> Author: 
	> Mail: 
	> Created Time: Mon 13 Apr 2015 03:23:50 AM EDT
 ************************************************************************/

#ifndef GET_ADD_TASKDISPATCHER_H
#define GET_ADD_TASKDISPATCHER_H
#include "TaskDispatcher.h"

template <typename Dtype>
class AddTaskDispatcher : public TaskDispatcher<Dtype>
{
    public:
    ~AddTaskDispatcher() {};
    explicit AddTaskDispatcher(GET::TaskParam task_param, DeviceManager* device_manager):TaskDispatcher<Dtype>(task_param, device_manager)
    {
        add_param_ = task_param.add_param();
    }

    void PreTaskDispatch();
    void TaskDispatch();

    protected:
    GET::AddParam add_param_;
};
#endif
