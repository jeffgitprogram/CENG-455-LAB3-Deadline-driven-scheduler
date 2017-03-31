#include "linklist.h"
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>
#include <malloc.h> 

void MallocMemInitNode(ListNode** pNode){
	*pNode = (ListNode*) malloc ( sizeof(ListNode));
	
	if(*pNode == NULL){
		fprintf(stderr, "The allocating memory defeat.\n");

	}
	else{
		//initialize the node data here
		(*pNode)-> tid = 0;
		(*pNode)-> deadline = 0;
		(*pNode) -> task_type = 0;
		(*pNode) -> creation_time = 0;
		(*pNode) -> next = NULL;

	}
} 


//Create a link list
ListNode* CreatMyList(ListNode* pHeadNode){
	pHeadNode = NULL;
	return pHeadNode;
}



//Reset a linked list to empty
void ClearMyList(ListNode** pHeadNode){
	ListNode* curr = NULL;

	if(*pHeadNode == NULL){
		//print("The list is already empty, no need to clear\n");
		return;
	}
	while((*pHeadNode)->next !=NULL){
		curr = (*pHeadNode)->next;
		_mem_free(*pHeadNode);
		*pHeadNode = curr;
	}
	//CLear the last node or linked list has only one node
	if(*pHeadNode!= NULL){
			_mem_free(*pHeadNode);
			*pHeadNode= NULL;
	}
	//printf("The list in cleared\n");
}




//Insert a new node at tail, every time put a new task at the tail
ListNode* InsertNode(ListNode* pHeadNode, _task_id tid, uint32_t deadline,uint32_t task_type, uint32_t creation_time){
	ListNode* curr = pHeadNode;
	ListNode* newNode = NULL;
	//ListNode* Tail = pHeadNode;

	MallocMemInitNode(&newNode);
	 if(newNode == NULL)
    {  
        fprintf(stderr, "The new node's memory allocating defeat.\n");  
        return NULL;  
    } 


	 newNode -> tid = tid;
	 newNode -> deadline = deadline;
	 newNode -> task_type = task_type;
	 newNode -> creation_time = creation_time;
	 newNode -> next = NULL;
	
	if(curr == NULL)//empty list
	{
		pHeadNode = newNode;
	}
	else{//not the first node
		/*while(Tail->next!=NULL)
		{
			Tail = Tail->next;//Find the last node;
		}*/
		if(curr->next == NULL){//if only one node existent
			if(newNode->deadline < curr->deadline){//insert to right
				curr->next = newNode;
			}else{//insert to left
				newNode->next = curr;
				curr->next = 0;
				pHeadNode = newNode;
			}

		}else{//if more than one node
			while(curr != NULL){
				if(curr->next == NULL){//if curr node is the last node, insert at tail
					curr->next = newNode;
					//Tail = newNode;//Locate the new tail
					break;
				}else{// insert in the middle
					if(curr == pHeadNode){
						if(newNode->deadline >= curr->deadline){//insert to the head
							newNode->next = curr;
							pHeadNode = newNode;
							break;
						}
					}
					if((newNode->deadline)<(curr->deadline) && (newNode->deadline)>=(curr->next->deadline)){
						newNode->next = curr->next;
						curr->next = newNode;
						break;
					}
					else{
						curr = curr->next;
					}
				}
			}
			//Tail->next = NULL;
		}

	}
	return pHeadNode;
}

//Delete tail node, delete the task at the node, consider the tail node as active task.
bool DeleteTailNode(ListNode** pHeadNode){
	ListNode* prev = * pHeadNode;
	ListNode* curr = NULL;
	
	if(* pHeadNode == NULL)//Determine if is there a list
	{
		fprintf(stderr, "There is no list.\n");
        return FALSE; 
	}
	
	if(prev->next == NULL)//If true, list have last node
	{
		//_mem_free(prev);
		* pHeadNode = NULL;
		//printf("Last node deleted. \n");
		return TRUE;

	}else{
		while((prev->next)->next != NULL){
		prev = prev-> next;
	}
		curr = prev->next;
		prev->next = 0;
		//_mem_free(curr);
		curr = NULL;

	
	    return TRUE;
	}
	
}

ListNode* GetLastNode(ListNode* pHeadNode){
		ListNode* prev = pHeadNode;
		ListNode* curr = NULL;

		if(pHeadNode == NULL)//Determine if is there a list
		{
			//fprintf(stderr, "There is no list.\n");
	        return NULL;
		}
		if(prev->next == NULL)//If true, list have last node
			{


				//printf("This is the last node. \n");
				return prev;

			}else{
		while((prev->next)->next != NULL){
			prev = prev-> next;
		}
		curr = prev->next;
		return curr;
	}
}
//Get the length of the current linklist

int Get_length(ListNode* pHeadNode){
	ListNode* curr = pHeadNode;
	int length =0;
	while(curr != NULL){
			length++;
			curr = curr->next;
		}
	return length;
}
// print all the data in the link list
void PrintfListDataNode(ListNode* pHeadNode){
	ListNode* curr = pHeadNode;
	printf("This linked list deadline is: ");
	while(curr != NULL){
		printf("%d ",curr->deadline);
		curr = curr->next;
	}
	printf("\n");
	curr = pHeadNode;
	printf("This linked list task id is: ");
	while(curr != NULL){
		printf("%d ",curr->tid);
		curr = curr->next;
	}

	printf("\n");
}

//bool CheckListEmpty(ListNode* pHeadNode);
