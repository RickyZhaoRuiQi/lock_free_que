#ifndef Lock_free_que_H_
#define Lock_free_que_H_

#include <cstddef>
#include <iostream>
using namespace std;

template<typename T>
struct Q_Node
{
	Q_Node(T e) :data(e), next(NULL) {}
	T data;
	Q_Node<T>* next;
};

/*无锁队列类*/
template<typename T>
class Lock_free_que
{
public:
	Lock_free_que()		//构造函数（初始化无锁队列），让head和tail都指向头节点
	{
		head = tail = new Q_Node<T>(0);
		head->next = NULL;
	}
	void push(T e)		//普通入队操作
	{
		Q_Node<T>* newNode = new Q_Node<T>(e);
		tail->next = newNode;
		tail = newNode;
	}
	T front()			//得到对头数据
	{
		return head->data;
	}
	bool pop(T* e)		//普通出队
	{
		if (empty())
			return false;
		Q_Node<T>* tmp = head->next;
		head->next = tmp->next;
		*e = tmp->data;
		delete tmp;
		return true;
	}
	//cas入队操作，新建节点后，在入队时需要进行cas操作。__sync_bool_compare_and_swap说明参数只能用ing，long，long long。所以要把指针类型转换程long，long可以屏蔽在32位和64位系统下的差异，它在32位位4字节，64位为8字节
	void cas_push(T e)
	{
		Q_Node<T>* newNode = new Q_Node<T>(e);
		Q_Node<T>* tmp;
		do
		{
			tmp = tail;
		} while (!__sync_bool_compare_and_swap((long*)(&(tmp->next)),(long)NULL, (long)(newNode)));
		tail = newNode;
	}
	bool cas_pop(T* e)	//cas_出队
	{
		Q_Node<T>* tmp;
		do
		{
			if(empty())
			  return false;
			tmp = head->next;
		} while (!__sync_bool_compare_and_swap((long*)(&head->next), (long)(tmp), (long)(tmp->next)));
		*e = tmp->data;
		delete tmp;
		return true;
	}
	bool empty()
	{
		return head->next == NULL;
	}
	void show()
	{
		Q_Node<T>* cur = head->next;
		int count = 0;
		while (cur)
		{
			cout << cur->data << " ";
			cur = cur->next;
			++count;
		}
		cout<<endl<<"共"<<count<<"个数据"<<endl;
	}
	~Lock_free_que()
	{
		while (!empty())
		{
			Q_Node<T>* cur = head->next;
			head->next = cur->next;
			delete cur;
		}
		delete head;
		head = NULL;
		tail = NULL;
	}
private:
	Q_Node<T>* head;
	Q_Node<T>* tail;
};

#endif
