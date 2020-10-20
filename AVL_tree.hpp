#pragma once
#include <cassert>

namespace AVL
{
template <typename T>
class AVL_tree_t {
	T val_;
	AVL_tree_t *parent_,
		   *left_ = nullptr,
		   *right_ = nullptr;

	signed char h_dif_ = 0;
	AVL_tree_t *balance(AVL_tree_t *root);
	AVL_tree_t *rotateLeft(AVL_tree_t *root);
	AVL_tree_t *rotateRight(AVL_tree_t *root);
	AVL_tree_t *bigRotateLeft(AVL_tree_t *root) {
		return rotateLeft(right_->rotateRight(root));
	}
	AVL_tree_t *bigRotateRight(AVL_tree_t *root) {
		return rotateRight(left_->rotateLeft(root));
	}
	
	std::size_t size_ = 1;
	void update_size() {
		size_ = 1 + (left_ ? left_->size_ : 0) + (right_ ? right_->size_ : 0);
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

	const AVL_tree_t *search(const T &elem) const;
	AVL_tree_t *search(const T &elem) {
		return const_cast<AVL_tree_t *>(const_cast<const AVL_tree_t *>(this)->search(elem));
	}
	const AVL_tree_t *lower_bound(const T &elem) const;
	const AVL_tree_t *upper_bound(const T &elem) const;

	AVL_tree_t *insert(const T &elem, AVL_tree_t *root);

	T get_val() const {
		return val_;
	}
	int get_h_dif() const {
		return h_dif_;
	}
	std::size_t get_size() const {
		return size_;
	}
	AVL_tree_t *get_left(){
		return left_;
	}
	const AVL_tree_t *get_left() const {
		return left_;
	}
	AVL_tree_t *get_right(){
		return right_;
	}
	const AVL_tree_t *get_right() const {
		return right_;
	}
	const AVL_tree_t *get_parent() const {
		return parent_;
	}
	AVL_tree_t *min() {
		assert(this);
		auto node = this;
		while (node->left_)
			node = node->left_;
		return node;
	}
	AVL_tree_t *max() {
		assert(this);
		auto node = this;
		while (node->right_)
			node = node->right_;
		return node;
	}
	const AVL_tree_t *next() const;
	AVL_tree_t *next() {
		return const_cast<AVL_tree_t *>(const_cast<const AVL_tree_t *>(this)->next());
	}
	const AVL_tree_t *prev() const;
	AVL_tree_t *prev() {
		return const_cast<AVL_tree_t *>(const_cast<const AVL_tree_t *>(this)->prev());
	}

	AVL_tree_t *delete_node(AVL_tree_t *root);
	AVL_tree_t *delete_leaf(AVL_tree_t *root);
};

template <typename T>
AVL_tree_t<T> *AVL_tree_t<T>::insert(const T &elem, AVL_tree_t *root) {
	if (!root)
		return new AVL_tree_t(elem);
	auto node = root;
	while (true) {
		node->size_++;
		if (elem < node->val_) {
			if (!node->left_) {
				node->left_ = new AVL_tree_t(elem, node);
				node->h_dif_++;
				break;
			}
			node = node->left_;
		}
		else if (!node->right_) {
			node->right_ = new AVL_tree_t(elem, node);
			node->h_dif_--;
			break;
		}
		else
			node = node->right_;	
	}

	while (node->parent_) {
		auto prev = node;
		node = node->parent_;
		if (node->left_ == prev)
			node->h_dif_++;
		else
			node->h_dif_--;
		root = node->balance(root);
		if (!node->h_dif_)
			return root;
	}
	return root;
}

template <typename T>
const AVL_tree_t<T> *AVL_tree_t<T>::search(const T &elem) const {
	auto node = this;
	while (node) {
		if (elem < node->val_)
			node = node->left_;
		else if (elem > node->val_)
			node = node->right_;
		else
			return node;
	}
	return nullptr;
}

template <typename T>
const AVL_tree_t<T> *AVL_tree_t<T>::lower_bound(const T &elem) const {
	const AVL_tree_t *prev = nullptr;
	auto node = this;
	while (node) {
		if (elem < node->val_) {
			prev = node;
			node = node->left_;
		}
		else if (elem > node->val_)
			node = node->right_;
		else
			return node;
	}
	return prev;
}

template <typename T>
const AVL_tree_t<T> *AVL_tree_t<T>::upper_bound(const T &elem) const {
	const AVL_tree_t *prev = nullptr;
	auto node = this;
	while (node) {
		if (elem < node->val_) {
			prev = node;
			node = node->left_;
		}
		else
			node = node->right_;
	}
	return prev;
}

template <typename T>
AVL_tree_t<T> *AVL_tree_t<T>::delete_node(AVL_tree_t *root) {
	auto node = this;
	while (node->right_ || node->left_) {
		auto next = node->right_ ? node->right_->min() : node->left_->max();
		node->val_ = next->val_;
		node = next;
	}
	
	auto del_node = node;
	del_node->size_ = 0;
	while (node->parent_) {
		node = node->parent_;
		node->size_--;
	}
	node = del_node;
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
	return del_node->delete_leaf(root);
}

template <typename T>
AVL_tree_t<T> *AVL_tree_t<T>::delete_leaf(AVL_tree_t *root) {
	assert(!left_ && !right_);
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
				case 1:
					h_dif_ = -1;
					left_->h_dif_ = 0;
					break;
				case 0:
					h_dif_ = left_->h_dif_ = 0;
					break;
				case -1:
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
	going_up->size_ = size_;
	
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

	update_size();

	return root;
}

template <typename T>
AVL_tree_t<T> *AVL_tree_t<T>::rotateRight(AVL_tree_t<T> *root) {
	AVL_tree_t *going_up = left_;
	AVL_tree_t *trfd_subtree = going_up->right_;
	going_up->size_ = size_;
	
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

	update_size();

	return root;
}
} //namespace AVL
