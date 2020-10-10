#include <iostream>
#include <set>
#include <queue>
#include <utility>
#include <iterator>
#include <cassert>

namespace {
	std::size_t range_query(const std::set<int> &set, std::pair<int, int> query) {
		auto first_it = set.lower_bound(query.first);
		auto second_it = set.upper_bound(query.second);
		std::cout << "lb " << *first_it << "ub " << *second_it << std::endl;
		return std::distance(first_it, second_it);
	}
}

int main() {
	std::size_t n;
	std::cin >> n;
	std::set<int> set;
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
	while (!queries.empty()) {
		answers.emplace(range_query(set, queries.front()));
		queries.pop();
	}
	
	while (!answers.empty()) {
		std::cout << answers.front() << ' ';
		answers.pop();
	}
	std::cout << std::endl;
}
