/*************************************************************************
	> File Name: AddStreamTask.cpp
	> Author: 
	> Mail: 
	> Created Time: Mon 20 Apr 2015 05:14:02 AM EDT
 ************************************************************************/

#include<iostream>
using namespace std;
#include"StreamTask.h"
#include"opencl_z.h"
#include<typeinfo>

#define GLOBAL_WORKGROUP_HEIGHT 4096
#define GLOBAL_WORKGROUP_WIDTH 4096
#define LOCAL_WORKGROUP_HEIGHT 16
#define LOCAL_WORKGROUP_WIDTH 16


template <typename Dtype>
ReLUStreamTask<Dtype>::~ReLUStreamTask()
{

}

template <typename Dtype>
void 
ReLUStreamTask<Dtype> :: PreCompute()
{
    //reshape the block for addstream
    long mem_used = 0;
    mem_used += this->datas_[0].count() * sizeof(Dtype);
    
    int temp_num = 0;
    
    temp_num = (mem_used + this->data_per_block_ - 1) / this->data_per_block_;

    //create inner blocks 
    unsigned int height_each = height_ / temp_num;
    unsigned int height_left = 0;
    unsigned int  offset = 0;
    for(int i = 0; i < temp_num - 1 ; i++)
    {
        DataBlob<Dtype> inner;
  
        inner.ReshapeLike(1, 1, height_each, width_);
        inner.CopyFromMemory(this->datas_[0].host_data()+offset);
        this->inner_datas_.push_back(inner);
        
        inner.CopyFromMemory(this->results_[0].host_data()+offset);
        this->inner_results_.push_back(inner);

        offset += height_each*width_*sizeof(Dtype);
        height_left += height_each;
    }
    
    height_left = height_ - height_left;
    DataBlob<Dtype> inner;
    inner.ReshapeLike(1, 1, height_left, width_);
    inner.CopyFromMemory(this->datas_[0].host_data()+offset);
    this->inner_datas_.push_back(inner);
        
    inner.CopyFromMemory(this->results_[0].host_data()+offset);
    this->inner_results_.push_back(inner);
 
    this->blocks_num_ = temp_num;
    //create the buffer
    cl_int status;
    cl_mem temp_buffer; 
  
    cl_int datasize;

    datasize = this->device_->GlobalMemory() / 3;

    temp_buffer = clCreateBuffer(this->device_->DeviceContext(), CL_MEM_READ_ONLY, datasize , NULL, &status);
    cl_pStatus(status, "clCreateBuffer 1");
    this->device_buffers_.push_back(temp_buffer);
    temp_buffer = clCreateBuffer(this->device_->DeviceContext(), CL_MEM_WRITE_ONLY, datasize, NULL, &status);
    cl_pStatus(status, "clCreateBuffer 2");
    this->device_buffers_.push_back(temp_buffer);

 /*
    temp_buffer = clCreateBuffer(this->device_->DeviceContext(), CL_MEM_READ_ONLY, height_each *width_*sizeof(Dtype), NULL, &status);
    cl_pStatus(status, "clCreateBuffer 4");
    this->device_buffers_.push_back(temp_buffer);
    temp_buffer = clCreateBuffer(this->device_->DeviceContext(), CL_MEM_READ_ONLY, height_each *width_*sizeof(Dtype), NULL, &status);
    cl_pStatus(status, "clCreateBuffer 5");
    this->device_buffers_.push_back(temp_buffer);
    temp_buffer = clCreateBuffer(this->device_->DeviceContext(), CL_MEM_WRITE_ONLY, height_each*width_*sizeof(Dtype), NULL, &status);
    cl_pStatus(status, "clCreateBuffer 6");
    this->device_buffers_.push_back(temp_buffer);
*/

}
template <typename Dtype>
void
ReLUStreamTask<Dtype>::Compute()
{
    cl_int status;
    
    char* program_source =NULL;

    if (typeid(Dtype) == typeid(int))
        program_source = cl_readSource("/home/zy/GET/src/get/ReLU_task_int.cl");
    if (typeid(Dtype) == typeid(long))
        program_source = cl_readSource("/home/zy/GET/src/get/ReLU_task_long.cl");
    if (typeid(Dtype) == typeid(float))
        program_source = cl_readSource("/home/zy/GET/src/get/ReLU_task_float.cl");
    if (typeid(Dtype) == typeid(double))
        program_source = cl_readSource("/home/zy/GET/src/get/ReLU_task_double.cl");

    cl_program program = clCreateProgramWithSource(this->device_->DeviceContext(), 1, (const char**)&program_source, NULL, &status);
    cl_pStatus(status, "clCreateProgramWithSource");

    cl_device_id temp_device = this->device_->DeviceID();
    status = clBuildProgram(program, 1, &temp_device, NULL, NULL, NULL);
    cl_pStatus(status, "clBuildProgram");

    cl_uint thread_division_row = 1;
    cl_uint thread_division_col = 1;
    int height_each = this->inner_datas_[0].height();
    
    if (height_each > GLOBAL_WORKGROUP_HEIGHT)
        thread_division_row = (height_each + GLOBAL_WORKGROUP_HEIGHT- 1 ) / GLOBAL_WORKGROUP_HEIGHT;

    if (width_ > GLOBAL_WORKGROUP_WIDTH)
        thread_division_col = (width_ + GLOBAL_WORKGROUP_WIDTH -1 ) / GLOBAL_WORKGROUP_WIDTH;

    size_t global_worksize[2] = {GLOBAL_WORKGROUP_HEIGHT, GLOBAL_WORKGROUP_WIDTH};
    size_t local_worksize[2] = {LOCAL_WORKGROUP_HEIGHT, LOCAL_WORKGROUP_WIDTH};

    int blocks_num = this->blocks_num_;
    cl_kernel kernel[blocks_num];
    
    int i;
    cout <<"blocks_num : "<<blocks_num<<endl;
    for(i = 0; i < blocks_num-1 ; i++)
    {
        kernel[i] = clCreateKernel(program, "ReLUMatrix", &status);
        cl_pStatus(status, "clCreateKernel");
        status = clSetKernelArg(kernel[i], 0, sizeof(cl_mem), &(this->device_buffers_[0]));
        cl_pStatus(status, "clSetKernelArg_0");
        status = clSetKernelArg(kernel[i], 1, sizeof(cl_mem), &(this->device_buffers_[1]));
        cl_pStatus(status, "clSetKernelArg_1");
        status = clSetKernelArg(kernel[i], 2, sizeof(cl_int), &width_);
        cl_pStatus(status, "clSetKernelArg_2");
        status = clSetKernelArg(kernel[i], 3, sizeof(cl_int), &height_each);
        cl_pStatus(status, "clSetKernelArg_3");
        status = clSetKernelArg(kernel[i], 4, sizeof(cl_uint), &thread_division_row);
        cl_pStatus(status, "clSetKernelArg_4");
        status = clSetKernelArg(kernel[i], 5, sizeof(cl_uint), &thread_division_col);
        cl_pStatus(status, "clSetKernelArg_5");
    }

    int height_left = this->inner_datas_[blocks_num - 1].height();
    kernel[i] = clCreateKernel(program, "ReLUMatrix", &status);
    cl_pStatus(status, "clCreateKernel");
    status = clSetKernelArg(kernel[i], 0, sizeof(cl_mem), &(this->device_buffers_[0]));
    cl_pStatus(status, "clSetKernelArg_0");
    status = clSetKernelArg(kernel[i], 1, sizeof(cl_mem), &(this->device_buffers_[1]));
    cl_pStatus(status, "clSetKernelArg_1");
    status = clSetKernelArg(kernel[i], 2, sizeof(cl_int), &width_);
    cl_pStatus(status, "clSetKernelArg_2");
    status = clSetKernelArg(kernel[i], 3, sizeof(cl_int), &height_left);
    cl_pStatus(status, "clSetKernelArg_3");
    status = clSetKernelArg(kernel[i], 4, sizeof(cl_uint), &thread_division_row);
    cl_pStatus(status, "clSetKernelArg_4");
    status = clSetKernelArg(kernel[i], 5, sizeof(cl_uint), &thread_division_col);
    cl_pStatus(status, "clSetKernelArg_5");
 
    

    for(int i = 0; i < blocks_num ; i+=2)
    {
        status = clEnqueueWriteBuffer(this->device_->DeviceCmdQueA(),this->device_buffers_[0], CL_FALSE, 0, this->inner_datas_[i].height()*width_*sizeof(Dtype), (Dtype*)(this->inner_datas_[i].host_data()), 0, NULL, NULL );
        cl_pStatus(status, "clEnqueueWriteBuffer");
        status = clEnqueueNDRangeKernel(this->device_->DeviceCmdQueA(), kernel[i] , 2, NULL , global_worksize, local_worksize, 0, NULL, NULL );
        cl_pStatus(status, "clEnqueueNDRangeKernel");

        if (i+1 == blocks_num)
        {
            
        }
        else
        {
            status = clEnqueueWriteBuffer(this->device_->DeviceCmdQueB(),this->device_buffers_[0], CL_FALSE, 0, this->inner_datas_[i+1].height()*width_*sizeof(Dtype), (Dtype*)(this->inner_datas_[i+1].host_data()), 0, NULL, NULL );
            cl_pStatus(status, "clEnqueueWriteBuffer");
            status = clEnqueueNDRangeKernel(this->device_->DeviceCmdQueB(), kernel[i+1] , 2, NULL , global_worksize, local_worksize, 0, NULL, NULL );
            cl_pStatus(status, "clEnqueueNDRangeKernel");
        }

        status = clEnqueueReadBuffer(this->device_->DeviceCmdQueA(),this->device_buffers_[1], CL_FALSE, 0, this->inner_results_[i].height()*width_*sizeof(Dtype), (Dtype*)(this->inner_results_[i].host_data()), 0, NULL, NULL );
        cl_pStatus(status, "clEnqueueReadBuffer");       
        if (i+1 == blocks_num)
        {
            
        }
        else
        {
            status = clEnqueueReadBuffer(this->device_->DeviceCmdQueB(),this->device_buffers_[1], CL_FALSE, 0, this->inner_results_[i+1].height()*width_*sizeof(Dtype), (Dtype*)(this->inner_results_[i+1].host_data()), 0, NULL, NULL );
        cl_pStatus(status, "clEnqueueReadBuffer");       
        }        
    }


   clFinish(this->device_->DeviceCmdQueA());
   clFinish(this->device_->DeviceCmdQueB());

    //clean
    for(int i = 0; i < blocks_num; i++)
    {
        clReleaseKernel(kernel[i]);
    }
    clReleaseProgram(program);
}
template <typename Dtype>
void 
ReLUStreamTask<Dtype>::PostCompute()
{
}


template class ReLUStreamTask<int>;
template class ReLUStreamTask<long>;
template class ReLUStreamTask<float>;
template class ReLUStreamTask<double>;
