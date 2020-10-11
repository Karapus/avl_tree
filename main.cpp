#include <iostream>
#include <queue>
#include <utility>
#include <iterator>
#include <cassert>

#ifdef STD
#include <set>
#else
#include "AVL_set.hpp"
#endif

#ifdef TIME
#include <chrono>
#endif

#ifdef STD
namespace {
	std::size_t range_query(const std::set<int> &set, std::pair<int, int> query) {
		auto first_it = set.lower_bound(query.first);
		auto second_it = set.upper_bound(query.second);
		return std::distance(first_it, second_it);
	}
}
#endif

int main() {
	std::size_t n;
	std::cin >> n;
#ifdef STD
		std::set<int> set;
#else
		AVL::AVL_set_t<int> set;
#endif

	for (auto i = 0LU; i < n; i++) {
		int tmp = 0;
		std::cin >> tmp;
		set.insert(tmp);
	}
	std::size_t n_queries;
	std::cin >> n_queries;
	std::queue<std::pair<int, int>> queries;
	for (auto i = 0LU; i < n_queries; i++) {
		int first;
		int second;
		std::cin >> first >> second;
		queries.emplace(first, second);
	}
	
	std::queue<std::size_t> answers;
#ifdef TIME
	auto beg = std::chrono::high_resolution_clock::now();
#endif
	while (!queries.empty()) {
#ifdef STD
		answers.emplace(range_query(set, queries.front()));
#else
		answers.emplace(set.range_query(queries.front()));
#endif
		queries.pop();
	}
#ifdef TIME
	auto end = std::chrono::high_resolution_clock::now();
#endif
	
	
	while (!answers.empty()) {
		std::cout << answers.front() << ' ';
		answers.pop();
	}
	std::cout << std::endl;
#ifdef TIME
	std::cout << std::chrono::duration<double>(end - beg).count() << std::endl;
#endif
}
