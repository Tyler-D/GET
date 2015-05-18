/*************************************************************************
	> File Name: ADD_TaskDispatcher.h
	> Author: 
	> Mail: 
	> Created Time: Mon 13 Apr 2015 03:23:50 AM EDT
 ************************************************************************/

#ifndef GET_SUB_TASKDISPATCHER_H
#define GET_SUB_TASKDISPATCHER_H
#include "TaskDispatcher.h"

template <typename Dtype>
class SubTaskDispatcher : public TaskDispatcher<Dtype>
{
    public:
    ~SubTaskDispatcher() {};
    explicit SubTaskDispatcher(GET::TaskParam task_param, DeviceManager* device_manager):TaskDispatcher<Dtype>(task_param, device_manager)
    {
        sub_param_ = task_param.sub_param();
    }

    void PreTaskDispatch();
    void TaskDispatch();

    protected:
    GET::SubParam sub_param_;
};
#endif
