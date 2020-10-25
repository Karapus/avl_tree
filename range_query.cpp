#include <iostream>
#include <queue>
#include <utility>

#ifdef STD
#include <set>
#include <iterator>
#include <cassert>
#else
#include "AVL_set.hpp"
#endif

#ifdef TIME
#define NDEBUG
#include <chrono>
#endif

#ifdef STD
namespace {
	std::size_t range_query(const std::set<int> &set, std::pair<int, int> query) {
		assert(query.first <= query.second);
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
#ifdef TIME
	auto buildup_beg = std::chrono::high_resolution_clock::now();
#endif
	for (auto i = 0LU; i < n; ++i) {
		int tmp = 0;
		std::cin >> tmp;
		set.insert(tmp);
	}
#ifdef TIME
	auto buildup_end = std::chrono::high_resolution_clock::now();
#endif
	std::size_t n_queries;
	std::cin >> n_queries;
	std::queue<std::pair<int, int>> queries;
	for (auto i = 0LU; i < n_queries; ++i) {
		int first;
		int second;
		std::cin >> first >> second;
		queries.emplace(first, second);
	}
	
	std::queue<std::size_t> answers;
#ifdef TIME
	auto queries_beg = std::chrono::high_resolution_clock::now();
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
	auto queries_end = std::chrono::high_resolution_clock::now();
	std::cout << "Build-up time, s:" << std::endl << std::chrono::duration<double>(buildup_end - buildup_beg).count() << std::endl
		<< "Queries time, s:" << std::endl << std::chrono::duration<double>(queries_end - queries_beg).count() << std::endl;
#else	
	while (!answers.empty()) {
		std::cout << answers.front() << ' ';
		answers.pop();
	}
	std::cout << std::endl;
#endif
}
