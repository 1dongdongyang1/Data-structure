# 二叉搜索树(Binary Search Tree, BSTree)

[toc]

> + 文章中是使用 `K` 实现的二叉搜索树，除此之外还有使用 `KV` 实现的二叉搜索树，二者区别于存储不同，但原理是一样的。
>
> + **源码链接放在文章结尾**。

## 1.概念

1. **定义**

   二叉搜索树是一种特定类型的二叉树，它满足以下性质：

   + **节点的左子树**中所有节点的值都小于该节点的值。
   + **节点的右子树**中所有节点的值都大于该节点的值。
   + 每个子树也是二叉搜索树。

   这种结构使得二叉搜索树具有很好的查找、插入和删除的性能，特别是当树的高度较低时。

2. **特点**

   + 查找：平均时间复杂度为 O(log N)，最坏情况下为 O(N)。
   + 插入与删除：时间复杂度为 O(log N)，但在最坏情况下可能退化为 O(N)。
   + 支持动态排序，便于快速查找最小、最大值。

3. **例子**

   ```plaintext
           10
          /  \
         5   15
        / \    \
       3   7   20
   ```

   在这个二叉搜索树中：

   + `5` 是 `10` 的左子节点，且小于 `10`。
   + `15` 是 `10` 的右子节点，且大于 `10`。
   + 左子树的所有节点都小于根节点，右子树的所有节点都大于根节点。

## 2.节点的定义

```cpp
template<class K>
struct BSTreeNode
{
public:
    BSTreeNode<K>* _left;   // 指向左子树节点的指针
    BSTreeNode<K>* _right;  // 指向右子树节点的指针
    K _key;                 // 节点的关键字（值）

    // 构造函数，初始化关键字并将左右子树指针置为 nullptr
    BSTreeNode(const K& key)
        : _left(nullptr), _right(nullptr), _key(key)
    {}
};
```

## 3.插入

**思路**：

1. **检查根节点**：
   + 如果树为空（`_root == nullptr`），直接创建一个新的节点，并将其作为根节点。
2. **遍历树**：
   + 使用 `cur` 指针遍历树，`prev` 用来记录当前节点的父节点。通过比较 `key` 和 `cur->_key` 的大小关系来决定在左子树还是右子树继续查找。
   + 如果 `key` 大于当前节点的值，向右子树移动（`cur = cur->_right`）。
   + 如果 `key` 小于当前节点的值，向左子树移动（`cur = cur->_left`）。
   + 如果 `key` 等于当前节点的值，说明该元素已经存在于树中，不插入，直接返回 `false`。
3. **插入新节点**：
   + 当遍历到 `cur` 为 `nullptr` 时，说明找到了插入位置。
   + 创建一个新的节点 `cur = new Node(key)`，然后根据 `key` 和 `prev->_key` 的大小关系，决定将新节点插入到父节点的左子树还是右子树。

**代码实现**

```cpp
template<class K>
class BSTree
{
    typedef BSTreeNode<K> Node;
public:
	bool insert(const K& key)
    {
        if (_root == nullptr)
        {
            // 如果树为空，创建一个新的节点并设置为根节点
            _root = new Node(key);
            return true;
        }

        Node* cur = _root;
        Node* prev = nullptr;
        while (cur)
        {
            prev = cur;
            if (key > cur->_key)  // 如果 key 比当前节点的键大，往右子树查找
                cur = cur->_right;
            else if (key < cur->_key)  // 如果 key 比当前节点的键小，往左子树查找
                cur = cur->_left;
            else  // 如果 key 等于当前节点的键，返回 false（不允许插入重复值）
                return false;
        }

        // 到达插入点时，创建新节点
        cur = new Node(key);
        // 判断应该插入到左子树还是右子树
        if (key > prev->_key)
            prev->_right = cur;
        else
            prev->_left = cur;

        return true;
    }
private:
    Node* _root = nullptr;
}
```

## 4.遍历

二叉搜索数的遍历使用**中序**，因为中序遍历二叉搜索树会得到一个升序排列的序列。

**为什么 BST 的中序遍历是升序的？**

+ 对于每一个节点，它的左子树的所有节点的值都比当前节点小，而右子树的所有节点的值都比当前节点大。
+ 在中序遍历中：
  + 左子树会先被遍历，而左子树的节点都比根节点小。
  + 然后遍历根节点。
  + 最后遍历右子树，右子树的节点都比根节点大。

因此，按照这种遍历顺序，节点值会从小到大地被访问，最终输出的顺序就是升序的。

```cpp
template<class K>
class BSTree
{
    typedef BSTreeNode<K> Node;
public:
	void InOrder()
    {
        _InOrder(_root);  // 从根节点开始进行中序遍历
        cout << endl;     // 输出换行，便于输出格式的整齐
    }
private:
    Node* _root = nullptr;
    
    void _InOrder(Node* root)
    {
        if (root == nullptr) return;  // 如果当前节点为空，直接返回
        _InOrder(root->_left);        // 递归遍历左子树
        cout << root->_key << " ";    // 访问当前节点（输出节点的关键字）
        _InOrder(root->_right);       // 递归遍历右子树
    }
}
```

**代码分析**：

1. **`InOrder` 函数**：
   + 使用`InOrder`函数调用`_InOrder`函数，因为`_root`是私有成员，类外无法使用，只能通过公有成员来调用。
2. **递归的 `_InOrder` 函数**：
   + 该函数是一个递归函数，它接受一个节点指针 `root` 作为参数，表示当前遍历的节点。
   + 通过递归的方式，先遍历左子树 `_InOrder(root->_left)`，然后访问当前节点 `cout << root->_key`，最后递归遍历右子树 `_InOrder(root->_right)`。
   + 如果当前节点为空（`root == nullptr`），说明已经到达叶节点的子节点，递归结束。

## 5.查找

**思路**：

1. 遍历一遍树，有返回`true`，没有返回`false`。

```cpp
template<class K>
class BSTree
{
    typedef BSTreeNode<K> Node;
public:
	bool Find(const K& key)
	{
		Node* cur = _root;
		while (cur)
		{
			if (key > cur->_key)	cur = cur->_right;
			else if (key < cur->_key) cur = cur->_left;
			else return true;
		}
		return false;
	}
private:
    Node* _root = nullptr;
}
```

## 6.删除

**删除操作的三种情况：**

1. **删除的节点没有左子树**：
   + 如果删除节点没有左子树，则可以直接将其右子树提上来（即将其父节点的指针指向右子树）。
   + 如果节点是根节点，则根节点指向其右子树；如果是其他节点，则父节点的左或右指针会指向该节点的右子树。
2. **删除的节点没有右子树**：
   + 如果删除节点没有右子树，则可以直接将其左子树提上来（即将其父节点的指针指向左子树）。
   + 如果节点是根节点，则根节点指向其左子树；如果是其他节点，则父节点的左或右指针会指向该节点的右子树。
3. **删除的节点有左右子树**：
   + 如果节点有左右子树，删除节点后需要找到右子树中的最小节点（即右子树最左边的节点），并将其值替换到当前节点。这样，当前节点就“变成”了右子树的最小节点。
   + 然后，删除右子树中的最小节点，连接其右子树。

**详细步骤：**

1. **查找节点**：首先，我们遍历树来查找目标节点（`cur`）。如果找到了目标节点，我们开始处理它。
2. **处理无子树的节点**：如果节点没有左子树或右子树，直接将父节点的指针指向子节点（或空），然后释放当前节点。
3. **处理有子树的节点**：如果节点有左右子树，找到右子树中的最小节点，替换当前节点的值，并删除右子树中的最小节点。
4. **返回结果**：如果找到了目标节点并成功删除，返回 `true`；否则，返回 `false`。

```cpp
template<class K>
class BSTree
{
    typedef BSTreeNode<K> Node;
public:
    bool Erase(const K& key)
    {
        Node* cur = _root;        // 当前节点初始化为根节点
        Node* prev = nullptr;     // prev 用来记录当前节点的父节点

        while (cur)  // 遍历树，直到找到要删除的节点或树为空
        {
            prev = cur;
            if (key > cur->_key)  cur = cur->_right;  // 如果 key 大于当前节点的值，查找右子树
            else if (key < cur->_key) cur = cur->_left;  // 如果 key 小于当前节点的值，查找左子树

            if (key == cur->_key)  // 找到目标节点
            {
                if (cur->_left == nullptr)  // 如果当前节点没有左子树
                {
                    if (cur == _root)  // 如果当前节点是根节点
                        _root = cur->_right;  // 让根节点指向右子树
                    else
                    {
                        if (prev->_left == cur)  // 如果是父节点的左子节点
                            prev->_left = cur->_right;  // 将父节点的左子树指向当前节点的右子树
                        else  // 如果是父节点的右子节点
                            prev->_right = cur->_right;  // 将父节点的右子树指向当前节点的右子树
                    }
                    delete cur;  // 释放当前节点的内存
                }
                else if (cur->_right == nullptr)  // 如果当前节点没有右子树
                {
                    if (cur == _root)  // 如果当前节点是根节点
                        _root = cur->_left;  // 让根节点指向左子树
                    else
                    {
                        if (prev->_left == cur)  // 如果是父节点的左子节点
                            prev->_left = cur->_left;  // 将父节点的左子树指向当前节点的左子树
                        else  // 如果是父节点的右子节点
                            prev->_right = cur->_left;  // 将父节点的右子树指向当前节点的左子树
                    }
                    delete cur;  // 释放当前节点的内存
                }
                else  // 当前节点有左右子树
                {
                    Node* rightMin = cur->_right;  // 从当前节点的右子树开始寻找最小的节点
                    Node* rightMinPrev = cur;  // 记录最小节点的父节点

                    // 找右子树中的最小节点（即左子树最深的节点）
                    while (rightMin->_left)
                    {
                        rightMinPrev = rightMin;
                        rightMin = rightMin->_left;
                    }

                    cur->_key = rightMin->_key;  // 将当前节点的值替换为右子树最小节点的值

                    // 删除右子树最小节点
                    if (rightMinPrev == cur)  // 如果最小节点是当前节点的右子节点
                        rightMinPrev->_right = rightMin->_right;  // 直接调整右子树
                    else  // 如果最小节点是当前节点右子树的某个更深节点
                        rightMinPrev->_left = rightMin->_right;  // 删除最小节点后，连接其右子树

                    delete rightMin;  // 释放最小节点的内存
                }
                return true;  // 成功删除节点
            }
        }
        return false;  // 没有找到节点，返回 false
    }
private:
    Node* _root = nullptr;  // 根节点初始化为空
};
```

## 7.修改

因为二叉搜索树的特点，所以不能对二叉搜索树的K值进行修改。

## 8.代码链接

[使用`K`实现的二叉搜索树](https://github.com/1dongdongyang1/Data-structure/blob/master/BSTree/BSTree_K.hpp)

[使用`KV`实现的二叉搜索树](https://github.com/1dongdongyang1/Data-structure/blob/master/BSTree/BSTree_KV.hpp)









