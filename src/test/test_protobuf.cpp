/*************************************************************************
	> File Name: test_protobuf.cpp
	> Author: 
	> Mail: 
	> Created Time: Thu 16 Apr 2015 10:05:40 PM EDT
 ************************************************************************/

#include<iostream>
using namespace std;
#include"proto/GET.pb.h"
#include<string>
#include<stdlib.h>
#include<stdio.h>
int
main()
{
    GET::TaskParam task_param;

    int* A = (int*)malloc(10*10*sizeof(int));
    cout << "address 1 :" << A <<endl;
    int* B = (int*)malloc(10*10*sizeof(int));
    cout << "address 2 :" << B <<endl;

    char s1[100];
    char s2[100];

    sprintf(s1, "%d", A);
    sprintf(s2, "%d", B);

    string ss1(s1);
    string ss2(s2); 
    task_param.add_source(ss1);
    task_param.add_source(ss2);

    cout<<"address 1: "<< (void *)atoi(s1)<<endl;
    cout<<"address 2: "<< (void *)atoi(s2)<<endl;

    cout<<"address 1:" <<(void *)atoi(task_param.source(0).c_str())<<endl;
    cout<<"address 2:" <<(void *)atoi(task_param.source(1).c_str())<<endl;
    
}
