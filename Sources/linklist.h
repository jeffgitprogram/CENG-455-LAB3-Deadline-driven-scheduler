#ifndef __linklist_H
#define __linklist_H

#include "stdio.h"
#include <stdlib.h>
#include "string.h"
#include <malloc.h> 
#include <timer.h>

/*Scheduler Linked list strut*/
typedef struct ListNode {
	_task_id tid;
	uint32_t deadline;
	uint32_t task_type;
	uint32_t creation_time;
	struct ListNode *next;
	} ListNode;


//Allocate memory space for list node
void MallocMemInitNode(ListNode** pNode);

//Create a double link list
ListNode* CreatMyList(ListNode* pHeadNode);

//Reset a linklist to empty
void ClearMyList(ListNode** pHeadNode);

//Delete the headnode to delete the whole list
//bool DeleteFirstNode(ListNode** ppHeadNode);

//Find any node with the specific task id
//_timer_id FindNode(ListNode* pHeadNode, _task_id tid);

//Insert a new node at tail
ListNode* InsertNode(ListNode* pHeadNode, _task_id tid, uint32_t deadline,uint32_t task_type, uint32_t creation_time);

//Delete tail node
bool DeleteHeadNode(ListNode** ppHeadNode);
//Fetch the headnode, return a pointer to headnode
ListNode* GetLastNode(ListNode* pHeadNode);


//Get the length of the current linklist
//int GetListLen(ListNode* pHeadNode);

// print all the data in the link list
void PrintfListDataNode(ListNode* pHeadNode);

//Check list empty
//bool CheckListEmpty(ListNode* pHeadNode);

#endif /* ifndef __linklist_H*/
