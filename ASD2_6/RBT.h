#pragma once

#include <vector>

#define RED false
#define BLACK true
#define LEFT 0
#define RIGHT 1

template<class T>
class RBT_node {
public:
	T data;
	bool color;  //false - red, true - black
	RBT_node<T>* parent;
	RBT_node<T>* left;
	RBT_node<T>* right;

	RBT_node(T data, bool color) {
		this->data = data;
		this->color = color;
		this->parent = NULL;
		this->left = NULL;
		this->right = NULL;
	}

	int getDir() { //0 if left, 1 if right
		return (this->parent->left == this) ? LEFT : RIGHT;
	}

	RBT_node<T>* getChild(int dir) {
		return (dir == LEFT) ? this->left : this->right;
	}

	RBT_node<T>* getSibling() {
		if (this->parent == NULL) {
			return NULL;
		}
		return (this->parent->right == this) ? this->parent->left : this->parent->right;
	}

	RBT_node<T>* getUncle() {
		if ((parent == NULL) || (parent->parent == NULL)) {
			return NULL;
		}
		return this->parent->getSibling();
	}

	bool hasRedChild() {
		return ((left != NULL) && (left->color == RED) || (right != NULL) && (right->color == RED));
	}
};

template<class T>
class RBT {
private:
	RBT_node<T>* root;
	int _size;

	void swapColors(RBT_node<T>* a, RBT_node<T>* b) {
		bool temp = a->color;
		a->color = b->color;
		b->color = temp;
	}

	void swapData(RBT_node<T>* a, RBT_node<T>* b) {
		T temp = a->data;
		a->data = b->data;
		b->data = temp;
	}

	void balance(RBT_node<T>* node) {
		RBT_node<T>* P = NULL; //parent
		RBT_node<T>* GP = NULL; //grand parent
		while ((node != root) && (node->parent->color == RED) && (node->color == RED)) {
			P = node->parent;
			GP = node->parent->parent;
			RBT_node<T>* U = NULL;
			U = node->getUncle();
			if ((U != NULL) && (U->color == RED)) {
				P->color = BLACK;
				U->color = BLACK;
				GP->color = RED;
				node = GP;
			}
			else {
				if (node->getDir() != P->getDir()) {
					if (node->getDir() == RIGHT) {
						this->rotateLeft(P);
					}
					else {
						this->rotateRight(P);
					}
					node = P;
					P = node->parent;
				}
				if (node->getDir() == LEFT) {
					this->rotateRight(GP);
				}
				else {
					this->rotateLeft(GP);
				}
				swapColors(P, GP);
				node = P;
			}
		}
		root->color = BLACK;
	}

	bool _contains(T data, RBT_node<T>* node) {
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

	RBT_node<T>* getSuccessor(RBT_node<T>* root) {
		if ((root->left != NULL) && (root->right != NULL)) {
			RBT_node<T>* current = root->right;
			while (current->left != NULL) {
				current = current->left;
			}
			return current;
		}

		if ((root->left == NULL) && (root->right == NULL)) {
			return NULL;
		}

		if (root->left == NULL) {
			return root->right;
		}
		else {
			return root->left;
		}
	}


	void rotate(RBT_node<T>* P, int dir) {
		RBT_node<T>* GP = P->parent;
		RBT_node<T>* C = P->getChild(1 - dir);
		RBT_node<T>* CC = C->getChild(dir);

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
	}

	void rotateLeft(RBT_node<T>* P) {
		rotate(P, LEFT);
	}

	void rotateRight(RBT_node<T>* P) {
		rotate(P, RIGHT);
	}


	void deleteNode(RBT_node<T>* node) {
		RBT_node<T>* toSwapWith = getSuccessor(node);

		bool bothBlack = (((toSwapWith == NULL) || (toSwapWith->color == BLACK)) && (node->color == BLACK));

		if (toSwapWith == NULL) {
			if (node == root) {
				root = NULL;
			}
			else {
				if (bothBlack) {
					fixDoubleBlack(node);
				}

				if (node->getDir() == LEFT) {
					node->parent->left = NULL;
				}
				else {
					node->parent->right = NULL;
				}
			}
			delete node;
			return;
		}

		if ((node->left == NULL) || (node->right == NULL)) {
			if (node == root) {
				root = toSwapWith;
			}
			else {
				if (node->getDir() == LEFT) {
					node->parent->left = toSwapWith;
				}
				else {
					node->parent->right = toSwapWith;
				}
				toSwapWith->parent = node->parent;

				if (bothBlack) {
					fixDoubleBlack(toSwapWith);
				}
				else {
					toSwapWith->color = BLACK;
				}
			}
			delete node;
			return;
		}

		node->data = toSwapWith->data;
		deleteNode(toSwapWith);
	}

	void fixDoubleBlack(RBT_node<T>* node) {
		if (node == root) {
			return;
		}

		RBT_node<T>* sibling = node->getSibling();
		if (sibling == NULL) {
			fixDoubleBlack(node->parent);
		}
		else {
			if (sibling->color == RED) {
				sibling->color = BLACK;
				node->parent->color = RED;
				if (sibling->getDir() == LEFT) {
					rotateRight(node->parent);
				}
				else {
					rotateLeft(node->parent);
				}
				fixDoubleBlack(node);
			}
			else {
				if (sibling->hasRedChild()) {
					if ((sibling->right != NULL) && (sibling->right->color == RED)) {
						if (sibling->getDir() == RIGHT) {
							sibling->right->color = BLACK;
							sibling->color = node->parent->color;
							rotateLeft(node->parent);
						}
						else {
							sibling->right->color = node->parent->color;
							rotateLeft(sibling);
							rotateRight(node->parent);
						}
					}
					else {
						if (sibling->getDir() == LEFT) {
							sibling->left->color = BLACK;
							sibling->color = node->parent->color;
							rotateRight(node->parent);
						}
						else {
							sibling->left->color = node->parent->color;
							rotateRight(sibling);
							rotateLeft(node->parent);
						}

					}
					node->parent->color = BLACK;
				}
				else {
					sibling->color = RED;
					if (node->parent->color == RED) {
						node->parent->color = BLACK;
					}
					else {
						fixDoubleBlack(node->parent);
					}
				}
			}
		}
	}

	void deleteByVal(T data, RBT_node<T>* node) {
		if (data < node->data) {
			deleteByVal(data, node->left);
		}
		else if (data > node->data) {
			deleteByVal(data, node->right);
		}
		else {
			deleteNode(node);
		}
	}

	void display(RBT_node<T>* node) {
		if (node == NULL) {
			return;
		}
		display(node->left);
		std::cout << node->data << ' ';
		display(node->right);
	}

	void clear(RBT_node<T>* root) {
		if (root == NULL) {
			return;
		}
		clear(root->left);
		clear(root->right);
		delete root;
	}

	T sum(RBT_node<T>* node) {
		if (node == NULL) {
			return 0;
		}
		return sum(node->left) + node->data + sum(node->right);
	}

	static void _getSorted(RBT_node<T>* node, std::vector<T>& v) {
		if (node->left)
			_getSorted(node->left, v);
		v.push_back(node->data);
		if (node->right)
			_getSorted(node->right, v);
	}
public:
	RBT() {
		root = NULL;
		_size = 0;
	}

	RBT(T data) {
		root = new RBT_node<T>(data, true);
		_size = 1;
	}

	virtual ~RBT() {
		clear();
	}

	void insert(T data) {
		if (root == NULL) {
			root = new RBT_node<T>(data, BLACK);
			_size = 1;
			return;
		}
		RBT_node<T>* current = root;
		RBT_node<T>* newNode = new RBT_node<T>(data, RED);
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
		if (newNode->parent->color == RED) {
			this->balance(newNode);
		}
	}

	bool contains(T data) {
		return _contains(data, root);
	}

	void erase(T data) {
		if (!contains(data)) {
			return;
		}
		else {
			deleteByVal(data, root);
			_size--;
			return;
		}
	}

	int size() {
		return this->_size;
	}

	virtual void display() {
		display(root);
	}

	void clear() {
		clear(root);
		root = NULL;
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