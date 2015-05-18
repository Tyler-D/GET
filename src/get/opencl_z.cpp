/*************************************************************************
    > File Name: opencl_z.cpp
    > Author: crows
    > Mail: 136211494@qq.com 
    > Created Time: Fri 09 Jan 2015 04:35:38 PM CST
 ************************************************************************/

#include<iostream>
using namespace std;
#include<CL/cl.h>
#include<string>
#include<fcntl.h>
#include<unistd.h>
//#include<opencv2/opencv.hpp>
//using namespace cv;

void 
cl_pStatus(cl_int status, string funcName)
{
	if (status != CL_SUCCESS)
	{
		cout<< funcName <<" error "<< status <<endl;
		exit(0);
	}
	else 
		cout<< funcName <<" CL_SUCCESS"<<endl;
}


char*  
cl_readSource(char* srcPath)
{
	int fd;
	int len;
    char* src = NULL;

	if ((fd = open(srcPath, O_RDONLY)) < 0 )
		cout<< "cl_readSource : open error" <<endl;

	len = lseek(fd, 0, SEEK_END);
	//cout << "length of file : "<< len << endl;
	lseek(fd, 0, SEEK_SET);
  
	src = (char *)malloc(sizeof(char)*len);

	ssize_t nread;
    while (len > 0)
	{
       if ((nread = read(fd, src, len)) < 0)
		   cout<<"cl_readSource : read error"<<endl;
	   len -= nread;
	}

	cout << src <<endl;
    return src; 
}
unsigned int RoundUp(unsigned int value, unsigned int multiple)
{
    unsigned int remainder = value % multiple;
    if (remainder != 0)
    {
        value += (multiple-remainder);
    }
    return value;
}
#if 0
float*
cl_readImagef_gray_cv(char* imgPath, int* img_width, int* img_height, void* Array)
{
	float* srcImg = NULL; 

	Mat img = imread(imgPath,0);
    *img_width = img.cols;
	*img_height = img.rows;
	cout << "img_width: "<<img.cols<< " img_height: "<<img.rows<<endl;

	srcImg = (float*)img.data;
	return srcImg;
}

void
cl_writeImage_simple()
{
	
}
#endif
