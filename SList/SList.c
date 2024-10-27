#define _CRT_SECURE_NO_WARNINGS 1
#include"SList.h"

void SListPrint(SListNode* phead)
{
	if (phead!=NULL)
	{
		SListNode* cur = phead;
		while (cur != NULL)
		{
			printf("%d->", cur->data);
			cur = cur->next;
		}
		printf("NULL\n");
	}
}

SListNode* BuySListNode(SListDataType x)
{
	SListNode* newNode = (SListNode*)malloc(sizeof(SListNode));
	if (newNode == NULL)
	{
		exit(-1);
	}
	newNode->data = x;
	newNode->next = NULL;
	return newNode;
}

void SListPushBack(SListNode** pphead, SListDataType x)
{
	SListNode* newNode = BuySListNode(x);
	if (*pphead == NULL)
	{
		*pphead = newNode;
	}
	else
	{
		SListNode* tail = *pphead;
		while (tail->next != NULL)
		{
			tail = tail->next;
		}
		tail->next = newNode;
	}
	
}
void SListPopBack(SListNode** pphead)
{
	if (*pphead == NULL)
	{
		printf("无结点可删\n");
	}
	else if ((*pphead)->next == NULL)
	{
		free(*pphead);
		*pphead = NULL;
	}
	else
	{
		SListNode* tail = *pphead;
		SListNode* prev = NULL;
		while (tail->next != NULL)
		{
			prev = tail;
			tail = tail->next;
		}
		free(tail);
		prev->next = NULL;
	}
}
void SListPushFront(SListNode** pphead, SListDataType x)
{
	SListNode* newNode = BuySListNode(x);
	newNode->next = *pphead;
	*pphead = newNode;
}
void SListPopFront(SListNode** pphead)
{
	if (*pphead == NULL)
	{
		printf("无结点可删\n");
	}
	/*else if ((*pphead)->next == NULL)
	{
		free(*pphead);
		*pphead = NULL;
	}*/
	else
	{
		SListNode* head = (*pphead)->next;
		free(*pphead);
		*pphead = head;
	}
}

SListNode* SListFind(SListNode* phead, SListDataType x)
{
	SListNode* cur = phead;
	while (cur != NULL)
	{
		if (cur->data == x)
		{
			return cur;
		}
		cur = cur->next;
	}
	return NULL;
}