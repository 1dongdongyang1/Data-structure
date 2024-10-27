#define  _CRT_SECURE_NO_WARNINGS
#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

typedef int HPDataType;
typedef struct Heap
{
	HPDataType* _a;
	int _size;
	int _capacity;
}Heap;

void AdjustDown(HPDataType* a, int n, int root);
void AdjustUp(HPDataType* a, int n, int child);
void Swap(HPDataType* p1, HPDataType* p2);


void HeapInit(Heap* php, HPDataType* a, int n);
void HeapDestory(Heap* php);
void HeapPush(Heap* php,HPDataType x);
void HeapPop(Heap* php);
HPDataType HeapTop(Heap* php);
void HeapSort(Heap* php);