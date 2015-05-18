/*************************************************************************
	> File Name: test_addstream.cpp
	> Author: 
	> Mail: 
	> Created Time: Fri 24 Apr 2015 11:36:19 AM EDT
 ************************************************************************/

#include<iostream>
using namespace std;
#include"StreamTask.h"
#include"DeviceManager.h"
#include"DataBlob.h"
#include<stdio.h>
#include<stdlib.h>

int
main(int main , char** argv)
{
    int n = atoi(argv[1]);
    int elements = n*n; 
    
    int* IN = (int *)malloc(elements*sizeof(int));
    int* OUT = (int *)malloc(elements*sizeof(int));


    for(int i = 0; i < elements ; i++)
    {
        IN[i] = 1;
        OUT[i] = 0;
    }
 
    DeviceManager dm;
    dm.Init();

    DataBlob<int> data[2];

    for(int i = 0 ; i < 2 ;i++)
        data[i].ReshapeLike(1,1,n,n);

    data[0].CopyFromMemory((void*)IN);
    data[1].CopyFromMemory((void*)OUT);
    
    BaseDevice* device = dm.GetAvailableDevice();

    SigmoidStreamTask<int> relust(device);

    relust.SetParams(1, n, n);
    relust.AddtoDatas(data[0]);
    relust.AddtoResults(data[1]);

    relust.TaskOn();

    cout<<"Right Answer "<<endl;
}
