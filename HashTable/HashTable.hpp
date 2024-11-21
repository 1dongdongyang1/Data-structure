#pragma once

#include <vector>
using namespace std;

namespace close_hash
{
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
			
			//linear detection
			/*******************************************************************
			size_t index = koft(d) % _tables.size();
			while (_tables[index]._state == EXITS)
			{
				if (koft(_tables[index]._data) == d) return false;

				index++;
				if (index == _tables.size()) index = 0;
			}
			*******************************************************************/

			//quadratic detection
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
}

namespace open_hash
{
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

	template<class T>
	struct SetKeyOfT
	{
		const T& operator()(const T& T)
		{
			return T;
		}
	};

	void test1()
	{
		HashTable<int, int, SetKeyOfT<int>> ht;
		ht.Insert(2);
		ht.Insert(32);
		ht.Insert(1);
		ht.Insert(31);
		ht.Insert(9);
		ht.Insert(0);
		
		ht.Erase(32);
	}
}