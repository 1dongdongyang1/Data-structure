#include"List.h"

void TestList1()
{
	ListNode* phead = ListInit();
	ListPushBack(phead, 1);
	ListPushBack(phead, 2);
	ListPushBack(phead, 3);
	ListPushBack(phead, 4);
	ListPrint(phead);

	ListPopBack(phead);
	ListPrint(phead);

	ListPushFront(phead, 2);
	ListPrint(phead);

	ListPopFront(phead);
	ListPrint(phead);

	//ListNode* pos = ListFind(phead,2);
	////ListErase(pos);
	//ListInsert(pos, 8);
	//ListPrint(phead);

	//ListDestory(&phead);
}

void TestList2()
{
	ListNode* newNode = ListInit();
	//free(newNode);
}


int main()
{
	TestList1();
	//TestList2();
	return 0;
}