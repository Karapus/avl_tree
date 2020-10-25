#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <algorithm>
#include <set>

namespace {
constexpr int dev_factor = 4;
std::default_random_engine gen{static_cast<unsigned>(time(0))};

std::vector<int> gen_data(std::size_t nkeys, int keymax, std::ofstream &of) {
	of << nkeys << std::endl;
        std::uniform_int_distribution<int> distr{0, keymax};
	std::set<int> keys;
	while (keys.size() < nkeys) {
		auto key = distr(gen);
		if (keys.emplace(key).second) {
			of << key << ' ';
		}
	}
	of << std::endl;
	std::vector<int> data(keys.begin(), keys.end());
	return data;
}

template <typename T, typename F>
void gen_answers(std::size_t n, T min, T max, std::ofstream &fdat, std::ofstream &fans, F gen_ans) {
	fdat << n << std::endl;
	std::uniform_int_distribution<T> distr{min, max};
	for (auto i = 0u; i < n; ++i) {
		auto query = distr(gen);
		fdat << query << ' ';
		fans << gen_ans(query) << ' ';
	}
	fdat << std::endl;
	fans << std::endl;
}
}

int main() {
	std::size_t nkeys, nq_getnth, nq_order;
	int keymax;
	std::cout << "Input number of keys, number of queries for nth key, number of queries for order of key and keymax" << std::endl;
	std::cin >> nkeys >> nq_getnth >> nq_order >> keymax;
	std::cout << "Input filename prefix" << std::endl;
	std::string fname;
	std::cin >> fname;
	std::ofstream fdat, fans;
	fdat.open(fname + ".dat");
	fans.open(fname + ".ans");

	std::vector<int> data = gen_data(nkeys, keymax, fdat);
	gen_answers(nq_getnth, 1lu, data.size(), fdat, fans, [&data](std::size_t n){ return data[n - 1]; });
	gen_answers(nq_order, -keymax/dev_factor, keymax* (1 + dev_factor) / dev_factor, fdat, fans, 
			[&data](int key){ return lower_bound(data.begin(), data.end(), key) - data.begin(); });
	
	fdat.close();
	fans.close();		
}
