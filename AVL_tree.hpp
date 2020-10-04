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
	AVL_tree_t<T> *root_;
	public:
	AVL_set_t() : root_(nullptr)
	{}
	template <typename InputIt>
	AVL_set_t(InputIt first, InputIt last) {
		if (first == last)
		{
			root_ = nullptr;
			return;
		}
		root_ = new AVL_tree_t<T>(*first++);
		for (; first < last; first++)
		       root_->insert(*first);
	}
	AVL_set_t(const AVL_set_t &other) :
		AVL_set_t(other.begin(), other.end())
	{}
	AVL_set_t(AVL_set_t &&other) :
		AVL_set_t()
	{
		std::swap(root_, other.root_);
	}
	~AVL_set_t() {
		if (root_)
			root_->delete_tree();
	}
	void insert(const T &elem) {
		if (!root_)
			root_ = new AVL_tree_t<T>(elem);
		return root_->insert(elem);
	}
	const AVL_tree_t<int> *min() {
		return root_->min();
	}
	const AVL_tree_t<int> *max() {
		return root_->max();
	}
};

template <typename T>
class AVL_tree_t {
	const T val_;
	AVL_tree_t *parent_ = nullptr,
		   *left_ = nullptr,
		   *right_ = nullptr;
	//bool h_;
	void rotate_left();
	void rotate_right();

	public:
	AVL_tree_t(const T &elem) :
		val_(elem)
	{}
	void insert(const T &elem) {
		if (elem < val_) {
			if (left_)
				return left_->insert(elem);
			left_ = new AVL_tree_t(elem);
			left_->parent_ = this;
			return;
		}
		if (right_)
			return right_->insert(elem);
		right_ = new AVL_tree_t(elem);
		right_->parent_ = this;
		return;
	}
	T get_val() const {
		return val_;
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
	const AVL_tree_t *min() const {
		if (left_)
			return left_->min();
		return this;
	}
	const AVL_tree_t *max() const {
		if (right_)
			return right_->max();
		return this;
	}
	const AVL_tree_t *next() const {
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
	const AVL_tree_t *prev() const {
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
	void null_node(const AVL_tree_t *node)
	{
		assert(node == right_ || node == left_);
		if (node == right_)
			right_ = nullptr;
		else
			left_ = nullptr;
	}
	void delete_tree() {
		if (!parent_)
		{
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
			if (!left && !right)
			{
				node->parent_->null_node(node);
				delete node;
				node = stack.front();
				stack.pop_front();
			}
			else
			{
				stack.push_front(node);
				if (left)
					node = left;
				else if (right)
					node = right;
			}
		} while (!stack.empty());
	}
};

template <typename T>
void AVL_tree_t<T>::rotate_left() {
	assert(right_);
	AVL_tree_t *&ptr_to_this = (parent_->left_ == this()) ? parent_->left_ : parent_->right_;
	AVL_tree_t *new_root = right_;
	AVL_tree_t *trfd_subtree = new_root->left_;
	ptr_to_this = new_root;
	new_root->parent_ = parent_;
	parent_ = new_root;
	trfd_subtree->parent = this;
	right_ = trfd_subtree;
	new_root->left_ = this;
}
}
