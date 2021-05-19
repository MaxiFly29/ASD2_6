#pragma once
#include <memory>
#include <vector>

template<class T>
class Tree {
	struct Node {
		T val;
		std::shared_ptr<Node> l, r;
		Node(T val, std::shared_ptr<Node> l, std::shared_ptr<Node> r) : val(val), l(l), r(r) {}
	};
	std::shared_ptr<Node> mainNode = nullptr;
	size_t size_ = 0;
	void _insert(std::shared_ptr<Node> node, T value) {
		if (value < node->val) {
			if (node->l)
				_insert(node->l, value);
			else {
				node->l = std::make_shared<Node>(value, nullptr, nullptr);
				size_++;
			}
		}
		else if (value > node->val) {
			if (node->r)
				_insert(node->r, value);
			else {
				node->r = std::make_shared<Node>(value, nullptr, nullptr);
				size_++;
			}
		}
	}
	static void _insert(std::shared_ptr<Node> node, std::shared_ptr<Node> ins) {
		if (ins->val < node->val) {
			if (node->l)
				_insert(node->l, ins);
			else
				node->l = ins;
		}
		else if (ins->val > node->val) {
			if (node->r)
				_insert(node->r, ins);
			else
				node->r = ins;
		}
	}
	void _erase(std::shared_ptr<Node> node, T value) {
		if (!node)
			return;
		if (value < node->val) {
			if (!node->l)
				return;
			if (node->l->val == value) {
				auto toIns = node->l->r;
				node->l = node->l->l;
				size_--;
				if (toIns)
					_insert(node, toIns);
			}
			else
				_erase(node->l, value);
		}
		else if (value > node->val) {
			if (!node->r)
				return;
			if (node->r->val == value) {
				auto toIns = node->r->l;
				node->r = node->r->r;
				size_--;
				if (toIns)
					_insert(node, toIns);
			}
			else
				_erase(node->r, value);
		}
	}
	static T _getSum(std::shared_ptr<Node> node) {
		T res = node->val;
		if (node->l)
			res += _getSum(node->l);
		if (node->r)
			res += _getSum(node->r);
		return res;
	}
	static bool _find(std::shared_ptr<Node> node, T value) {
		if (!node)
			return false;
		if (node->val)
			return true;
		if (value < node->val)
			return _find(node->l, value);
		else
			return _find(node->r, value);
	}

	static void _getSorted(std::shared_ptr<Node> node, std::vector<T>& v) {
		if (node->l)
			_getSorted(node->l, v);
		v.push_back(node->val);
		if (node->r)
			_getSorted(node->r, v);
	}
public:
	void insert(T value) {
		if (!mainNode)
			mainNode = std::make_shared<Node>(value, nullptr, nullptr);
		else
			_insert(mainNode, value);
	}
	void erase(T value) {
		if (!mainNode)
			return;
		if (mainNode->val == value) {
			auto toIns = mainNode->l;
			if (mainNode->r)
				mainNode = mainNode->r;
			else {
				toIns = mainNode->r;
				mainNode = mainNode->l;
			}
			if (toIns != nullptr)
				_insert(mainNode, toIns);
		}
		else
			_erase(mainNode, value);
	}
	T sum() const {
		return _getSum(mainNode);
	}
	bool contains(T value) const {
		return _find(mainNode, value);
	}

	size_t size() const {
		return size_;
	}

	void clear() {
		mainNode = nullptr;
	}

	std::vector<T> getSorted() const {
		std::vector<T> res;
		if (mainNode)
			_getSorted(mainNode, res);
		return res;
	}
};