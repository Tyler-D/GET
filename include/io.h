/*************************************************************************
    > File Name: io.h
    > Author: crows
    > Mail: 136211494@qq.com 
    > Created Time: Mon 23 Mar 2015 11:58:22 PM EDT
 ************************************************************************/
#ifndef GET_IO_HEADER
#define GET_IO_HEADER
#include<iostream>
using namespace std;
#include<google/protobuf/io/coded_stream.h>
#include<google/protobuf/io/zero_copy_stream_impl.h>
#include<google/protobuf/text_format.h>

using google::protobuf::io::FileInputStream;
using google::protobuf::Message;

int ReadProtoFromTextFile(const char* filename, TaskParam* param);

//template <typename Dtype>
//int ReadDataFromMemory(void* data, DataBlob<Dtype>* blob);

template <typename Dtype>
int ReadDataFromFile(const char* filename, DataBlob<Dtype>* blob);
#endif
