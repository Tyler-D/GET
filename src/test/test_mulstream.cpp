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
    
    int* A = (int *)malloc(elements*sizeof(int));
    int* B = (int *)malloc(elements*sizeof(int));
    int* C = (int *)malloc(elements*sizeof(int));

    cout << "A address : "<<A<<endl;
    cout << "B address : "<<B<<endl;
    cout << "C address : "<<C<<endl;

    for(int i = 0; i < elements ; i++)
    {
        A[i] = 1;
        B[i] = 2;
        C[i] = 0;
    }
 
    DeviceManager dm;
    dm.Init();

    DataBlob<int> data[3];

    for(int i = 0 ; i < 3 ;i++)
        data[i].ReshapeLike(1,1,n,n);

    data[0].CopyFromMemory((void*)A);
    data[1].CopyFromMemory((void*)B);
    data[2].CopyFromMemory((void*)C);
    
    BaseDevice* device = dm.GetAvailableDevice();

    MulStreamTask<int> mulst(device);

    mulst.SetParams(1, n, n, n);
    mulst.AddtoDatas(data[0]);
    mulst.AddtoDatas(data[1]);
    mulst.AddtoResults(data[2]);

    mulst.TaskOn();
    int cnt = 0;

    int answer  = 0;
    for(int i = 0; i< n ;i++)
      answer+= A[i]*B[i];
    for(int i = 0; i < elements ; i++)
    {
        if (C[i] != answer)
        {
            cnt++;
            if (cnt % 50 == 0)
                cout<<C[i]<<endl;
        }
    }

    if (cnt)
    {
        cout<< "Wrong Answer : " <<cnt<<endl;
    }
    else
    {
        cout<<"Right Answer "<<endl;
    }
    free(A);
    free(B);
    free(C);
}
