/*************************************************************************
    > File Name: BaseDevice.h
    > Author: crows
    > Mail: 136211494@qq.com 
    > Created Time: Tue 24 Mar 2015 02:51:11 AM EDT
 ************************************************************************/
#ifndef GET_BASE_DEVICE_HEADER
#define GET_BASE_DEVICE_HEADER
#include<iostream>
using namespace std;
#include<vector>
#include<CL/cl.h>

class BaseDevice
{
	public:
    ~BaseDevice() {};
	BaseDevice()
		:device_id_(), device_type_(), device_ctx_(), device_cmdqueues_(), global_memory_(0), catch_line_size_(0), wavefront_size_(64), max_block_size_(0), max_grid_size_(0) {};

	explicit BaseDevice(cl_device_id device_id, int device_index, cl_context device_ctx, cl_command_queue device_que1, cl_command_queue device_que2)
	{
		device_id_ = device_id;
		device_index_ = device_index;
		device_ctx_ =device_ctx;
		device_cmdqueues_.push_back(device_que1);
		if (device_que2 != NULL)
			device_cmdqueues_.push_back(device_que2);
	}
    void Init();
	inline unsigned long GlobalMemory() {return global_memory_;}
	inline int WavefrontSize() {return wavefront_size_;}
    inline int MaxBlockSize() {return max_block_size_;}
	inline size_t* MaxGridSize() {return max_grid_size_;}
	inline cl_context DeviceContext() {return device_ctx_;}
	inline cl_device_id DeviceID() {return device_id_;}
    inline int DeviceIndex() {return device_index_;}
    inline cl_command_queue DeviceCmdQueA() {return device_cmdqueues_[0];}
    inline cl_command_queue DeviceCmdQueB() {return device_cmdqueues_[1];}

	typedef enum {CPU, GPU} DeviceType;

	protected:
	cl_device_id device_id_;
	int device_index_;
	DeviceType device_type_;
	cl_context device_ctx_;
	vector<cl_command_queue> device_cmdqueues_;
	cl_ulong global_memory_;
	cl_uint catch_line_size_;
	int wavefront_size_;
	int max_block_size_;
	size_t* max_grid_size_;

};

#endif
