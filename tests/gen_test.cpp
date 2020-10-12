#include <iostream>
#include <fstream>
#include <random>
#include <ctime>

void gen_test(int nkeys, int keymax, int nqrs, std::ofstream &ftest, std::ofstream &fanws) {
	int spacing = keymax / nkeys;
	spacing = (spacing) ? spacing : 1;
	ftest << nkeys << ' ';
	for (int i = 0; i < nkeys; i++)
		ftest << i * spacing << ' ';
	std::default_random_engine gen{static_cast<unsigned>(time(0))};
	auto deviation = keymax / 4;
        std::uniform_int_distribution<int> distr{-deviation, keymax + deviation};
	ftest << '\n' << nqrs << ' ';
	for (int i = 0; i < nqrs; i++) {
		auto frst = distr(gen);
		auto scnd = distr(gen);
		if (frst > scnd)
			std::swap(frst, scnd);
		ftest << frst << ' ' << scnd << ' ';
		auto max = (nkeys - 1) * spacing;
		if ((frst < 0 && scnd < 0) || (frst > max && scnd > max))
			fanws << "0 ";
		else {
			frst = (frst < 0) ? 0 : frst;
			scnd = (scnd < 0) ? 0 : scnd;
			frst = (frst > max) ? max : frst;
			scnd = (scnd > max) ? max : scnd;
			frst = (frst % spacing) ? frst + spacing - frst % spacing : frst;
			scnd = (scnd % spacing) ? scnd - scnd % spacing : scnd;
			fanws << (scnd - frst) / spacing + 1 << ' ';
		}
	}
	fanws << std::endl;
}

int main() {
	int nkeys, keymax, nqrs;
	std::cout << "Input nkeys, keymax and nqueryes" << std::endl;
	std::cin >> nkeys >> keymax >> nqrs;
	std::cout << "Input filename" << std::endl;
	std::string fname;
	std::cin >> fname;
	std::ofstream ftest, fanws;
	ftest.open(fname + ".dat");
	fanws.open(fname + ".ans");

	gen_test(nkeys, keymax, nqrs, ftest, fanws);

	ftest.close();
	fanws.close();		
}
