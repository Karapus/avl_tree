#include <iostream>
#include <set>
#include <list>
#include <utility>

namespace {
	std::size_t range_query(const std::set<int> &set, std::pair<int, int> query) {
		auto res = 0;
		auto first_it = set.upper_bound(query.first);
		auto second_it = set.lower_bound(query.second);
		if (first_it == set.end() || second_it == set.end() || query.first >= query.second)
			return 0;
		while (first_it++ != second_it)
			res++;
		return res;
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
	std::list<std::pair<int, int>> queries;
	for (auto i = 0LU; i < n_queries; i++) {
		int first;
		int second;
		std::cin >> first >> second;
		queries.emplace_back(first, second);
	}

	std::list<std::size_t> answers;
	while (!queries.empty()) {
		answers.emplace_back(range_query(set, queries.front()));
		queries.pop_front();
	}
	
	while (!answers.empty()) {
		std::cout << answers.front() << ' ';
		answers.pop_front();
	}
	std::cout << std::endl;
}
