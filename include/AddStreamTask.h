/*************************************************************************
	> File Name: AddStreamTask.h
	> Author: 
	> Mail: 
	> Created Time: Mon 20 Apr 2015 04:59:12 AM EDT
 ************************************************************************/

#ifndef GET_ADDSTREAMTASK_H
#define GET_ADDSTREAMTASK_H
#include"StreamTask.h"

template <typename Dtype>
class AddStreamTask: public StreamTask<Dtype>
{
    public:
        ~AddStreamTask();
        explicit AddStreamTask(BaseDevice* device):
        StreamTask<Dtype>(device)
        { 
            this->data_per_block_ =device->GlobalMemory() / 6;
        }

    void PreCompute();
    void Compute();
    void PostCompute();
    void SetParams(int channels, int height, int width)
    {
        channels_ = channels;
        height_ = height;
        width_ = width;
    }
    protected:
    cl_int height_, width_;
    int channels_;

};
#endif
