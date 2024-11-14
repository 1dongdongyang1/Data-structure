# AVL树(Adelson-Velsky and Landis Tree)

[toc]

## 1.前言

> + 本文中，我们使用`KV`实现的AVL树。
> + **源码链接放在文章结尾**。

**AVL树**（**Adelson-Velsky and Landis Tree**）是一种**自平衡的二叉搜索树（BST）**，它基于二叉搜索树并通过平衡而得到。

AVL树的**增删查改**都是基于**二叉搜索树**的，如果不了解二叉搜索树，[点我进入**二叉搜索树**的世界](https://blog.csdn.net/2301_80030944/article/details/143752509)

+ 在前面的学习中我们提到，二叉搜索树可以提高搜索数据的效率，但在数据有序的情况下会退化为单支树，此时在树中查找元素就得遍历一整个分支，时间复杂度也会退化至O(N)。
+ 如果有一种算法，可以使二叉搜索树时刻保持左右子树的平衡，就可以避免这种最坏情况。
+ 两位俄罗斯的数学家***G.M.Adelson-Velskii***和***E.M.Landis***在1962年发明了并以他们的名字命名了**AVL树**，解决了上述问题。

## 2.AVL树的基本概念

+ **二叉搜索树的性质**：对于任何一个节点，它的左子树的所有键值都小于该节点的键值，右子树的所有键值都大于该节点的键值。

+ **平衡因子**：AVL树中的每个节点都会有一个**平衡因子**（balance factor）。

  ***本文定义***平衡因子是该节点的**右子树高度减去左子树高度的值**。具体来说：

  + 如果平衡因子为 `1`，说明右子树比左子树高。
  + 如果平衡因子为 `-1`，说明左子树比右子树高。
  + 如果平衡因子为 `0`，说明左右子树的高度相同。

  AVL树的要求是：**对于每个节点，平衡因子的绝对值不超过1**。如果某个节点的平衡因子绝对值大于1，就需要进行旋转操作来恢复平衡。

## 3.节点的定义

```cpp
template<class K, class V>
struct AVLTreeNode
{
    AVLTreeNode<K, V>* _left;      // 指向左子树的指针
    AVLTreeNode<K, V>* _right;     // 指向右子树的指针
    AVLTreeNode<K, V>* _parent;    // 指向父节点的指针，用于实现三叉链表（Three-pronged chain）
    
    int _bf;                       // 平衡因子 (Balance Factor)，用于判断树是否平衡

    pair<K, V> _kv;                // 存储键值对
    
    // 构造函数：初始化一个节点
    AVLTreeNode(const pair<K, V>& kv)
        : _left(nullptr), _right(nullptr), _parent(nullptr), _bf(0), _kv(kv)
    {}
};
```

## 4.插入

**思路**：

1. **插入节点**：
   + 首先执行标准的 **二叉搜索树**（BST）插入操作。通过循环，遍历树来找到合适的位置，然后插入新的节点 `cur`。
   + 对于每个节点，比较其键 `kv.first` 来决定插入到左子树还是右子树。
   + 如果树中已经存在相同的键值对（即 `kv.first == cur->_kv.first`），直接返回 `false`，表示不插入重复的元素。
2. **更新平衡因子**：
   + 如果插入的节点是父节点的左子节点，父节点的平衡因子减 `1`，如果插入的节点是父节点的右子节点，父节点的平衡因子增 `1`。
   + 如果更新后父节点的平衡因子的值为 `0`，则说明树的高度没变，更新停止。
   + 如果更新后父节点的平衡因子的值为 `1` 或 `-1`，则说明树的高度发生变化，因此继续向上回溯。
   + 如果更新后父节点的平衡因子的值为 `2` 或 `-2`，则说明树失衡，**旋转**处理后树的高度恢复到插入前，更新停止。

```cpp
template<class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;
public:
	bool Insert(const pair<K, V>& kv)
    {
        // 1. 按照二叉搜索树规则进行插入
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
            if (kv.first < cur->_kv.first) cur = cur->_left;
            else if (kv.first > cur->_kv.first) cur = cur->_right;
            else return false;  // 说明已有相同的键，不能插入
        }

        cur = new Node(kv);
        if (kv.first < parent->_kv.first) parent->_left = cur;
        else parent->_right = cur;
        cur->_parent = parent;

        // 2. 更新平衡因子
        while (parent)
        {
            if (cur == parent->_right) parent->_bf++;
            else parent->_bf--;

            // 如果平衡因子为0，说明树已经恢复平衡，退出循环
            if (parent->_bf == 0)
                break;
            else if (parent->_bf == 1 || parent->_bf == -1)
            {
                // 如果平衡因子的值为1或-1，说明不需要旋转，继续向上更新父节点
                cur = parent;
                parent = parent->_parent;
            }
            else if (parent->_bf == 2 || parent->_bf == -2)
            {
                // 如果平衡因子的值为2或-2，说明需要进行旋转
                // 旋转操作前先确保树是一个有效的搜索树
                if (parent->_bf == 2)
                {
                    if (cur->_bf == 1) RotateL(parent);  // 左旋
                    else if (cur->_bf == -1) RotateRL(parent);  // 右左旋
                }
                else if (parent->_bf == -2)
                {
                    if (cur->_bf == -1) RotateR(parent);  // 右旋
                    else if (cur->_bf == 1) RotateLR(parent);  // 左右旋
                }
                // 旋转后，树的高度恢复到插入前的水平，更新停止
                break;
            }
        }
        return true;
    }
private:
	Node* _root = nullptr;
}
```

## 5.旋转

### 1.左旋(RotateL)

左旋操作适用于右子树比左子树高的情况，具体步骤如下：

```cpp
void RotateL(Node* parent)
{
    Node* subR = parent->_right;          // 获取右子树
    Node* subRL = subR->_left;            // 获取右子树的左子树（可能是旋转后要更新的节点）
    Node* ppNode = parent->_parent;       // 获取父节点（parent的父节点）

    // 更新 parent 和 subR 的左右子树关系
    parent->_right = subRL;
    if (subRL) subRL->_parent = parent;   // 如果 subRL 存在，更新其父节点为 parent

    subR->_left = parent;                 // 将 subR 作为 parent 的左子树
    parent->_parent = subR;               // 更新 parent 的父节点为 subR

    // 如果 parent 是根节点，需要更新根节点
    if (_root == parent)
    {
        _root = subR;
        subR->_parent = nullptr;          // 使 subR 成为新根，父节点设为 null
    }
    else
    {
        // 否则更新 parent 在其父节点中的位置
        if (parent == ppNode->_left) ppNode->_left = subR;
        else ppNode->_right = subR;
        subR->_parent = ppNode;           // 更新 subR 的父节点
    }

    // 旋转后，parent 和 subR 的平衡因子归零
    parent->_bf = subR->_bf = 0;
}
```

+ **左旋** 的目标是将右子树的根节点提升为新的父节点，同时将原来的父节点变为左子树。
+ 旋转后，`parent` 和 `subR` 的平衡因子都应该设为 `0`，因为它们的高度没有变化。

### 2.右旋(RotateR)

右旋操作适用于左子树比右子树高的情况，步骤类似左旋：

```cpp
void RotateR(Node* parent)
{
    Node* subL = parent->_left;           // 获取左子树
    Node* subLR = subL->_right;           // 获取左子树的右子树
    Node* ppNode = parent->_parent;       // 获取父节点

    // 更新 parent 和 subL 的左右子树关系
    parent->_left = subLR;
    if (subLR) subLR->_parent = parent;   // 如果 subLR 存在，更新其父节点为 parent

    subL->_right = parent;                // 将 subL 作为 parent 的右子树
    parent->_parent = subL;               // 更新 parent 的父节点为 subL

    // 如果 parent 是根节点，需要更新根节点
    if (_root == parent)
    {
        _root = subL;
        subL->_parent = nullptr;          // 使 subL 成为新根
    }
    else
    {
        // 否则更新 parent 在其父节点中的位置
        if (parent == ppNode->_left) ppNode->_left = subL;
        else ppNode->_right = subL;
        subL->_parent = ppNode;           // 更新 subL 的父节点
    }

    // 旋转后，parent 和 subL 的平衡因子归零
    parent->_bf = subL->_bf = 0;
}
```

+ **右旋** 操作的目标是将左子树的根节点提升为新的父节点，同时将原来的父节点变为右子树。
+ 旋转后，`parent` 和 `subL` 的平衡因子都应该设为 `0`，因为它们的高度没有变化。

### 3.右左旋(RotateRL)

右左旋是先对右子树进行右旋，再对父节点进行左旋。这种旋转用于右子树的左子树比右子树高的情况：

```cpp
void RotateRL(Node* parent)
{
    Node* subR = parent->_right;          // 获取右子树
    Node* subRL = subR->_left;            // 获取右子树的左子树
    int bf = subRL->_bf;                  // 获取右左子树的平衡因子

    // 首先对右子树进行右旋
    RotateR(subR);

    // 然后对父节点进行左旋
    RotateL(parent);

    // 更新平衡因子
    if (bf == -1) subR->_bf = 1;         // 如果右左子树的平衡因子为 -1，设置右子树的平衡因子为 1
    else if (bf == 1) parent->_bf = -1;  // 如果右左子树的平衡因子为 1，设置父节点的平衡因子为 -1
    // 如果 bf == 0，平衡因子已是正确的，不需要修改
}
```

+ **右左旋** 通过两个旋转来恢复平衡，首先是右旋（对右子树进行旋转），然后是左旋（对父节点进行旋转）。
+ 旋转后，需要根据右左子树的平衡因子 `bf` 来调整旋转后节点的平衡因子。

### 4.左右旋(RotateLR)

左右旋是先对左子树进行左旋，再对父节点进行右旋。这种旋转用于左子树的右子树比左子树高的情况：

```cpp
void RotateLR(Node* parent)
{
    Node* subL = parent->_left;           // 获取左子树
    Node* subLR = subL->_right;           // 获取左子树的右子树
    int bf = subLR->_bf;                  // 获取左子树的右子树平衡因子

    // 首先对左子树进行左旋
    RotateL(subL);

    // 然后对父节点进行右旋
    RotateR(parent);

    // 更新平衡因子
    if (bf == -1) parent->_bf = 1;        // 如果左子树的右子树的平衡因子为 -1，设置父节点的平衡因子为 1
    else if (bf == 1) subL->_bf = -1;     // 如果左子树的右子树的平衡因子为 1，设置左子树的平衡因子为 -1
}
```

+ **左右旋** 通过两个旋转来恢复平衡，首先是左旋（对左子树进行旋转），然后是右旋（对父节点进行旋转）。
+ 旋转后，需要根据左子树的右子树的平衡因子 `bf` 来调整旋转后节点的平衡因子。

## 6.删除

**思路**：

1. **查找删除的节点**
2. **更新平衡因子**
   + 如果删除的节点是父节点的左子节点，父节点的平衡因子增 `1`，如果删除的节点是父节点的右子节点，父节点的平衡因子减 `1`。
   + 如果更新后父节点的平衡因子的值为 `1` 或 `-1`，则说明树的高度没变，更新停止。
   + 如果更新后父节点的平衡因子的值为 `0`，则说明树的高度发生变化，因此继续向上回溯。
   + 如果更新后父节点的平衡因子的值为 `2` 或 `-2`，则说明树失衡，**旋转**处理后树的高度恢复到插入前，更新停止。
3. **真正删除节点**

```cpp
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

Node* Find(const K& key)
{
	Node* cur = _root;
	while (cur)
	{
		if (key > cur->_key)	cur = cur->_right;
		else if (key < cur->_key) cur = cur->_left;
		else return cur;
	}
	return nullptr;
}

void DeleteNode(const Node& target)
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
		while (minNode) minNode = minNode->_left;
		target->_kv = minNode->_kv;
		DeleteNode(minNode);
	}
}
```

## 7.总结

+ **AVL树**（平衡二叉搜索树）通过平衡因子来调节树的平衡。
+ 比起二叉搜索树，AVL树多出来的是旋转的操作，特别是双旋，需要精准的把控旋转后的平衡因子，使AVL树实现起来很复杂。

## 8.源码链接

