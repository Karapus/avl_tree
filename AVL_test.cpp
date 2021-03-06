#include <gtest/gtest.h>
#include "AVL_set.hpp"
#include <vector>
#include <list>
#include <algorithm>
#include <random>

namespace {
	using T = int;
	constexpr T ksize = 100;

TEST(SearchTree, Insert) {
	AVL::AVL_set_t<T> set;
	std::default_random_engine e;
	std::uniform_int_distribution<int> distr{0, ksize};
	std::vector<T> v;
	for (auto i = 0; i < ksize; ++i)
	{
		auto val = distr(e);
		v.push_back(val);
		set.insert(val);
	}
	std::sort(v.begin(), v.end());
	auto el = set.min();
	for (auto i : v)
	{
		EXPECT_EQ(el->get_val(), i);
		el = el->next();
	}
}

TEST(SearchTree, CtorFromRange) {
	std::default_random_engine e;
	std::uniform_int_distribution<T> distr{0, ksize};
	std::vector<T> v;
	for (std::size_t i = 0; i < ksize; ++i)
		v.push_back(distr(e));
	AVL::AVL_set_t<T> set{v.begin(), v.end()};
	std::sort(v.begin(), v.end());
	auto el = set.min();
	for (auto i : v)
	{
		EXPECT_EQ(el->get_val(), i);
		el = el->next();
	}
}

TEST(SearchTree, DeleteRootNoChildren) {
	AVL::AVL_set_t<T> set;
	set.insert(1);
	set.erase(1);
}

TEST(SearchTree, DeleteRootLeftNode) {
	std::vector<T> v{{1, 2}};
	AVL::AVL_set_t<T> set{v.begin(), v.end()};
	set.erase(1);
}

TEST(SearchTree, DeleteRootRightNode) {
	std::vector<T> v{{3, 2}};
	AVL::AVL_set_t<T> set{v.begin(), v.end()};
	set.erase(3);
}

TEST(SearchTree, Erase) {
	std::default_random_engine e;
	std::uniform_int_distribution<T> distr{0, ksize};
	std::list<T> l;
	for (std::size_t i = 0; i < ksize; ++i)
		if (distr(e) > ksize / 2)
			l.push_back(distr(e));
		else
			l.push_front(distr(e));

	AVL::AVL_set_t<T> set{l.begin(), l.end()};
	for (auto i : l) {
		set.erase(i);
		auto set_el = set.min();
		auto next = (set_el) ? set_el->next() : nullptr;
		while (next) {
			EXPECT_LE(set_el->get_val(), next->get_val());
			set_el = next;
			next = set_el->next();
		}
	}
	EXPECT_TRUE(set.empty());
}

TEST(AVLTree, RotateRight) {
	std::vector<T> v{{3, 4, 2, 1, 0}};
	AVL::AVL_set_t<T> set{v.begin(), v.end()};
	auto el = set.min();
	for (auto i = 0; i < 5; ++i) {
		EXPECT_TRUE(std::abs(el->get_h_dif()) < 2);
		el = el->next();
	}
}

TEST(AVLTree, RotateLeft) {
	std::vector<T> v{{1, 0, 2, 3, 4}};
	AVL::AVL_set_t<T> set{v.begin(), v.end()};
	auto el = set.min();
	for (auto i = 0; i < 5; ++i) {
		EXPECT_TRUE(std::abs(el->get_h_dif()) < 2);
		el = el->next();
	}
}

TEST(AVLTree, Insert) {
	AVL::AVL_set_t<T> set;
	std::default_random_engine e;
	std::uniform_int_distribution<int> distr{0, ksize};
	for (auto i = 0; i < ksize; ++i)
		set.insert(distr(e));
	auto el = set.min();
	for (auto i = 0; i < ksize; ++i) {
		EXPECT_TRUE(std::abs(el->get_h_dif()) < 2);
		el = el->next();
	}
}

TEST(AVLTree, Erase) {
	std::default_random_engine e;
	std::uniform_int_distribution<T> distr{0, ksize};
	std::list<T> l;
	for (std::size_t i = 0; i < ksize; ++i)
		l.push_front(distr(e));

	AVL::AVL_set_t<T> set{l.begin(), l.end()};
	for (auto i : l) {
		set.erase(i);
		auto set_el = set.min();
		while (set_el) {
			EXPECT_TRUE(std::abs(set_el->get_h_dif()) < 2);
			set_el = set_el->next();
		}
	}
	EXPECT_TRUE(set.empty());
}

TEST(RangeQuery, LowerBound) {
	std::vector<T> v{{1, 2, 3, 5, 6}};
	AVL::AVL_set_t<T> set{v.begin(), v.end()};
	auto root = set.get_root();
	EXPECT_EQ(root->lower_bound(0)->get_val(), 1);
	EXPECT_EQ(root->lower_bound(4)->get_val(), 5);
	EXPECT_EQ(root->lower_bound(5)->get_val(), 5);
	EXPECT_EQ(root->lower_bound(7), nullptr);
}

TEST(RangeQuery, UpperBound) {
	std::vector<T> v{{1, 2, 3, 5, 6}};
	AVL::AVL_set_t<T> set{v.begin(), v.end()};
	auto root = set.get_root();
	EXPECT_EQ(root->upper_bound(5)->get_val(), 6);
	EXPECT_EQ(root->upper_bound(4)->get_val(), 5);
	EXPECT_EQ(root->upper_bound(0)->get_val(), 1);
	EXPECT_EQ(root->upper_bound(6), nullptr);
}

TEST(RangeQuery, RangeQuery) {
	std::vector<T> v{{1, 2, 3, 5, 6}};
	AVL::AVL_set_t<T> set{v.begin(), v.end()};
	EXPECT_EQ(set.range_query({1, 6}), 5);
	EXPECT_EQ(set.range_query({0, 7}), 5);
	EXPECT_EQ(set.range_query({1, 4}), 3);
	EXPECT_EQ(set.range_query({4, 6}), 2);
	EXPECT_EQ(set.range_query({5, 5}), 1);
	EXPECT_EQ(set.range_query({4, 4}), 0);
	EXPECT_EQ(set.range_query({0, 0}), 0);
}

TEST(OrderStat, SizeInsert) {
	std::default_random_engine e;
	std::uniform_int_distribution<T> distr{0, ksize};
	std::list<T> l;
	for (std::size_t i = 0; i < ksize; ++i)
		l.push_front(distr(e));

	AVL::AVL_set_t<T> set{l.begin(), l.end()};
	auto el = set.min();
	while (el) {
		auto left = el->get_left();
		auto right = el->get_right();
		EXPECT_EQ(el->get_size(), (left ? left->get_size() : 0) + (right ? right->get_size() : 0) + 1);
		el = el->next();
	}
}

TEST(OrderStat, SizeErase) {
	std::default_random_engine e;
	std::uniform_int_distribution<T> distr{0, ksize};
	std::list<T> l;
	for (std::size_t i = 0; i < ksize; ++i)
		l.push_front(distr(e));

	AVL::AVL_set_t<T> set{l.begin(), l.end()};
	for (auto i : l) {
		set.erase(i);
		auto el = set.min();
		while (el) {
			auto left = el->get_left();
			auto right = el->get_right();
			EXPECT_EQ(el->get_size(), (left ? left->get_size() : 0) + (right ? right->get_size() : 0) + 1);
			el = el->next();
		}
	}
}

TEST(OrderStat, GetNth) {
	std::vector<T> v{{0, 1, 2, 3, 4, 5}};
	AVL::AVL_set_t<T> set{v.begin(), v.end()};
	auto sz = v.size();
	for (auto i = 0u; i < sz; ++i)
		EXPECT_EQ(set.get_root()->get_nth(i + 1), set.get_root()->search(v[i]));
}

TEST(OrderStat, Order) {
	std::vector<T> v{{1, 3, 5, 7, 9, 11}};
	AVL::AVL_set_t<T> set{v.begin(), v.end()};
	for (auto i = 0u; i < 12; i++)
		 EXPECT_EQ(set.get_root()->order(i), i / 2);
	EXPECT_EQ(set.get_root()->order(12), v.size());
}
}
