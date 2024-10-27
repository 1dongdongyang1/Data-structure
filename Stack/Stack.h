#pragma once
#define  _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<assert.h>
#include<stdlib.h>

typedef int STDataType;
typedef struct Stack
{
	STDataType* _a;
	int _top; 
	int _capacity;
}Stack;

void StackInit(Stack* pst);
void StackDestory(Stack* pst);
void StackPush(Stack* pst, STDataType x);
void StackPop(Stack* pst);
int StackSize(Stack* pst);
int StackEmpty(Stack* pst);
STDataType StackTop(Stack* pst);


