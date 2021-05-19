#pragma once
#include <algorithm>

#define LEFT 0
#define RIGHT 1

template<class T>
class AVLT_node {
public:
	T data;
	int height;
	AVLT_node<T>* left;
	AVLT_node<T>* right;
	AVLT_node<T>* parent;


	AVLT_node(T data) {
		this->data = data;
		height = 1;
		left = nullptr;
		right = nullptr;
		parent = nullptr;
	}

	bool isRight() {
		return this->parent->right == this;
	}

	AVLT_node<T>* getChild(int dir) {
		return (dir == LEFT) ? this->left : this->right;
	}
};

template<class T>
int getHeight(AVLT_node<T>* node) {
	if (node == nullptr) {
		return 0;
	}
	return node->height;
}

template<class T>
int getBalance(AVLT_node<T>* node) {
	if (node == nullptr) {
		return 0;
	}
	return (node->left ? node->left->height : 0) - (node->right ? node->right->height : 0);
}

template<class T>
AVLT_node<T>* getMin(AVLT_node<T>* node) {
	AVLT_node<T>* current = node;

	while (current->left != nullptr) {
		current = current->left;
	}

	return current;
}

template<class T>
class AVLT {
private:
	AVLT_node<T>* root;
	int _size;

	void rotate(AVLT_node<T>* P, int dir) {
		AVLT_node<T>* GP = P->parent;
		AVLT_node<T>* C = P->getChild(1 - dir);
		AVLT_node<T>* CC = C->getChild(dir);

		if (dir == LEFT) {
			P->right = CC;
		}
		else {
			P->left = CC;
		}

		if (CC != NULL) {
			CC->parent = P;
		}

		if (dir == LEFT) {
			C->left = P;
		}
		else {
			C->right = P;
		}

		P->parent = C;
		C->parent = GP;
		if (GP != NULL) {
			if (GP->left == P) {
				GP->left = C;
			}
			else {
				GP->right = C;
			}
		}
		else {
			this->root = C;
		}

		P->height = 1 + std::max(getHeight(P->left), getHeight(P->right));
		C->height = 1 + std::max(getHeight(C->left), getHeight(C->right));
	}

	void rotateLeft(AVLT_node<T>* P) {
		rotate(P, LEFT);
	}

	void rotateRight(AVLT_node<T>* P) {
		rotate(P, RIGHT);
	}

	void balance(AVLT_node<T>* node, T data) {
		if (node == nullptr) {
			return;
		}

		int newHeight = 1 + std::max(getHeight(node->left), getHeight(node->right));
		if (newHeight == node->height) {
			return;
		}
		else {
			node->height = newHeight;
		}

		int balanceNum = getBalance(node);

		if (balanceNum > 1 && data < node->left->data) {
			rotateRight(node);
		}
		else if (balanceNum < -1 && data > node->right->data) {
			rotateLeft(node);
		}
		else if (balanceNum > 1 && data > node->left->data) {
			rotateLeft(node->left);
			rotateRight(node);
		}
		else if (balanceNum < -1 && data < node->right->data) {
			rotateRight(node->right);
			rotateLeft(node);
		}
		else {
			balance(node->parent, data);
		}

	}

	bool _contains(T data, AVLT_node<T>* node) {
		if (node == NULL) {
			return false;
		}
		if (node->data == data) {
			return true;
		}
		else if (node->data < data) {
			return _contains(data, node->right);
		}
		else {
			return _contains(data, node->left);
		}
	}

	void clear(AVLT_node<T>* root) {
		if (root == nullptr) {
			return;
		}

		clear(root->left);
		clear(root->right);
		delete root;
	}

	T sum(AVLT_node<T>* node) {
		if (node == nullptr) {
			return 0;
		}

		return sum(node->left) + node->data + sum(node->right);
	}

	void deleteByVal(T data, AVLT_node<T>* node) {
		if (data < node->data) {
			deleteByVal(data, node->left);
		}
		else if (data > node->data) {
			deleteByVal(data, node->right);
		}
		else {
			if (node->left == nullptr || node->right == nullptr) {
				AVLT_node<T>* temp = node->left ? node->left : node->right;
				if (temp == nullptr) {
					if (node->parent != nullptr) {
						if (node->isRight()) {
							node->parent->right = nullptr;
						}
						else {
							node->parent->left = nullptr;
						}
					}
					else {
						root = nullptr;
					}
					temp = node;
					node = nullptr;
				}
				else {
					node->data = temp->data;
					node->left = temp->left;
					node->right = temp->right;
					node->height = temp->height;
				}
				delete temp;
			}
			else {
				AVLT_node<T>* temp = getMin(node->right);
				node->data = temp->data;
				deleteByVal(temp->data, node->right);
			}
		}

		if (node == nullptr) {
			return;
		}

		node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
		int balance = getBalance(node);

		if (balance > 1 && getBalance(node->left) >= 0) {
			rotateRight(node);
		}
		else if (balance > 1 && getBalance(node->left) < 0) {
			rotateLeft(node->left);
			rotateRight(node);
		}
		else if (balance < -1 && getBalance(node->right) <= 0) {
			rotateLeft(node);
		}
		else if (balance < -1 && getBalance(node->right) > 0) {
			rotateRight(node->right);
			rotateLeft(node);
		}
	}

	static void _getSorted(AVLT_node<T>* node, std::vector<T>& v) {
		if (node->left)
			_getSorted(node->left, v);
		v.push_back(node->data);
		if (node->right)
			_getSorted(node->right, v);
	}
public:


	AVLT_node<T>* getRoot() {
		return root;
	}

	AVLT() {
		root = nullptr;
		_size = 0;
	}

	AVLT(T data) {
		root = new AVLT_node<T>(data);
		_size = 1;
	}

	void insert(T data) {
		if (root == nullptr) {
			root = new AVLT_node<T>(data);
			_size = 1;
			return;
		}
		AVLT_node<T>* current = root;
		AVLT_node<T>* newNode = new AVLT_node<T>(data);
		_size++;
		while (true) {
			if (data == current->data) {
				delete newNode;
				_size--;
				return;
			}
			else if (data < current->data) {
				if (current->left == NULL) {
					current->left = newNode;
					newNode->parent = current;
					break;
				}
				else {
					current = current->left;
				}
			}
			else {
				if (current->right == NULL) {
					current->right = newNode;
					newNode->parent = current;
					break;
				}
				else {
					current = current->right;
				}
			}
		}

		current->height = 1 + std::max(getHeight(current->left), getHeight(current->right));
		if (current->parent != nullptr) {
			balance(current->parent, data);
		}

	}

	bool contains(T data) {
		return _contains(data, root);
	}

	bool erase(T data) {
		if (!contains(data)) {
			return false;
		}
		else {
			deleteByVal(data, root);
			_size--;
			return true;
		}
	}

	int size() {
		return _size;
	}

	void clear() {
		clear(root);
		_size = 0;
	}

	T sum() {
		return sum(root);
	}

	std::vector<T> getSorted() {
		std::vector<T> res;
		if (root)
			_getSorted(root, res);
		return res;
	}
};