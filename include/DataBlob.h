/*************************************************************************
    > File Name: DataBlob.h
    > Author: crows
    > Mail: 136211494@qq.com 
    > Created Time: Tue 24 Mar 2015 03:03:57 AM EDT
 ************************************************************************/
#ifndef GET_DATABLOB_HEADER_
#define GET_DATABLOB_HEADER_
#include<iostream>
using namespace std;
#include<stdlib.h>

template <typename Dtype>

class DataBlob
{
	public:
		~DataBlob() {};
		DataBlob()
			: num_(0), channels_(0), height_(0), width_(0), capacity_(0), host_data_ptr_(NULL) {}

	   // explicit DataBlob(const int n,const int c, const int h, int w,  BaseDevice* device); 
        explicit DataBlob(const int n, const int c, const int h, int w);
		explicit DataBlob(void * hd,int n,int c,int h,int w);
		void Reshape_HOST(const int n,  const int c, const int h, const int w);
        void ReshapeLike(const int n, const int c, const int h, const int w);
        void CopyFromMemory(void* data);
		//void Reshape_DEVICE(const int n, const int c, const int h, const int w);
        //void CopyFromCPUtoGPU();
        //void CopyFromGPUtoCPU();
        //void CopyFromDataBlob();
		inline int num() {return num_ ;}
		inline int channels() {return channels_ ;}
		inline int height() {return height_ ;}
		inline int width() {return width_ ;}
        inline int count() {return count_;}
        inline int offset(const int n, const int c, const int h, const int w)
    {
        return ((n*channels() + c)*height() + h)*width()+w;   
    }
        /*
        inline Dtype data_at(const int n, const int c, const int h, const int w) const 
        {
            return ((const Dtype*)host_data())[offset(n, c, h, w)];
        }
        */
        inline void* host_data() {return host_data_ptr_;}
		inline void set_host_data(void * host_data){host_data_ptr_ = host_data;}
        //inline void* device_data() {return device_data_ptr_;}
		enum wh_data {HOST, DEVICE, SYNCED} data_state_;
    protected:
		int num_;
		int channels_;
		int height_;
		int width_;
		int count_;
		long capacity_;
        void* host_data_ptr_;
		//void* device_data_ptr_;
	    //BaseDevice* device_;
};
#endif
