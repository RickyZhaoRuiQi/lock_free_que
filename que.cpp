#include <iostream>
#include <unistd.h>
#include <cstddef>
#include <thread>
#include <vector>
#include "lock_free_que.h"
using namespace std;

#define CORE_NUM 4		//线程数，我电脑核心是4核心

Lock_free_que<int> que;	//声明无锁队列资源
void thread_push()		//线程函数，模拟对资源的抢占
{
	cout<<"start push"<<endl;
	for(int i=0;i<1000;++i)
	  que.cas_push(i+1);//que.push(i+1);普通入队会，因为资源竞争会导致数据的丢失
	cout<<"finishi push"<<endl;
	pthread_exit(0);
}
void thread_pop()
{
	cout<<"start pop"<<endl;
	bool res;int tmp = 0;;
	int count = 0;
	while(1)
	{
		res = que.cas_pop(&tmp);//res = que.pop(&tmp);
		if(!res)
		  break;
		++count;
	}
	cout<<"pop "<<count<<" data"<<endl;
	pthread_exit(0);
}

int main()
{
	vector<thread> t;
	for(int i = 0;i < CORE_NUM;++i)
	{
		t.push_back(thread(thread_push));
	}
	for(int i = 0;i < CORE_NUM;++i)
	{
		t[i].join();
	}
	t.clear();
	/*for(int i = 0;i < CORE_NUM;++i)
	{
		t.push_back(thread(thread_pop));
	}
	for(int i = 0;i < CORE_NUM;++i)
	{
		t[i].join();
	}
	t.clear();*/



	/*int tmp = 0,count = 0;
	while(!que.empty())
	{
		que.cas_pop(&tmp);
		++count;
	}
	cout<<"pop:"<<count<<" number"<<endl;*/
	que.show();
	cout<<endl;
	exit(0);
}
