#include <gtest/gtest.h>
#include "AVL_tree.hpp"
#include <vector>
#include <list>
#include <algorithm>
#include <random>

namespace {
	using T = int;
	constexpr T ksize = 100;
}

TEST(SearchTree, Insert) {
	AVL::AVL_set_t<T> set;
	std::default_random_engine e;
	std::uniform_int_distribution<int> distr{0, ksize};
	std::vector<T> v;
	for (auto i = 0; i < ksize; i++)
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
	for (std::size_t i = 0; i < ksize; i++)
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
	std::vector<T> v{{1, 2, 3}};
	AVL::AVL_set_t<T> set{v.begin(), v.end()};
	set.erase(1);
}

TEST(SearchTree, DeleteRootRightNode) {
	std::vector<T> v{{3, 2, 1}};
	AVL::AVL_set_t<T> set{v.begin(), v.end()};
	set.erase(3);
}

TEST(SearchTree, Erase) {
	std::default_random_engine e;
	std::uniform_int_distribution<T> distr{0, ksize};
	std::list<T> l;
	for (std::size_t i = 0; i < ksize; i++)
		if (distr(e) > ksize / 2)
			l.push_back(distr(e));
		else
			l.push_front(distr(e));

	AVL::AVL_set_t<T> set{l.begin(), l.end()};
	for (auto i : l)
		set.erase(i);
	EXPECT_TRUE(set.empty());
}
