#pragma once
#include <iostream>
#include <utility>
using namespace std;

template<class K, class V>
struct AVLTreeNode
{

	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;	//Three-pronged chain

	int _bf;	//balance factor

	pair<K, V> _kv;

	AVLTreeNode(const pair<K, V>& kv)
		: _left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _bf(0)
		, _kv(kv)
	{}
};

template<class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;

public:
	bool Insert(const pair<K, V>& kv)
	{
		//1.insert according to the rules of binary search tree
		if (!_root)
		{
			_root = new Node(kv);
			return true;
		}

		Node* parent = nullptr;
		Node* cur = _root;
		while (cur)
		{
			parent = cur;
			if (kv.first < cur->_kv.first)	cur = cur->_left;
			else if (kv.first > cur->_kv.first)	cur = cur->_right;
			else return false;
		}

		cur = new Node(kv);
		if (kv.first < parent->_kv.first)	parent->_left = cur;
		else	parent->_right = cur;
		cur->_parent = parent;

		//2.update balance factor
		while (parent)
		{
			if (cur == parent->_right)	parent->_bf++;
			else parent->_bf--;

			if (parent->_bf == 0)
				break;
			else if (parent->_bf == 1 || parent->_bf == -1)
			{
				//upgrade
				cur = parent;
				parent = parent->_parent;
			}
			else if (parent->_bf == 2 || parent->_bf == -2)
			{
				//need to rotate
				//the premise of rotation is to keep it as a search tree
				if (parent->_bf == 2)
				{
					if (cur->_bf == 1)	RotateL(parent);
					else if (cur->_bf == -1)	RotateRL(parent);
				}
				else if (parent->_bf == -2)
				{
					if (cur->_bf == -1)	RotateR(parent);
					else if (cur->_bf == 1)		RotateLR(parent);
				}
				//the height is restored to the level before insertion and the update is stopped
				break;
			}
		}
		return true;
	}

	void InOrder()
	{
		_InOrder(_root);
	}

	bool IsBalance()
	{
		return _IsBalance(_root);
	}

	int Height()
	{
		return _Height(_root);
	}

	Node* Find(const K& key)
	{
		Node* cur = _root;
		while (cur)
		{
			if (key > cur->_kv.first)	cur = cur->_right;
			else if (key < cur->_kv.first) cur = cur->_left;
			else return cur;
		}
		return nullptr;
	}

	bool Erase(const K& key)
	{
		//1.find erase node
		Node* target = Find(key);
		if (!target)	return false;

		//2.update balance factor
		Node* parent = target->_parent;
		Node* child = target;
		while (parent)
		{
			if (child == parent->_left)	parent->_bf++;
			else parent->_bf--;

			if (parent->_bf == 1 || parent->_bf == -1) break;
			else if (parent->_bf == 0) child = parent, parent = parent->_parent;
			else if (parent->_bf == 2 || parent->_bf == -2)
			{
				if (parent->_bf == 2)
				{
					if (child->_bf == 1) RotateL(parent);
					else if (child->_bf == -1) RotateRL(parent);
				}
				else if(parent->_bf == -2)
				{
					if (child->_bf == -1) RotateR(parent);
					else if (child->_bf == 1) RotateLR(parent);
				}
			}
		}

		//3. delete node
		DeleteNode(target);
		return true;
	}
private:
	Node* _root = nullptr;

	void DeleteNode(Node* target)
	{
		Node* parent = target->_parent;
		if (!target->_left && !target->_right)	//leaf node
		{
			if (parent)
			{
				if (target == parent->_left)	parent->_left = nullptr;
				else parent->_right = nullptr;
			}
			else _root = nullptr;
			delete target;
		}
		else if (!target->_left || !target->_right) //only one child node
		{
			Node* child = target->_left ? target->_left : target->_right;
			if (parent)
			{
				if (target == parent->_left)	parent->_left = child;
				else parent->_right = child;
			}
			else _root = child;
			child->_parent = parent;
			delete target;
		}
		else //two child node
		{
			Node* minNode = target->_right;
			while (minNode->_left) minNode = minNode->_left;
			target->_kv = minNode->_kv;
			DeleteNode(minNode);
		}
	}

	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		Node* ppNode = parent->_parent;

		parent->_right = subRL;
		if (subRL)	subRL->_parent = parent;
		subR->_left = parent;
		parent->_parent = subR;

		if (_root == parent)
		{
			_root = subR;
			subR->_parent = nullptr;
		}
		else
		{
			if (parent == ppNode->_left)	ppNode->_left = subR;
			else ppNode->_right = subR;
			subR->_parent = ppNode;
		}

		parent->_bf = subR->_bf = 0;
	}

	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		Node* ppNode = parent->_parent;

		parent->_left = subLR;
		if (subLR)	subLR->_parent = parent;
		subL->_right = parent;
		parent->_parent = subL;

		if (_root == parent)
		{
			_root = subL;
			subL->_parent = nullptr;
		}
		else
		{
			if (parent == ppNode->_left)	ppNode->_left = subL;
			else ppNode->_right = subL;
			subL->_parent = ppNode;
		}

		parent->_bf = subL->_bf = 0;
	}

	void RotateRL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;

		RotateR(subR);
		RotateL(parent);

		//the rotation is over and the balance factors of the three nodes are all 0
		//we need to set the correct balance factor value
		if (bf == -1) subR->_bf = 1;
		else if (bf == 1) parent->_bf = -1;
		// when bf == 0, the balance factors of the three nodes should be 0, we didn't to modfiy it
	}

	void RotateLR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		int bf = subLR->_bf;

		RotateL(subL);
		RotateR(parent);

		//same as above
		if (bf == -1) parent->_bf = 1;
		else if (bf == 1) subL->_bf = -1;
	}

	void _InOrder(Node* root)
	{
		if (!root)	return;

		_InOrder(root->_left);
		cout << root->_kv.first << ":" << root->_kv.second << endl;
		_InOrder(root->_right);
	}

	bool _IsBalance(Node* root)
	{
		if (!root)	return true;

		int leftHeight = _Height(root->_left);
		int rightHeight = _Height(root->_right);

		return abs(leftHeight - rightHeight) < 2 && _IsBalance(root->_left) && _IsBalance(root->_right);
	}

	int _Height(Node* root)
	{
		if (!root) return 0;

		int leftHeight = _Height(root->_left);
		int rightHeight = _Height(root->_right);

		return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
	}
};

void TestAVLTree()
{
	int a[] = { 1,2,3,4,5,6 };
	AVLTree<int, int> t;
	for (auto e : a)
	{
		t.Insert(make_pair(e, e));
	}
	t.InOrder();
	cout << t.IsBalance() << endl;
	cout << t.Erase(2) << endl;
	cout << t.IsBalance() << endl;

}