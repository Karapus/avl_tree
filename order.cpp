#include <iostream>
#include <queue>
#include "AVL_set.hpp"
#ifdef TIME
#include <chrono>
#endif

namespace {
template <typename DatT, typename AnsT, typename QueryT>
[[nodiscard]] std::queue<AnsT> process_queries(QueryT query) {
	std::size_t n_queries;
	std::cin >> n_queries;
	std::queue<DatT> queries;
	for (auto i = 0LU; i < n_queries; ++i) {
		DatT data;
		std::cin >> data;
		queries.emplace(data);
	}
#ifdef TIME
	auto beg = std::chrono::high_resolution_clock::now();
#endif	
	std::queue<AnsT> answers;
	while (!queries.empty()) {
		answers.emplace(query(queries.front()));
		queries.pop();
	}
#ifdef TIME
	auto end = std::chrono::high_resolution_clock::now();
	std::clog << std::endl << std::chrono::duration<double>(end - beg).count() << std::endl;
#endif
	return answers;
}

template <typename T>
std::ostream & operator << (std::ostream & os, std::queue<T> queue) {
	while (!queue.empty()) {
		os << queue.front() << ' ';
		queue.pop();
	}
	return os;
}
}

int main() {
	std::size_t n;
	std::cin >> n;
	AVL::AVL_set_t<int> set;
#ifdef TIME
	auto beg = std::chrono::high_resolution_clock::now();
#endif
	for (auto i = 0LU; i < n; ++i) {
		int tmp = 0;
		std::cin >> tmp;
		set.insert(tmp);
	}
#ifdef TIME
	auto end = std::chrono::high_resolution_clock::now();
	std::clog << std::endl << std::chrono::duration<double>(end - beg).count() << std::endl;
#endif
	std::cout	<< process_queries<std::size_t, int>([&set](auto n){ return set.get_root()->get_nth(n)->get_val(); })		\
			<< process_queries<int, std::size_t>([&set](auto key){ return set.get_root()->order(key); }) << std::endl;
}
