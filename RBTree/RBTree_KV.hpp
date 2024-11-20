#pragma once

enum Color
{
	RED,
	BLACK
};

template<class K, class V>
struct RBTreeNode
{
	RBTreeNode<K, V>* _left;
	RBTreeNode<K, V>* _right;
	RBTreeNode<K, V>* _parent;

	pair<K, V> _kv;

	Color _col;

	RBTreeNode(const pair<K, V>& kv)
		:_left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _kv(kv)
		, _col(RED)		//newly added node is red
	{}
};

template<class K, class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
public:
	bool Insert(const pair<K, V>& kv)
	{
		//1.insert according to the rules of binary search tree
		if (!_root)
		{
			_root = new Node(kv);
			_root->_col = BLACK;
			return true;
		}

		Node* parent = nullptr;
		Node* cur = _root;
		while (cur)
		{
			parent = cur;
			if (kv.first < cur->_kv.first) cur = cur->_left;
			else if (kv.first > cur->_kv.first) cur = cur->_right;
			else return false;
		}

		cur = new Node(kv);
		if (kv.first < parent->_kv.first) parent->_left = cur;
		else parent->_right = cur;
		cur->_parent = parent;
		
		//2. update color
		while (parent && parent->_col == RED)
		{
			//the conditional judgment of RBTree mainly relies on uncle
			Node* grandfather = parent->_parent;
			if (grandfather->_left == parent)
			{
				Node* uncle = grandfather->_right;
				//Case 1, uncle exists and is red
				//Case 2, parent == grandfather->_left, uncle does not exist or is black
				//Case 3, parent == grandfather->_right, uncle does not exist or is black
				if (uncle && uncle->_col == RED)
				{
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					//upgrade
					cur = grandfather;
					parent = cur->_parent;
				}
				else //uncle does not exist or is black
				{
					
					if (cur == parent->_right)
					{
						RotateL(parent);
						swap(parent, cur);
					}

					
					RotateR(grandfather);
					grandfather->_col = RED;
					parent->_col = BLACK;

					break;
				}

			}
			else //grandfather->_right == parent
			{
				Node* uncle = grandfather->_left;
				if (uncle&& uncle->_col == RED)
				{
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					cur = grandfather;
					parent = cur->_parent;
				}
				else
				{
					if (cur == parent->_left)
					{
						RotateR(parent);
						swap(parent, cur);
					}
					RotateL(grandfather);
					grandfather->_col = RED;
					parent->_col = BLACK;
				}
			}
		}

		_root->_col = BLACK;
		return true;
	}
private:
	Node* _root = nullptr;

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
	}
};