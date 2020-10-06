#pragma once
#include <forward_list>
#include <algorithm>
#include <iostream>
#include <cassert>

namespace AVL
{
template <typename T>
class AVL_tree_t;

template <typename T>
class AVL_set_t {
	AVL_tree_t<T> *root_ = nullptr;
	public:
	AVL_set_t() = default;
	template <typename InputIt>
	AVL_set_t(InputIt first, InputIt last) {
		for (; first != last; first++)
		       insert(*first);
	}
	AVL_set_t(const AVL_set_t &other) = delete;
	AVL_set_t &operator = (const AVL_set_t &other) = delete;
	AVL_set_t(AVL_set_t &&other) :
		AVL_set_t() {
		std::swap(root_, other.root_);
	}
	AVL_set_t &operator = (AVL_set_t &&other) {
		std::swap(root_, other.root_);
	}
	~AVL_set_t() {
		if (root_)
			root_->delete_tree();
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
	const AVL_tree_t<int> *min() const{
		if (root_)
			return root_->min();
		return nullptr;
	}
	const AVL_tree_t<int> *max() const{
		if (root_)
			return root_->max();
		return nullptr;
	}
};

template <typename T>
class AVL_tree_t {
	T val_;
	AVL_tree_t *parent_,
		   *left_ = nullptr,
		   *right_ = nullptr;
	signed char h_dif_ = 0;
	private:

	AVL_tree_t *transplant(AVL_tree_t *node, AVL_tree_t *root);

	AVL_tree_t *balance(AVL_tree_t *root);
	AVL_tree_t *rotateLeft(AVL_tree_t *root);
	AVL_tree_t *rotateRight(AVL_tree_t *root);
	AVL_tree_t *bigRotateLeft(AVL_tree_t *root) {
		return rotateLeft(right_->rotateRight(root));
	}
	AVL_tree_t *bigRotateRight(AVL_tree_t *root) {
		return rotateRight(left_->rotateLeft(root));
	}
	
	AVL_tree_t(const T &elem, AVL_tree_t *parent = nullptr) :
		val_(elem), parent_(parent)
	{}
	~AVL_tree_t() = default;

	public:
	AVL_tree_t(const AVL_tree_t &other) = delete;
	AVL_tree_t &operator = (const AVL_tree_t &other) = delete;
	AVL_tree_t(AVL_tree_t &&other) = delete;
	AVL_tree_t &operator = (AVL_tree_t &&other) = delete; 

	AVL_tree_t *search(const T &elem);
	const AVL_tree_t *search(const T &elem) const;

	AVL_tree_t *insert(const T &elem, AVL_tree_t *root);

	T get_val() const {
		return val_;
	}
	int get_h_dif() const {
		return h_dif_;
	}
	const AVL_tree_t *get_left() const {
		return left_;
	}
	const AVL_tree_t *get_right() const {
		return right_;
	}
	const AVL_tree_t *get_parent() const {
		return parent_;
	}
	AVL_tree_t *min() {
		if (left_)
			return left_->min();
		return this;
	}
	AVL_tree_t *max() {
		if (right_)
			return right_->max();
		return this;
	}
	AVL_tree_t *next();
	const AVL_tree_t *next() const;
	
	AVL_tree_t *prev();
	const AVL_tree_t *prev() const;
	
	void delete_tree();
	AVL_tree_t *delete_node(AVL_tree_t *root);
};

template <typename T>
AVL_tree_t<T> *AVL_tree_t<T>::insert(const T &elem, AVL_tree_t *root) {
	if (!root)
		return new AVL_tree_t(elem);
	if (elem < val_) {
		if (left_)
			return left_->insert(elem, root);
		left_ = new AVL_tree_t(elem, this);
		h_dif_++;
	}
	else if (right_)
		return right_->insert(elem, root);
	else {
		right_ = new AVL_tree_t(elem, this);
		h_dif_--;
	}
	auto node = this;
	while (node->parent_) {
		auto prev = node;
		node = node->parent_;
		if (node->left_ == prev)
			node->h_dif_++;
		else
			node->h_dif_--;
	//	if (!node->h_dif_)
	//		return root;
		root = node->balance(root);
		if (!node->h_dif_)
			return root;
	}
	return root;
}

template <typename T>
AVL_tree_t<T> *AVL_tree_t<T>::search(const T &elem) {
	if (elem < val_) {
		if (left_)
			return left_->search(elem);
		else
			return nullptr;
	}
	if (elem == val_)
		return this;
	if (right_)
		return right_->search(elem);
	return nullptr;
}

template <typename T>
const AVL_tree_t<T> *AVL_tree_t<T>::search(const T &elem) const {
	if (elem < val_) {
		if (left_)
			return left_->search(elem);
		else
			return nullptr;
	}
	if (elem == val_)
		return this;
	if (right_)
		return right_->search(elem);
	return nullptr;
}

template <typename T>
AVL_tree_t<T> *AVL_tree_t<T>::delete_node(AVL_tree_t *root) {
	if (right_) {
		auto n_node = next();
		val_ = n_node->val_;
		return n_node->delete_node(root);
	}
	if (left_) {
		auto p_node = prev();
		val_ = p_node->val_;
		return p_node->delete_node(root);
	}

	auto node = this;
	while (node->parent_) {
		auto prev = node;
		node = node->parent_;
		if (node->left_ == prev)
			node->h_dif_--;
		else
			node->h_dif_++;
		if (node->h_dif_ == 1 || node->h_dif_ == -1)
			break;
		root = node->balance(root);
		if (!node->h_dif_)
			break;
	}

	if (!parent_)
		root = nullptr;
	else if (parent_->left_ == this)
		parent_->left_ = nullptr;
	else 
		parent_->right_ = nullptr;
	delete this;

	return root;
}

template <typename T>
AVL_tree_t<T> *AVL_tree_t<T>::transplant(AVL_tree_t *node, AVL_tree_t *root) {
	if (!parent_)
		root = node;
	else if (this == parent_->right_)
		parent_->right_ = node;
	else
		parent_->left_ = node;
	if (node)
		node->parent_ = parent_;
	return root;
}

template <typename T>
AVL_tree_t<T> *AVL_tree_t<T>::next() {
	if (right_)
		return right_->min();
	auto node = this;
	auto parent = parent_;
	while (parent && (node == parent->right_)) {
		node = parent;
		parent = parent->parent_;
	}
	return parent;
}

template <typename T>
const AVL_tree_t<T> *AVL_tree_t<T>::next() const{
	if (right_)
		return right_->min();
	auto node = this;
	auto parent = parent_;
	while (parent && (node == parent->right_)) {
		node = parent;
		parent = parent->parent_;
	}
	return parent;
}

template <typename T>
AVL_tree_t<T> *AVL_tree_t<T>::prev() {
	if (left_)
		return left_->max();
	auto node = this;
	auto parent = parent_;
	while (parent && node == parent->left_) {
		node = parent;
		parent = parent->parent_;
	}
	return parent;
}

template <typename T>
const AVL_tree_t<T> *AVL_tree_t<T>::prev() const{
	if (left_)
		return left_->max();
	auto node = this;
	auto parent = parent_;
	while (parent && node == parent->left_) {
		node = parent;
		parent = parent->parent_;
	}
	return parent;
}

template <typename T>
void AVL_tree_t<T>::delete_tree() {
	if (!parent_) {
		if (left_)
			left_->delete_tree();
		if (right_)
			right_->delete_tree();
		delete this;
		return;
	}

	std::forward_list<AVL_tree_t *> stack{this};
	auto node = this;
	do {
		auto left = node->left_;
		auto right = node->right_;
		if (!left && !right) {
			if (node->parent_->left_ == node)
				node->parent_->left_ = nullptr;
			else
				node->parent_->right_ = nullptr;
			delete node;
			node = stack.front();
			stack.pop_front();
		}
		else {
			stack.push_front(node);
			if (left)
				node = left;
			else
				node = right;
		}
	} while (!stack.empty());
}

template <typename T>
AVL_tree_t<T> *AVL_tree_t<T>::balance(AVL_tree_t<T> *root) {
	if (h_dif_ == -2) {
		if (right_->h_dif_ <= 0) {
			if (right_->h_dif_)
				h_dif_ = right_->h_dif_ = 0;
			else {
				h_dif_ = -1;
				right_->h_dif_ = 1;
			}
			return rotateLeft(root);
		}
		else {
			switch (right_->left_->h_dif_) {
				case -1:
					h_dif_ = 1;
					right_->h_dif_ = 0;
					break;
				case 0:
					h_dif_ = right_->h_dif_ = 0;
					break;
				case 1:
					h_dif_ = 0;
					right_->h_dif_ = -1;
					break;
			}
			right_->left_->h_dif_ = 0;
			return bigRotateLeft(root);
		}
	}
	else if (h_dif_ == 2) {
		if (left_->h_dif_ >= 0) {
			if (left_->h_dif_)
				h_dif_ = left_->h_dif_ = 0;
			else {
				h_dif_ = 1;
				left_->h_dif_ = -1;
			}
			return rotateRight(root);
		}
		else {
			switch (left_->right_->h_dif_) {
				case -1:
					h_dif_ = -1;
					left_->h_dif_ = 0;
					break;
				case 0:
					h_dif_ = left_->h_dif_ = 0;
					break;
				case 1:
					h_dif_ = 0;
					left_->h_dif_ = 1;
					break;
			}
			left_->right_->h_dif_ = 0;
			return bigRotateRight(root);
		}
	}
	return root;
}

template <typename T>
AVL_tree_t<T> *AVL_tree_t<T>::rotateLeft(AVL_tree_t<T> *root) {
	AVL_tree_t *going_up = right_;
	AVL_tree_t *trfd_subtree = going_up->left_;
	
	if (trfd_subtree)
		trfd_subtree->parent_ = this;
	right_ = trfd_subtree;
	
	AVL_tree_t *&ptr_to_this = (parent_) 	? ((parent_->left_ == this)	? parent_->left_ 
										: parent_->right_)
						: root;
	ptr_to_this = going_up;
	going_up->parent_ = parent_;
	
	parent_ = going_up;
	going_up->left_ = this;

	return root;
}

template <typename T>
AVL_tree_t<T> *AVL_tree_t<T>::rotateRight(AVL_tree_t<T> *root) {
	AVL_tree_t *going_up = left_;
	AVL_tree_t *trfd_subtree = going_up->right_;
	
	if (trfd_subtree)
		trfd_subtree->parent_ = this;
	left_ = trfd_subtree;

	AVL_tree_t *&ptr_to_this = (parent_) 	? ((parent_->left_ == this)	? parent_->left_ 
										: parent_->right_)
						: root;
	ptr_to_this = going_up;
	going_up->parent_ = parent_;
	
	parent_ = going_up;
	going_up->right_ = this;

	return root;
}
} //namespace AVL
