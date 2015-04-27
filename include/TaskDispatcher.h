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
#include"CommonTask.h"
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
#endif
