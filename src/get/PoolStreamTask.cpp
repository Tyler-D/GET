/*************************************************************************
	> File Name: PoolStreamTask.cpp
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
PoolStreamTask<Dtype>::~PoolStreamTask()
{

}

template <typename Dtype>
void 
PoolStreamTask<Dtype> :: PreCompute()
{
    //reshape the block for addstream
    
	int stride = stride_h_;
	int width_filter = kernel_w_;	
	int input_height = data_h_;
	int input_width = data_w_;
	int output_height = output_h_;
	int output_width = output_w_;
	long total_device_size = this->device_->GlobalMemory() * 0.6;
	DataBlob<Dtype> *input_matrix,*filter,*output_matrix;
	Dtype *host_data_inputmatrix,*host_data_outputmatrix;
	host_data_inputmatrix = (Dtype *)this->datas_[0].host_data();
	host_data_outputmatrix = (Dtype *)this->results_[0].host_data();
	int height_per_inputmatrix = (total_device_size/sizeof(Dtype) -  width_filter * width_filter)/(input_width + output_width);
	int height_per_outputmatrix;
	int heightgoback = height_per_inputmatrix - (height_per_inputmatrix - width_filter + stride)/stride * stride;
    int blocks_num = 0;
	while(input_height > width_filter)
	{
		//filter = data_input_[1];
		height_per_inputmatrix = height_per_inputmatrix > input_height?input_height:height_per_inputmatrix;
		height_per_outputmatrix = (height_per_inputmatrix - width_filter + stride)/stride;
		DataBlob<Dtype> input_matrix((void *)host_data_inputmatrix,1,1,height_per_inputmatrix,input_width);
	    DataBlob<Dtype> output_matrix((void *)host_data_outputmatrix,1,1,height_per_outputmatrix,output_width);	
		host_data_inputmatrix += (height_per_inputmatrix - heightgoback)*input_width;
		host_data_outputmatrix += height_per_outputmatrix* output_width;
		input_height  = input_height - height_per_inputmatrix + heightgoback;
		this->inner_datas_.push_back(input_matrix);
		this->inner_results_.push_back(output_matrix);
        blocks_num++;
	}
    this->blocks_num_ = blocks_num;
    cl_int datasize_i = this->inner_datas_[0].height()*input_width*sizeof(Dtype);
    cl_int datasize_o = this->inner_results_[0].height()*this->inner_results_[0].width()*sizeof(Dtype);


    //datasize = this->device_->GlobalMemory() / 3;
    
    cl_mem temp_buffer;
    cl_int status;

    temp_buffer = clCreateBuffer(this->device_->DeviceContext(), CL_MEM_READ_ONLY, datasize_i , NULL, &status);
    cl_pStatus(status, "clCreateBuffer 1");
    this->device_buffers_.push_back(temp_buffer);
    temp_buffer = clCreateBuffer(this->device_->DeviceContext(), CL_MEM_WRITE_ONLY, datasize_o, NULL, &status);
    cl_pStatus(status, "clCreateBuffer 3");
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
PoolStreamTask<Dtype>::Compute()
{
    cl_int status;
    
    char* program_source = NULL;

    if (typeid(Dtype) == typeid(int))
        program_source = cl_readSource("/home/zy/GET/src/get/pool_task_int.cl");
    if (typeid(Dtype) == typeid(long))
        program_source = cl_readSource("/home/zy/GET/src/get/pool_task_long.cl");
    if (typeid(Dtype) == typeid(float))
        program_source = cl_readSource("/home/zy/GET/src/get/pool_task_float.cl");
    if (typeid(Dtype) == typeid(double))
        program_source = cl_readSource("/home/zy/GET/src/get/pool_task_double.cl");

    cl_program program = clCreateProgramWithSource(this->device_->DeviceContext(), 1, (const char**)&program_source, NULL, &status);
    cl_pStatus(status, "clCreateProgramWithSource");

    cl_device_id temp_device = this->device_->DeviceID();
    status = clBuildProgram(program, 1, &temp_device, NULL, NULL, NULL);
    cl_pStatus(status, "clBuildProgram");

    
    int padding_pixels = (int)(kernel_w_ / 2) * 2;

    unsigned int global_itemh = RoundUp(this->inner_datas_[0].height() - padding_pixels , LOCAL_WORKGROUP_HEIGHT);
    unsigned int global_itemw = RoundUp(this->inner_datas_[0].width() - padding_pixels, LOCAL_WORKGROUP_WIDTH);
    size_t global_worksize[2] = {global_itemh , global_itemw};
    size_t local_worksize[2] = {LOCAL_WORKGROUP_HEIGHT, LOCAL_WORKGROUP_WIDTH};

    cl_uint local_height = LOCAL_WORKGROUP_HEIGHT + padding_pixels;
    cl_uint local_width = LOCAL_WORKGROUP_WIDTH + padding_pixels;

    size_t local_mem = local_height * local_width * sizeof(Dtype);

    int blocks_num = this->blocks_num_;
    cl_kernel kernel[blocks_num];
    
    int i;
    cout <<"blocks_num : "<<blocks_num<<endl;
    int in_height_each = this->inner_datas_[0].height();
    int in_width_each = this->inner_datas_[0].width();
    int out_height_each = this->inner_results_[0].height();
    int out_width_each = this->inner_results_[0].width();
    for(i = 0; i < blocks_num-1 ; i++)
    {
         kernel[i] = clCreateKernel(program, "poolMatrix", &status);
        cl_pStatus(status, "clCreateKernel");
        status = clSetKernelArg(kernel[i], 0, sizeof(cl_mem), &(this->device_buffers_[0]));
        cl_pStatus(status, "clSetKernelArg_0");
        status = clSetKernelArg(kernel[i], 1, sizeof(cl_mem), &(this->device_buffers_[1]));
        cl_pStatus(status, "clSetKernelArg_1");
        status = clSetKernelArg(kernel[i], 2, sizeof(cl_int), &out_height_each);
        cl_pStatus(status, "clSetKernelArg_2");
        status = clSetKernelArg(kernel[i], 3, sizeof(cl_int), &out_width_each);
        cl_pStatus(status, "clSetKernelArg_3");
        status = clSetKernelArg(kernel[i], 4, sizeof(cl_int), &in_height_each);
        cl_pStatus(status, "clSetKernelArg_4");
        status = clSetKernelArg(kernel[i], 5, sizeof(cl_int), &in_width_each);
        cl_pStatus(status, "clSetKernelArg_5");
        status = clSetKernelArg(kernel[i], 6, sizeof(cl_uint), &kernel_w_);
        cl_pStatus(status, "clSetKernelArg_6");
        status = clSetKernelArg(kernel[i], 7, local_mem, NULL);
        cl_pStatus(status, "clSetKernelArg_7");
        status = clSetKernelArg(kernel[i], 8, sizeof(cl_uint), &local_height);
        cl_pStatus(status, "clSetKernelArg_8");
        status = clSetKernelArg(kernel[i], 9, sizeof(cl_uint), &local_width);
        cl_pStatus(status, "clSetKernelArg_9");
      
    }

    int in_height_left = this->inner_datas_[i].height();
    int in_width = this->inner_datas_[i].width();
    int out_height = this->inner_results_[i].height();
    int out_width = this->inner_results_[i].width();

    kernel[i] = clCreateKernel(program, "poolMatrix", &status);
    cl_pStatus(status, "clCreateKernel");
    status = clSetKernelArg(kernel[i], 0, sizeof(cl_mem), &(this->device_buffers_[0]));
    cl_pStatus(status, "clSetKernelArg_0");
    status = clSetKernelArg(kernel[i], 1, sizeof(cl_mem), &(this->device_buffers_[1]));
    cl_pStatus(status, "clSetKernelArg_1");
    status = clSetKernelArg(kernel[i], 2, sizeof(cl_int), &out_height);
    cl_pStatus(status, "clSetKernelArg_2");
    status = clSetKernelArg(kernel[i], 3, sizeof(cl_int), &out_width);
    cl_pStatus(status, "clSetKernelArg_3");
    status = clSetKernelArg(kernel[i], 4, sizeof(cl_int), &in_height_left);
    cl_pStatus(status, "clSetKernelArg_4");
    status = clSetKernelArg(kernel[i], 5, sizeof(cl_int), &in_width);
    cl_pStatus(status, "clSetKernelArg_5");
    status = clSetKernelArg(kernel[i], 6, sizeof(cl_uint), &kernel_w_);
    cl_pStatus(status, "clSetKernelArg_6");
    status = clSetKernelArg(kernel[i], 7, local_mem, NULL);
    cl_pStatus(status, "clSetKernelArg_7");
    status = clSetKernelArg(kernel[i], 8, sizeof(cl_uint), &local_height);
    cl_pStatus(status, "clSetKernelArg_8");
    status = clSetKernelArg(kernel[i], 9, sizeof(cl_uint), &local_width);
    cl_pStatus(status, "clSetKernelArg_9");
     

    for(int i = 0; i < blocks_num ; i+=2)
    {
        //cout<<"address : "<<this->inner_datas_[2*i].host_data()<<endl;
        status = clEnqueueWriteBuffer(this->device_->DeviceCmdQueA(),this->device_buffers_[0], CL_FALSE, 0, this->inner_datas_[i].height()*data_w_*sizeof(Dtype), (Dtype*)(this->inner_datas_[i].host_data()), 0, NULL, NULL );
        cl_pStatus(status, "clEnqueueWriteBuffer");
        status = clEnqueueNDRangeKernel(this->device_->DeviceCmdQueA(), kernel[i] , 2, NULL , global_worksize, local_worksize, 0, NULL, NULL );
        cl_pStatus(status, "clEnqueueNDRangeKernel");

        if (i+1 == blocks_num)
        {
            
        }
        else
        {
            status = clEnqueueWriteBuffer(this->device_->DeviceCmdQueB(),this->device_buffers_[0], CL_FALSE, 0, this->inner_datas_[(i+1)].height()*data_w_*sizeof(Dtype), (Dtype*)(this->inner_datas_[(i+1)].host_data()), 0, NULL, NULL );
            cl_pStatus(status, "clEnqueueWriteBuffer");
            status = clEnqueueNDRangeKernel(this->device_->DeviceCmdQueB(), kernel[i+1] , 2, NULL , global_worksize, local_worksize, 0, NULL, NULL );
            cl_pStatus(status, "clEnqueueNDRangeKernel");
        }

        status = clEnqueueReadBuffer(this->device_->DeviceCmdQueA(),this->device_buffers_[1], CL_FALSE, 0, this->inner_results_[i].height()*output_w_*sizeof(Dtype), (Dtype*)(this->inner_results_[i].host_data()), 0, NULL, NULL );
        cl_pStatus(status, "clEnqueueReadBuffer");       
        if (i+1 == blocks_num)
        {
            
        }
        else
        {
            status = clEnqueueReadBuffer(this->device_->DeviceCmdQueB(),this->device_buffers_[1], CL_FALSE, 0, this->inner_results_[i+1].height()*output_w_*sizeof(Dtype), (Dtype*)(this->inner_results_[i+1].host_data()), 0, NULL, NULL );
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
PoolStreamTask<Dtype>::PostCompute()
{
}


template class PoolStreamTask<int>;
template class PoolStreamTask<long>;
template class PoolStreamTask<float>;
template class PoolStreamTask<double>;
