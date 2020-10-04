#include <gtest/gtest.h>
#include "AVL_tree.hpp"
#include <vector>
#include <algorithm>
#include <random>

namespace {
	constexpr std::size_t ksize = 100;
}

TEST(SearchTree, CtorFromRange) {
	std::default_random_engine e;
	std::uniform_int_distribution<int> distr{0, ksize};
	std::vector<int> v;
	for (std::size_t i = 0; i < ksize; i++)
		v.push_back(distr(e));
	AVL::AVL_set_t<int> set{v.begin(), v.end()};
	std::sort(v.begin(), v.end());
	auto el = set.min();
	for (auto i : v)
	{
		EXPECT_EQ(el->get_val(), i);
		el = el->next();
	}
}


