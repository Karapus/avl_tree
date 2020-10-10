#include <iostream>
#include <queue>
#include <utility>
#include <iterator>
#include <cassert>
#include "AVL_set.hpp"

#ifdef TIME
#include <chrono>
#endif


int main() {
	std::size_t n;
	std::cin >> n;
	AVL::AVL_set_t<int> set;
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
		answers.emplace(set.range_query(queries.front()));
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
