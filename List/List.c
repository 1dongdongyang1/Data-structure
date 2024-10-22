#include"List.h"

ListNode* ListInit()
{
	ListNode* newNode = BuyListNode(0);
	newNode->next = newNode;
	newNode->prev = newNode;
	return newNode;
}

void ListClear(ListNode* phead)
{
	assert(phead);
	ListNode* cur = phead->next;
	ListNode* prev = NULL;
	while (cur != phead)
	{
		prev = cur;
		cur = cur->next;
		free(prev);
	}
	phead->next = phead;
	phead->prev = phead;
}

void ListDestory(ListNode** pphead)
{
	assert(*pphead);
	ListClear(*pphead);
	free(*pphead);
	*pphead = NULL;
}

ListNode* BuyListNode(x)
{
	ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
	newNode->data = x;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

void ListPrint(ListNode* phead)
{
	assert(phead);
	ListNode* cur = phead->next;
	while (cur != phead)
	{
		printf("%d ", cur->data);
		cur = cur->next;
	}
	printf("\n");
}

void ListPushBack(ListNode* phead, LTDataType x)
{
	/*assert(phead);
	ListNode* newNode = BuyListNode(x);
	phead->prev->next = newNode;
	newNode->prev = phead->prev;
	newNode->next = phead;
	phead->prev = newNode;*/
	ListInsert(phead, x);
}

void ListPopBack(ListNode* phead)
{
	/*assert(phead);
	ListNode* tail = phead->prev;
	ListNode* newtail = tail->prev;
	if (tail == newtail)
	{
		printf("�޽ڵ��ɾ\n");
		return;
	}
	newtail->next = phead;
	phead->prev = newtail;
	free(tail);
	tail = NULL;*/
	ListErase(phead->prev);
}

void ListPushFront(ListNode* phead, LTDataType x)
{
	/*assert(phead);
	ListNode* newhead = BuyListNode(x);
	ListNode* head = phead->next;
	newhead->next = head;
	newhead->prev = phead;
	head->prev = newhead;
	phead->next = newhead;*/
	ListInsert(phead->next, x);
}

void ListPopFront(ListNode* phead)
{
	/*assert(phead);
	ListNode* head = phead->next;
	ListNode* newhead = head->next;
	phead->next = newhead;
	newhead->prev = phead;
	free(head);
	head = NULL;*/
	ListErase(phead->next);
}

ListNode* ListFind(ListNode* phead, LTDataType x)
{
	assert(phead);
	ListNode* cur = phead->next;
	while (phead != cur)
	{
		if (cur->data == x)
			return cur;
		cur = cur->next;
	}
	return NULL;
}
void ListInsert(ListNode* pos, LTDataType x)
{
	assert(pos);
	ListNode* prev = pos->prev;
	ListNode* newNode = BuyListNode(x);
	newNode->next = pos;
	pos->prev = newNode;
	prev->next = newNode;
	newNode->prev = prev;
}
void ListErase(ListNode* pos)
{
	assert(pos);
	ListNode* next = pos->next;
	ListNode* prev = pos->prev;
	prev->next = next;
	next->prev = prev;
	free(pos);
}