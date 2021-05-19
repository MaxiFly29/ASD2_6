#pragma once

#include <stdexcept>
#include <algorithm>
#include <vector>

template <class T>
struct DoublyLinkedNode
{
	DoublyLinkedNode<T>* next;
	DoublyLinkedNode<T>* previous;
	T value;
	DoublyLinkedNode(T value) {
		this->next = NULL;
		this->previous = NULL;
		this->value = value;
	}
};

template <class T>
class DoublyLinkedList {
private:
	DoublyLinkedNode<T>* head;
	DoublyLinkedNode<T>* tail;
	int length;
public:
	DoublyLinkedList() {
		this->head = NULL;
		this->tail = NULL;
		this->length = 0;
	}

	DoublyLinkedList(T data) {
		DoublyLinkedNode* newNode = new DoublyLinkedNode(data);
		this->head = newNode;
		this->tail = newNode;
		this->length = 1;
	}

	T insert(T value, int index = 0) { //returns an added value
		if (index < 0 || index > this->length) {
			throw std::invalid_argument("Index is invalid");
		}
		DoublyLinkedNode<T>* newNode = new DoublyLinkedNode<T>(value);
		if (this->length == 0) {
			this->head = newNode;
			this->tail = newNode;
		}
		else if (index == 0) {
			newNode->next = this->head;
			this->head->previous = newNode;
			this->head = newNode;
		}
		else if (index == this->length) {
			this->tail->next = newNode;
			newNode->previous = this->tail;
			this->tail = newNode;
		}
		else {
			DoublyLinkedNode<T>* current = NULL;
			if (index < this->length / 2) {
				current = this->head;
				for (int i = 0; i < index; i++) {
					current = current->next;
				}
			}
			else {
				current = this->tail;
				for (int i = this->length - 1; i > index; i--) {
					current = current->previous;
				}
			}
			newNode->next = current;
			newNode->previous = current->previous;
			current->previous = newNode;
			newNode->previous->next = newNode;
		}
		this->length++;
		return value;
	}

	T push_back(T value) {
		return this->insert(value, this->length);
	}

	T push_front(T value) {
		return this->insert(value, 0);
	}

	bool eraseByValue(T value) { //TODO
		DoublyLinkedNode<T>* current = this->head;
		while (current != NULL) {
			if (current->value == value) {
				if (current->previous == NULL && current->next == NULL) {
					this->head = NULL;
					this->tail = NULL;
				}
				else if (current->previous == NULL) {
					this->head = this->head->next;
				}
				else if (current->next == NULL) {
					this->tail = this->tail->previous;
				}
				else {
					current->previous->next = current->next;
					current->next->previous = current->previous;
				}
				delete current;
				this->length--;
				return true;
			}
			current = current->next;
		}
		return false;
	}

	void eraseByIndex(int index) {
		if (index < 0 || index >= this->length) {
			throw std::invalid_argument("Index is invalid");
		}
		if (this->length == 1) {
			delete this->head;
			this->head = NULL;
			this->tail = NULL;
		}
		else if (index == 0) {
			DoublyLinkedNode<T>* newHead = this->head->next;
			delete this->head;
			this->head = newHead;
		}
		else if (index == this->length - 1) {
			DoublyLinkedNode<T>* newTail = this->tail->previous;
			delete this->tail;
			this->tail = newTail;
		}
		else {
			DoublyLinkedNode<T>* current = NULL;
			if (index < this->length / 2) {
				current = this->head;
				for (int i = 0; i < index; i++) {
					current = current->next;
				}
			}
			else {
				current = this->tail;
				for (int i = this->length - 1; i > index; i--) {
					current = current->previous;
				}
			}

			current->next->previous = current->previous;
			current->previous->next = current->next;
			delete current;
		}
		this->length--;
	}

	void pop_back() {
		if (this->length == 0) {
			throw std::logic_error("Can't delete from an empty List");
		}
		eraseByIndex(this->length - 1);
	}

	void pop_front() {
		if (this->length == 0) {
			throw std::logic_error("Can't delete from an empty List");
		}
		eraseByIndex(0);
	}

	void clear() {
		DoublyLinkedNode<T>* current = this->head;
		while (current != NULL) {
			DoublyLinkedNode<T>* toDelete = current;
			current = current->next;
			delete toDelete;
		}
		this->length = 0;
		this->head = NULL;
		this->tail = NULL;
	}

	T& operator[](int index) {
		if (index < 0 || index >= this->length) {
			throw std::invalid_argument("Index is invalid");
		}

		DoublyLinkedNode<T>* current = NULL;
		if (index < this->length / 2) {
			current = this->head;
			for (int i = 0; i < index; i++) {
				current = current->next;
			}
		}
		else {
			current = this->tail;
			for (int i = this->length - 1; i > index; i--) {
				current = current->previous;
			}
		}
		return current->value;
	}


	T& front() {
		return this->head->value;
	}

	T& back() {
		return this->tail->value;
	}

	const T& at(int index) const {
		if (index < 0 || index >= this->length) {
			throw std::invalid_argument("Index is invalid");
		}

		DoublyLinkedNode<T>* current = NULL;
		if (index < this->length / 2) {
			current = this->head;
			for (int i = 0; i < index; i++) {
				current = current->next;
			}
		}
		else {
			current = this->tail;
			for (int i = this->length - 1; i > index; i--) {
				current = current->previous;
			}
		}
		return current->value;
	}

	const T& front() const {
		return this->head->value;
	}

	const T& back() const {
		return this->tail->value;
	}

	int getIndexOf(T value) const { //returns -1 if there are no such elements
		DoublyLinkedNode<T>* current = this->head;
		for (int i = 0; i < this->length; i++) {
			if (current->value == value) {
				return i;
			}
			current = current->next;
		}
		return -1;
	}

	int size() const {
		return this->length;
	}

	bool empty() const {
		return this->length == 0;
	}

	T sum() const {
		T total = 0;
		DoublyLinkedNode<T>* current = this->head;
		while (current != NULL) {
			total += current->value;
			current = current->next;
		}
		return total;
	}

	std::vector<T> getSorted() const {
		std::vector<T> res;
		res.reserve(length);
		for (auto node = head; node; node = node->next) {
			res.push_back(node->value);
		}
		std::sort(res.begin(), res.end());
		return res;
	}
};