/*************************************************************************
	> File Name: test_taskmanager.cpp
	> Author: 
	> Mail: 
	> Created Time: Mon 13 Apr 2015 10:48:30 PM EDT
 ************************************************************************/

#include<iostream>
using namespace std;
#include"TaskManager.h"
#include"proto/GET.pb.h"

int 
main()
{
    TaskManager<int> tm;
    tm.Init();
    
    GET::TaskParam task_param;

    tm.TaskRequestLocal(task_param);
}
