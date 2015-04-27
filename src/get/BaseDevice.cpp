/*************************************************************************
    > File Name: BaseDevice.cpp
    > Author: crows
    > Mail: 136211494@qq.com 
    > Created Time: Tue 24 Mar 2015 02:53:33 AM EDT
 ************************************************************************/

#include<iostream>
using namespace std;
#include<CL/cl.h>
#include"BaseDevice.h"

void 
BaseDevice::Init()
{
	//device_id_ = device_id;
	//device_ctx_ = device_ctx;
	//get catch line size
	clGetDeviceInfo(device_id_, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, sizeof(cl_uint), &catch_line_size_, NULL);
    //get global mem size
	clGetDeviceInfo(device_id_, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &this->global_memory_, NULL);
    this->global_memory_ *= 0.9;
	//get block size
	clGetDeviceInfo(device_id_, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &max_block_size_, NULL);
	//get max grid size
	cl_uint maxdi = 0;
	clGetDeviceInfo(device_id_, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &maxdi, NULL);
	
	max_grid_size_ = (size_t*)malloc(maxdi*sizeof(size_t));
	clGetDeviceInfo(device_id_, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t)*maxdi, &max_grid_size_, NULL);
	
	
}
