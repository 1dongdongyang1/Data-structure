#include<stdio.h>
#include<stdlib.h>

#include"Queue.h"

typedef char BTDataType;
typedef struct BinaryTreeNode
{
	BTDataType _data;
	struct BinaryTreeNode* _left;
	struct BinaryTreeNode* _right;
}BTNode;

void PrevOrder(BTNode* root)
{
	if (!root)
		return;
	printf("%c ", root->_data);
	PrevOrder(root->_left);
	PrevOrder(root->_right);
}

void InOrder(BTNode* root)
{
	if (!root)
		return;
	InOrder(root->_left);
	printf("%c ", root->_data);
	InOrder(root->_right);
}

void PostOrder(BTNode* root)
{
	if (!root)
		return;
	PostOrder(root->_left);
	PostOrder(root->_right);
	printf("%c ", root->_data);	
}

BTNode* CreateNode(BTDataType x)
{
	BTNode* node = (BTNode*)malloc(sizeof(BTDataType));
	node->_data = x;
	node->_left = NULL;
	node->_right = NULL;
	return node;
}

int TreeSize(BTNode* root)
{
	if (!root)
		return 0;
	else
		return 1 + TreeSize(root->_left) + TreeSize(root->_right);
}

int TreeLeafSize(BTNode* root)
{
	if (!root)
		return 0;
	if (root->_left == NULL && root->_right == NULL)
		return 1;
	return TreeLeafSize(root->_left) + TreeLeafSize(root->_right);
}

int BinaryTreeLevelKSize(BTNode* root, int k)
{
	if (!root)
		return 0;

	if (k == 1)
		return 1;

	return BinaryTreeLevelKSize(root->_left, k - 1) + BinaryTreeLevelKSize(root->_right, k - 1);
}

BTNode* BinaryTreeFind(BTNode* root, BTDataType x)
{
	if (!root)
		return NULL;

	if (root->_data == x)
		return root;

	BTNode* node = BinaryTreeFind(root->_left, x);
	if (node)
		return node;

	node = BinaryTreeFind(root->_right, x);
	if (node)
		return node;

	return NULL;
}

void DestoryTree(BTNode* root)
{
	if (!root)
		return;

	DestoryTree(root->_left);
	DestoryTree(root->_right);
	free(root);
}

void BinaryTreeLevelOrder(BTNode* root)
{
	Queue q;
	QueueInit(&q);
	if (!root)
		return;

	QueuePush(&q, root);

	while (!QueueEmpty(&q))
	{
		BTNode* front = QueueFront(&q);
		QueuePop(&q);

		printf("%c ", front->_data);

		if (front->_left)
			QueuePush(&q, front->_left);

		if (front->_right)
			QueuePush(&q, front->_right);
	}
	QueueDestory(&q);
	printf("\n");

}

int BinaryTreeComplete(BTNode* root)
{
	Queue q;
	QueueInit(&q);
	if (!root)
		return 1;

	QueuePush(&q, root);

	while (!QueueEmpty(&q))
	{
		BTNode* front = QueueFront(&q);
		QueuePop(&q);

		if (!front)
			break;

		QueuePush(&q, front->_left);
		QueuePush(&q, front->_right);
	}

	while (!QueueEmpty(&q))
	{
		BTNode* front = QueueFront(&q);
		QueuePop(&q);
		
		if (front)
		{
			QueueDestory(&q);
			return 0;
		}

	}

	QueueDestory(&q);
	return 1;
}

