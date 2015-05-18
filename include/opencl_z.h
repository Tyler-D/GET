/*************************************************************************
    > File Name: opencl_z.h
    > Author: crows
    > Mail: 136211494@qq.com 
    > Created Time: Fri 09 Jan 2015 04:30:35 PM CST
 ************************************************************************/
#ifndef OPENCL_Z
#define OPENCL_Z
#include<iostream>
using namespace std;
#include<CL/cl.h>
#include<string>

void 
cl_pStatus(cl_int status, string funcName);

char* 
cl_readSource(char* srcPath);

unsigned int 
RoundUp(unsigned int value, unsigned int multiple);
//float*
//cl_readImagef_gray_cv(char* imgPath, int* img_width, int* img_height, void* Array);
#endif
