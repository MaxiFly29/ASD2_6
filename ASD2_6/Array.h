#pragma once
#include <algorithm>
#include <vector>

template <typename T>
class Array
{
public:
	void resize(int size)
	{
		capacity = size;
		if (begin)
		{
			T* temp_b = new int[size], * temp_e = temp_b + this->size();
			std::copy(begin, end, temp_b);
			delete[] begin;
			begin = temp_b; end = temp_e;
		}
		else
		{
			begin = new int[size]; end = begin;
		}
	}
	int size() const { return end - begin; }
	bool empty() const { return end == begin; }
	T& operator[](int index) { return begin[index]; }
	T& front() { return *begin; }
	T& back() { return *(end - 1); }
	const T& at(int index) const { return begin[index]; }
	const T& front() const { return *begin; }
	const T& back() const { return *(end - 1); }
	T insert(T value, int index = 0)
	{
		displaceR(index);
		begin[index] = value;
		return value;
	}
	T push_back(T value)
	{
		displaceR(size());
		begin[size() - 1] = value;
		return value;
	}
	T push_front(T value)
	{
		displaceR(0);
		begin[0] = value;
		return value;
	}
	void eraseByIndex(int index)
	{
		displaceL(index);
	}
	void pop_back()
	{
		displaceL(size() - 1);
	}
	void pop_front()
	{
		displaceL(0);
	}
	bool eraseByValue(T value)
	{
		for (int i = 0; i < size(); i++)
		{
			if (value == begin[i])
			{
				eraseByIndex(i);
				return true;
			}
		}
		return false;
	}
	void clear() { end = begin; }
	int getIndexOf(T value) const
	{
		for (int i = 0; i < size(); i++)
		{
			if (value == begin[i])
			{
				return i;
			}
		}
		return -1;
	}
	T sum() const
	{
		T Sum = 0;
		for (int i = 0; i < size(); ++i)
		{
			Sum += begin[i];
		}
		return Sum;
	}
	std::vector<T> getSorted() const {
		std::vector<T> res(begin, end);
		std::sort(res.begin(), res.end());
		return res;
	}
private:
	void displaceR(int pos, int dist = 1)
	{
		if (size() == capacity)
		{
			if (capacity) resize(size() * 2);
			else resize(1);
		}
		int curr = size() - 1;
		end += dist;
		while (curr >= pos)
		{
			begin[curr + dist] = begin[curr];
			curr--;
		}
	}
	void displaceL(int pos)
	{
		while (++pos < size())
		{
			begin[pos - 1] = begin[pos];
		}
		--end;
	}
	T* begin = nullptr, * end = nullptr;
	int capacity = 0;
};
