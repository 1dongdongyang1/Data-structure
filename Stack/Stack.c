#include"Stack.h"

void StackInit(Stack* pst)
{
	assert(pst);
	pst->_a = (STDataType*)malloc(sizeof(STDataType) * 4);
	pst->_top = 0;
	pst->_capacity = 4;
}

void StackDestory(Stack* pst)
{
	assert(pst);
	free(pst->_a);
	pst->_a = NULL;
	pst->_capacity = pst->_top = 0;
}

void StackPush(Stack* pst, STDataType x)
{
	assert(pst);
	if (pst->_top == pst->_capacity)
	{
		pst->_capacity *= 2;
		STDataType* tem = (STDataType*)realloc(pst->_a, sizeof(STDataType) * pst->_capacity);
		if (tem == NULL)
		{
			printf("ÄÚ´æ²»×ã\n");
			exit(-1);
		}
		else
			pst->_a = tem;
	}
	pst->_a[pst->_top] = x;
	pst->_top++;
}

void StackPop(Stack* pst)
{
	assert(pst);
	assert(pst->_top > 0);
	pst->_top--;
}

int StackSize(Stack* pst)
{
	assert(pst);
	return pst->_top;
}

int StackEmpty(Stack* pst)
{
	assert(pst);
	return pst->_top == 0 ? 1 : 0;
}

STDataType StackTop(Stack* pst)
{
	assert(pst);
	assert(pst->_top > 0);
	return pst->_a[pst->_top - 1];
}
