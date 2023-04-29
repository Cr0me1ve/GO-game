#pragma once
#include <exception>
#include <iostream>
#include <string>

template <typename T>
class List {	// Linked List
private:
	struct item {
		item() {}
		item(T val) { value = val; }
		~item() {}

		void set(T val) { value = val; }

		T value;
		item* next = nullptr;
	};

	item* start = nullptr;
	item* end = nullptr;
	item* temp = nullptr;
	item* last = start;

	int lastGet = -1;
	int len = 0;
public:
	List() {}

	~List() {	// clear every node
		while (start != 0) {
			temp = start->next;
			delete start;
			start = temp;
		}
	}

	List(int size) { setSize(size); }	// make list with given length

	List(const List& other) {
		for (int i = 0; i < other.size(); i++)
			push_back(other.getConst(i));
	}

	void push_back(T val) {		// push new item
		item* n = new item(val);
		if (len == 0) {
			start = n;
			end = start;
		}
		else {
			end->next = n;
			end = n;
		}
		len++;
	}

	void push_back() {			// push empty item
		T val;
		push_back(val);
	}

	void setSize(int n, T val) {	// make list with given length and given value
		for (int i = 0; i < n; i++)
			push_back(val);
	}

	void setSize(int s) {			// make list with given length and empty value
		setSize(s, T());
	}

	T& operator[] (int n) const {	// get element from list
		item* curr = start;
		if (n >= len || n < 0) {
			throw std::out_of_range("List index out of range");
			exit(-1);
		}
		for (int i = 0; i < n; i++)
			curr = curr->next;
		return curr->value;
	}

	T& set(int n) {		// set element in list
		item* curr = start;
		if (n >= len || n < 0) {
			throw std::out_of_range("List index out of range");
			exit(-1);
		}
		for (int i = 0; i < n; i++)
			curr = curr->next;
		return curr->value;
	}

	List<T>& operator=(List<T>& other) {	// copy list from other
		clear();
		for (int i = 0; i < other.size(); i++)
			push_back(other[i]);
		return *this;
	}

	bool operator==(const List<T>& other) {		// compare lists
		if (len != other.size())
			return false;
		for (int i = 0; i < len; i++)
			if (getConst(i) != other.getConst(i))
				return false;
		return true;
	}

	void concat(List<T>* other) {		// expand list with elements from other
		for (int i = 0; i < other->size(); i++)
			push_back(other->get(i));
	}

	void set(int n, T val) {		// set element in list
		item* curr = start;
		if (n >= len || n < 0) {
			throw std::out_of_range("List index out of range");
			exit(-1);
		}
		for (int i = 0; i < n; i++)
			curr = curr->next;
		curr->set(val);
	}

	T getConst(int n) const {	// get element from list (const func)
		item* curr = start;
		if (n >= len || n < 0) {
			throw std::out_of_range("List index out of range");
			exit(-1);
		}
		for (int i = 0; i < n; i++)
			curr = curr->next;
		return curr->value;
	}

	T get(int n) {		// get element from list
		item* curr = start;

		if (n >= len || n < 0) {
			throw std::out_of_range("List index out of range");
			exit(-1);
		}
		if (len > 0 && n + 1 == len)
			return end->value;

		if (n == lastGet)
			return last->value;
		else if (n > lastGet && lastGet != -1) {
			curr = last;
			for (int i = 0; i < n - lastGet; i++)
				curr = curr->next;
			last = curr;
			lastGet = n;
			return curr->value;
		}
		else {
			for (int i = 0; i < n; i++)
				curr = curr->next;
			last = curr;
			lastGet = n;
			return curr->value;
		}
	}

	T& getChangeable(int n) {	// get element from list which can be modified
		item* curr = start;

		if (n >= len || n < 0) {
			throw std::out_of_range("List index out of range");
			exit(-1);
		}
		if (len > 0 && n + 1 == len)
			return end->value;

		if (n == lastGet)
			return last->value;
		else if (n > lastGet && lastGet != -1) {
			curr = last;
			for (int i = 0; i < n - lastGet; i++)
				curr = curr->next;
			last = curr;
			lastGet = n;
			return curr->value;
		}
		else {
			for (int i = 0; i < n; i++)
				curr = curr->next;
			last = curr;
			lastGet = n;
			return curr->value;
		}
	}

	T* getPtr(int n) {		// get pointer to element
		item* curr = start;

		if (n >= len || n < 0) {
			throw std::out_of_range("List index out of range");
			exit(-1);
		}

		if (len > 0 && n + 1 == len)
			return &(end->value);

		if (n == lastGet)
			return &(last->value);
		else if (n > lastGet && lastGet != -1) {
			curr = last;
			for (int i = 0; i < n - lastGet; i++)
				curr = curr->next;
			last = curr;
			lastGet = n;
			return &(curr->value);
		}
		else {
			for (int i = 0; i < n; i++)
				curr = curr->next;
			last = curr;
			lastGet = n;
			return &(curr->value);
		}
	}

	T defaultGet(int n, T def) {	// if invalid "n" return default value
		item* curr = start;

		if (n >= len || n < 0)
			return def;
		else
			return get(n);
	}

	int size() const {		// get size of the list
		return len;
	}

	bool contain(const T val) {		// check if list contains given value
		for (int i = 0; i < len; i++)
			if (this->get(i) == val)
				return true;
		return false;
	}

	bool contain(const T* val) {	// check if list contains given value
		for (int i = 0; i < len; i++)
			if (get(i) == *val)
				return true;
		return false;
	}

	void clear() {		// clear list
		while (start != 0) {
			temp = start->next;
			delete start;
			start = temp;
		}
		start = 0;
		len = 0;
	}
};