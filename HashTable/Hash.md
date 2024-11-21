# Hash

[toc]



## 1.散列的基本概念

> **哈希**和**散列**在计算机科学中可以理解为**同一个概念的不同翻译**，二者本质上是等价的。

1. **散列(Hash)**

   + **散列（Hashing）是计算机科学中一种将数据映射为固定长度值的技术，目的是为了高效地存储、查找和处理数据**。

   + 散列的核心思想是通过一个函数将输入数据转换为一个唯一的索引值，然后利用这个索引值快速存储或查找数据。

2. **散列函数(Hash Function)**

   + 输入：可以是字符串、数字、对象等。
   + 输出：固定长度的哈希值（通常是整数）。
   + 特性：
     + **确定性**：相同输入总会得到相同的输出。
     + **快速性**：计算速度快。
     + **均匀性**：尽量将输入均匀分布到所有可能的哈希值中。

3. **哈希值(Hash Value)**

   + 是通过哈希函数计算得到的结果，用于决定数据的存储位置或进行数据比较。

4. **散列表(Hash Table)**

   + 一个基于散列思想的数据结构，将数据存储在一个数组中，利用哈希值作为数组索引。
   + 优点：查找、插入和删除操作的时间复杂度接近 O(1)。

5. **冲突(Collision)**

   + 由于哈希值的范围有限，不同的输入可能映射到相同的哈希值，称为**哈希冲突**。
   + 冲突的处理方法包括**链地址法**、**开放地址法**等。

## 2.常见哈希函数

1. 直接定址法：取关键字的某个线性函数值为散列地址。即H(key) = a*key + b，其中a和b为常数。
   + 优点：简单，均匀，不会产生哈希冲突。
   + 缺点：需要事先知道关键字的分布情况
   + 使用场景：适合查找比较小且连续的情况
2. 除留余数法：**公式**：h(k)=k % m，其中 m 是哈希表大小且应选择为**质数**以避免周期性冲突。

## 3.哈希冲突解决方法

### 3.1开放地址法

**核心思想**

1. **冲突处理**：当插入一个键时，如果其目标位置已经被占用，则使用某种**探测方式**寻找下一个可用的槽位。
2. **重新计算位置**：通过一个**探测函数**，根据当前的冲突状态，计算新的位置。
3. **单一表存储**：所有的键值都存储在同一个哈希表中，不需要额外的存储结构。

**常见探测方式**

1. **线性探测(Linear Probing)**

   **方法**：从发生冲突的位置开始，**依次**向后探测，直到寻找到下一个空位置为止。

2. **二次探测(Quadratic Probing)**

   **方法**：在冲突时，按**二次方的增量**探测位置。

**实现**

```cpp
enum State
{
    EMPTY,
    EXITS,
    DELETE
};

template<class T>
struct HashData
{
    T _data;
    State _state;
};

template<class K, class T, class KeyOfT>
class HashTable
{
    typedef HashData<T> HashData;
public:
    bool Insert(const K& d)
    {
        KeyOfT koft;

        //capacity expansion
        if (_tables.size() == 0 || _num * 10 / _tables.size() >= 7)
        {
            /************************************************************
            vector<HashData> newtable;
            size_t newsize = _tables.size() == 0 ? 10 : _tables.size() * 2;
            newtable.resize(newsize);
            for (size_t i = 0; i < _tables.size(); i++)
            {
                if (_tables[i]._state == EXITS)
                {
                    size_t index = koft(_tables[i]._data) % newsize;
                    while (newtable[index]._state == EXITS)
                    {
                        ++index;
                        if (index == newtable.size()) index = 0;
                    }
                    newtable[index] = _tables[i];
                }
            }
            _tables.swap(newtable);
            ******************************************************************/ 

            //modern writing
            HashTable<K, T, KeyOfT> newht;
            size_t newsize = _tables.size() == 0 ? 10 : _tables.size() * 2;
            newht._tables.resize(newsize);
            for (size_t i = 0; i < _tables.size(); i++)
            {
                if (_tables[i]._state == EXITS)
                {
                    newht.Insert(_tables[i]);
                }
            }
            _tables.swap(newht._tables);
        }

        //Linear Probing
        /*******************************************************************
        size_t index = koft(d) % _tables.size();
        while (_tables[index]._state == EXITS)
        {
            if (koft(_tables[index]._data) == d) return false;

            index++;
            if (index == _tables.size()) index = 0;
        }
        *******************************************************************/

        //Quadratic Probing
        size_t start = koft(d) % _tables.size();
        size_t index = start;
        int i = 1;
        while (_tables[index]._state == EXITS)
        {
            if (koft(_tables[index]._data) == d) return false;

            index = start + i * i;
            index %= _tables.size();
        }

        _tables[index]._data = d;
        _tables[index]._state = EXITS;
        _num++;
        return true;
    }

    bool Erase(const K& d)
    {
        KeyOfT koft;
        size_t index = koft(d) % _tables.size();
        size_t start = index;
        size_t i = 1;
        //Quadratic Probing
        while (_tables[index]._state != EMPTY)
        {
            if (_tables[index]._state == EXITS && koft(_tables[index]._data) == d)
            {
                _tables[index]._state = DELETE;
                _num--;
                return true;
            }

            index = start + i * i;
            if (index == _tables.size()) index = 0;
            if (index == start) break;
        }
        return false;
    }
private:
    vector<HashData> _tables;
    size_t _num;
};
```

### 3.2链地址法

**储存方式**：

+ 每个哈希表的槽（数组元素）都指向一个链表的头节点，链表中存储与该槽位置对应的所有元素。
+ 当有元素插入时，如果该位置已被占用，则将新元素添加到该位置的链表中。

```cpp
template<class T>
struct HashData
{
    T _data;
    HashData<T>* _next;

    HashData(const T& data)
        :_data(data)
        ,_next(nullptr)
    {}
};

template<class K, class T, class KeyOfT>
class HashTable
{
    typedef HashData<T> Node;
public:
    ~HashTable()
    {
        Clear();
    }

    void Clear()
    {
        for (size_t i = 0; i < _tables.size(); i++)
        {
            Node* cur = _tables[i];
            while (cur)
            {
                Node* next = cur->_next;
                delete cur;
                cur = next;
            }
            _tables[i] = nullptr;
        }
    }

    bool Insert(const K& data)
    {
        KeyOfT koft;

        //capacity expansion
        if (_tables.size() == _num)
        {
            //remove the nodes from the old table, 
            //recalculate their position in the new table,
            //and insert them 
            vector<Node*> newtables;
            size_t newsize = _tables.size() == 0 ? 10 : _tables.size() * 2;
            newtables.resize(newsize);
            for (size_t i = 0; i < _tables.size(); i++)
            {
                Node* cur = _tables[i];
                while (cur)
                {
                    Node* next = cur->_next;
                    size_t index = koft(cur->_data) % newsize;
                    cur->_next = newtables[index];
                    newtables[index] = cur;

                    cur = next;
                }
                _tables[i] = nullptr;
            }
            _tables.swap(newtables);
        }

        //calculate the position of data in the table
        size_t index = koft(data) % _tables.size();

        //1.find out if this value is in the table
        Node* cur = _tables[index];
        while (cur)
        {
            if (koft(cur->_data) == koft(data)) return false;
            else cur = cur->_next;
        }

        //2.insert the head into the linked list
        Node* newnode = new Node(data);
        newnode->_next = _tables[index];
        _tables[index] = newnode;

        _num++;
        return true;
    }

    Node* Find(const K& key)
    {
        KeyOfT koft;
        size_t index = key % _tables.size();
        Node* cur = _tables[index];
        while (cur)
        {
            if (koft(cur->_data) == key) return cur;
            else cur = cur->_next;
        }
        return nullptr;
    }

    bool Erase(const K& key)
    {
        KeyOfT koft;
        size_t index = key % _tables.size();
        Node* cur = _tables[index];
        Node* pre = nullptr;
        while (cur)
        {
            if (koft(cur->_data) == key)
            {
                if (pre == nullptr) _tables[index] = cur->_next;
                else pre->_next = cur->_next;
                delete cur;
                _num--;
                return true;
            }
            else
            {
                pre = cur;
                cur = cur->_next;
            }
        }
        return false;
    }
private:
    vector<Node*> _tables;
    size_t _num = 0;
};
```



