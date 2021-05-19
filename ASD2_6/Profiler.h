#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <cmath>
#include <random>
#include <typeinfo>

template<class T>
class Profiler {
private:
	T cont;
	std::function<void(T*, int)> insert, erase;
	std::function<bool(T*, int)> find;
	std::function<std::vector<int>(T*)> getSorted;
	std::vector<int> randArr;
	std::mt19937 gen;

public:
	template<class... Args>
	Profiler(Args&& ...args) : cont(args...), randArr(1e7) {
		for (int& x : randArr)
			x = gen();
	}

	Profiler& setInsert(std::function<void(T*, int)> func) {
		insert = func;
		return *this;
	}

	Profiler& setErase(std::function<void(T*, int)> func) {
		erase = func;
		return *this;
	}

	Profiler& setFind(std::function<bool(T*, int)> func) {
		find = func;
		return *this;
	}

	Profiler& setSort(std::function<std::vector<int>(T*)> func) {
		getSorted = func;
		return *this;
	}

	std::unordered_map<std::string, std::vector<size_t>> run(std::ostream& logs = std::clog) {
		std::unordered_map<std::string, std::vector<size_t>> results;
		using namespace std;
		using namespace std::chrono;
		using namespace std::placeholders;
		results["insert"].reserve(41);
		results["erase"].reserve(41);
		results["find"].reserve(41);
		results["getSorted"].reserve(41);
		for (int i = 0; i < randArr.size(); i++) {
			insert(&cont, randArr[i]);
			if (isProfile(i + 1)) {
				auto start = steady_clock::now();
				insert(&cont, randArr[i + 1]);
				results["insert"].push_back(duration_cast<microseconds>(steady_clock::now() - start).count());
				start = steady_clock::now();
				volatile auto tem1 = find(&cont, randArr[(i + 1) / 2]);
				results["find"].push_back(duration_cast<microseconds>(steady_clock::now() - start).count());
				start = steady_clock::now();
				erase(&cont, randArr[(i + 1) / 2]);
				results["erase"].push_back(duration_cast<microseconds>(steady_clock::now() - start).count());
				if (i > 40'000'000)
					break;
				start = steady_clock::now();
				getSorted(&cont);
				results["getSorted"].push_back(duration_cast<microseconds>(steady_clock::now() - start).count());
			}
		}
		logs << typeid(cont).name() << " - OK!" << std::endl;
		return results;
	}

private:
	bool isProfile(int ind) {
		using namespace std;
		return ind + 1 >= 1000 && ((ind + 1) % static_cast<int>(pow(10, floor(log10(ind + 1))))) == 0;
	}

	template<class Func>
	std::vector<std::size_t> profileFunc(Func func) {
		using namespace std;
		using namespace std::chrono;
		using namespace std::placeholders;

		auto start = steady_clock::now();
		vector<size_t> res;
		res.reserve(41);
		for (int i = 0; i < randArr.size(); i++) {
			func(&cont, randArr[i]);
			if (isProfile(i)) {
				res.push_back(duration_cast<milliseconds>(steady_clock::now() - start).count());
			}
		}
		return res;
	}

	std::vector<size_t> profileSort() {
		using namespace std;
		using namespace std::chrono;
		using namespace std::placeholders;

		vector<size_t> res;
		res.reserve(41);
		for (int i = 0; i < randArr.size(); i++) {
			insert(&cont, randArr[i]);
			if (isProfile(i)) {
				auto start = steady_clock::now();
				getSorted(&cont);
				res.push_back(duration_cast<milliseconds>(steady_clock::now() - start).count());
			}
		}
		return res;
	}
};