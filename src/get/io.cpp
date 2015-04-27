/*************************************************************************
    > File Name: io.cpp
    > Author: crows
    > Mail: 136211494@qq.com 
    > Created Time: Tue 24 Mar 2015 02:55:58 AM EDT
 ************************************************************************/

#include<iostream>
#include"proto/GET.pb.h"
#include"io.h"
using namespace std;

int ReadProtoFromTextFile(const char* filename, TaskParam* param)
{ 
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
        cout << "File not found: "<< filename <<endl;
    FileInputStream* input = new FileInputStream(fd);
    bool success = google::protobuf::TextFormat::Parse(input, param);
    delete input;
    close(fd)
}
/*
template <typename Dtype>
int ReadDataFromMemory(void* data, DataBlob<Dtype>* blob)
{ 
}
*/

template <typename Dtype>
int ReadDataFromMemory(void* data, DataBlob<Dtype>* blob)
{
    
}
