#pragma once
#include "AVL_tree.hpp"
#include <queue>
#include <stack>
#include <utility>
#include <cassert>

namespace AVL
{
template <typename T>
class AVL_set_t {
	AVL_tree_t<T> *root_ = nullptr;
	void copy_tree(const AVL_set_t &other);
	void delete_tree();
	
	public:
	AVL_set_t() = default;
	template <typename InputIt>
	AVL_set_t(InputIt first, InputIt last) {
		for (; first != last; first++)
		       insert(*first);
	}
	AVL_set_t(const AVL_set_t &other) : AVL_set_t(){
		copy_tree(other);	
	}
	AVL_set_t &operator = (const AVL_set_t &rhs) {
		if (rhs.root_ != root_) {
			delete_tree();
			copy_tree(rhs);
		}
		return *this;
	}
	AVL_set_t(AVL_set_t &&other) : AVL_set_t() {
		std::swap(root_, other.root_);
	}
	AVL_set_t &operator = (AVL_set_t &&other) {
		std::swap(root_, other.root_);
	}
	~AVL_set_t() {
		delete_tree();
	}
	const AVL_tree_t<T> *get_root() const {
		return root_;
	}
	void insert(const T &elem) {
		root_ = root_->insert(elem, root_);
	}
	void erase(const T &elem) {
		if (!root_)
			return;
		auto node = root_->search(elem);
		root_ = node->delete_node(root_);
	}
	bool empty() {
		return !root_;
	}
	const AVL_tree_t<T> *min() const {
		if (root_)
			return root_->min();
		return nullptr;
	}
	const AVL_tree_t<T> *max() const {
		if (root_)
			return root_->max();
		return nullptr;
	}
	std::size_t range_query(const std::pair<T, T> &query);
};

template <typename T>
void AVL_set_t<T>::copy_tree(const AVL_set_t &other) {
	if (!other.root_)
		return;
	std::queue<const AVL_tree_t<T> *> nodes{other.root_};
	while (!nodes.empty()) {
		auto node = nodes.front();
		nodes.pop();
		root_ = root_->insert(node->get_val());
		if (node->left)
			nodes.push(node->left);
		if (node->right)
			nodes.push(node->right);
	}
}

template <typename T>
void AVL_set_t<T>::delete_tree() {
	if (!root_)
		return;
	std::stack<AVL_tree_t<T> *> nodes;
	auto node = root_;
	do {
		auto left = node->get_left();
		auto right = node->get_right();
		if (!left && !right) {
			root_ = node->delete_leaf(root_);
			node = nodes.top();
			nodes.pop();
		}
		else {
			nodes.push(node);
			node = left ? left : right;
		}
	} while (!nodes.empty());
}

template <typename T>
std::size_t AVL_set_t<T>::range_query(const std::pair<T, T> &query) {
	assert(query.first <= query.second);
	auto lb = root_->lower_bound(query.first);
	auto ub = root_->upper_bound(query.second);
	std::size_t dist = 0;
	while (lb != ub) {
		lb = lb->next();
		dist++;
	}
	return dist;	
}	
} //namespace AVL
