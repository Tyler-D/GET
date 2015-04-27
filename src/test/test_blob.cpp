/*************************************************************************
    > File Name: test_blob.cpp
    > Author: crows
    > Mail: 136211494@qq.com 
    > Created Time: Tue 24 Mar 2015 03:55:19 AM EDT
 ************************************************************************/

#include<iostream>
using namespace std;
#include"DataBlob.h"

const int n = 1;
const int c = 1;
const int h = 1024;
const int w = 1024;

int main()
{
	DataBlob<float> t_blob(n, c, h, w);
	cout<< "num : " << t_blob.num()<<endl;
	cout<< "channel : " << t_blob.channels()<<endl;
	cout<< "height : " << t_blob.height()<<endl;
	cout<< "width : " << t_blob.width()<<endl;
	cout<< "host_data_ptr : " << t_blob.host_data()<<endl;
}
