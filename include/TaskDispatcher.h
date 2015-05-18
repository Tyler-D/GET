/*************************************************************************
	> File Name: TaskDispatcher.h
	> Author: 
	> Mail: 
	> Created Time: Thu 09 Apr 2015 10:03:02 PM EDT
 ************************************************************************/

#ifndef GET_TASK_DISPATCHER_HEADER
#define GET_TASK_DISPATCHER_HEADER
#include"proto/GET.pb.h"
#include"DataBlob.h"
#include"BaseTask.h"
#include"DeviceManager.h"
#include"BaseDevice.h"
#include"StreamTask.h"
#include<vector>

enum TaskProcessType {UNINITIAL, ORDINARY, STREAM, HYBRID};
enum BlobType {SOURCE, RESULT};
template <typename Dtype>
class TaskDispatcher
{
    public:
    virtual ~TaskDispatcher() ;
    explicit TaskDispatcher(GET::TaskParam task_param, DeviceManager* device_manager)
    {
        device_manager_ =device_manager;
        task_param_ = task_param;
        process_type_ = UNINITIAL;
    }

    int TaskOn();
    //inline void GetDevices(long mem_used);
    //inline void FreeDevices();
    virtual void PreTaskDispatch() {};
    void PrepareBlob(int blobs_num, GET::TaskParam_DataPosition& data_pos, vector<DataBlob<Dtype> >& blobs, BlobType bt) ;
    virtual void TaskDispatch() {};
    void ComputeAll();
    virtual void PostCompute() ;
   // void GatherAll();

    protected:
    GET::TaskParam task_param_;
    vector<StreamTask<Dtype>* > stream_tasks_;
    vector<BaseTask<Dtype>* > ordinary_tasks_;
    vector<DataBlob<Dtype> > datablobs_input_;
    vector<DataBlob<Dtype> > datablobs_output_;
    DeviceManager* device_manager_;
    vector<BaseDevice*> devices_needed_;
    //vector<DataBlob<Dtype> > datablobs_inner_results_;
    //vector<DataBlob<Dtype> > datablobs_inner_inputs_;
    unsigned long mem_used_;
    TaskProcessType process_type_;

};

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

template <typename Dtype>
class MulTaskDispatcher : public TaskDispatcher<Dtype>
{
    public:
    ~MulTaskDispatcher() {};
    explicit MulTaskDispatcher(GET::TaskParam task_param, DeviceManager* device_manager):TaskDispatcher<Dtype>(task_param, device_manager)
    {
        mul_param_ = task_param.mul_param();
    }

    void PreTaskDispatch();
    void TaskDispatch();

    protected:
    GET::MulParam mul_param_;
};

template <typename Dtype>
class ConvTaskDispatcher : public TaskDispatcher<Dtype>
{
    public:
    ~ConvTaskDispatcher() {};
    explicit ConvTaskDispatcher(GET::TaskParam task_param, DeviceManager* device_manager):TaskDispatcher<Dtype>(task_param, device_manager)
    {
        conv_param_ = task_param.conv_param();
    }

    void PreTaskDispatch();
    void TaskDispatch();

    protected:
    GET::ConvParam conv_param_;
};

template <typename Dtype>
class PoolTaskDispatcher : public TaskDispatcher<Dtype>
{
    public:
    ~PoolTaskDispatcher() {};
    explicit PoolTaskDispatcher(GET::TaskParam task_param, DeviceManager* device_manager):TaskDispatcher<Dtype>(task_param, device_manager)
    {
        pool_param_ = task_param.pool_param();
    }

    void PreTaskDispatch();
    void TaskDispatch();

    protected:
    GET::PoolParam pool_param_;
};

template <typename Dtype>
class ReLUTaskDispatcher : public TaskDispatcher<Dtype>
{
    public:
    ~ReLUTaskDispatcher() {};
    explicit ReLUTaskDispatcher(GET::TaskParam task_param, DeviceManager* device_manager):TaskDispatcher<Dtype>(task_param, device_manager)
    {
        relu_param_ = task_param.relu_param();
    }

    void PreTaskDispatch();
    void TaskDispatch();

    protected:
    GET::ReLUParam relu_param_;
};

template <typename Dtype>
class SigmoidTaskDispatcher : public TaskDispatcher<Dtype>
{
    public:
    ~SigmoidTaskDispatcher() {};
    explicit SigmoidTaskDispatcher(GET::TaskParam task_param, DeviceManager* device_manager):TaskDispatcher<Dtype>(task_param, device_manager)
    {
        sig_param_ = task_param.sigmoid_param();
    }

    void PreTaskDispatch();
    void TaskDispatch();

    protected:
    GET::SigmoidParam sig_param_;
};
#endif
