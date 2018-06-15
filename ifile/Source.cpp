#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

auto prefix_func(const std::string& s) noexcept {
	std::vector<int> p(s.length());

	auto k = 0; p[0] = 0;
	for (size_t i = 1; i < s.length(); ++i) {
		while (k > 0 && s[k] != s[i]) k = p[k - 1];
		if (s[k] == s[i]) ++k;
		p[i] = k;
	}
	return p;
}

int BM_algorithm(std::string& s, std::string& t) noexcept {
	if (s.length() < t.length()) return -1;
	if (!t.length()) return s.length();

	using TStopTable = std::unordered_map<char, int>; TStopTable stop_table;
	using TSufficsTable = std::unordered_map<int, int>; TSufficsTable suffics_table;

	for (size_t i = 0; i < t.length(); ++i) stop_table[t[i]] = i;

	std::string rt(t.crbegin(), t.crend());
	auto p = prefix_func(t), pr = prefix_func(rt);
	for (size_t i = 0; i < t.length() + 1; ++i) {
		suffics_table[i] = t.length() - p.back();
	}

	for (size_t i = 1; i < t.length(); ++i) {
		auto j = pr[i]; auto k = 1;
		suffics_table[j] = std::min(suffics_table[j], k - pr[i] + 1);
	}

	for (size_t shift = 0; shift <= s.length() - t.length();) {
		auto pos = t.length() - 1;

		while (t[pos] == s[pos + shift]) {
			if (pos == 0) return shift;
			--pos;
		}

		if (pos == t.length() - 1) {
			TStopTable::const_iterator stop_symbol = stop_table.find(s[pos + shift]);
			auto stop_symbol_additional = pos - (stop_symbol != stop_table.end() ? stop_symbol->second : -1);
			shift += stop_symbol_additional;
		}
		else shift += suffics_table[t.length() - pos - 1];
	}

	return -1;
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "RUS");

	std::string str; std::ifstream strf("text.txt"); std::string str1; std::ifstream strf1("text1.txt"); 
	std::string str2; std::ifstream strf2("text2.txt"); std::string sub; std::ifstream subf("sub.txt");
	std::ofstream resf("res.txt");

	while (std::getline(strf, str) && std::getline(strf1, str1) && std::getline(strf2, str2) && std::getline(subf, sub)) {

		std::cout << "Исходные строки: " << str << " " << str1 << " " << str2 << std::endl;
		std::cout << "Подстрока: " << sub << std::endl;

		std::vector<std::string> str_arr{ str, str1, str2 };

		resf << "Строки найденные с помощью алгоритма Бойера - Мура: ";
		std::cout << "\nАлгоритм Бойера - Мура: ";
		for (size_t i = 0; i < str_arr.size(); ++i) {
			std::cout << "\nПроверка строки " << i + 1 << ": ";

			if (BM_algorithm(str_arr[i], sub) >= 0) {
				std::cout << "подстрока найдена на позиции " << BM_algorithm(str_arr[i], sub);
				resf << str_arr[i] << " ";
			}
			else std::cout << "подстрока не найдена\n";;
		}

		resf << "\nСтроки найденные с помощью STL функций: ";
		std::cout << "\nSTL поиск подстроки: ";
		for (size_t i = 0; i < str_arr.size(); ++i) {
			std::cout << "\nПроверка строки " << i + 1 << ": ";
			auto pos = str_arr[i].find(sub);

			if (pos != std::string::npos) {
				std::cout << "подстрока найдена на позиции " << pos;
				resf << str_arr[i] << " ";
			}
			else std::cout << "подстрока не найдена\n";
		}
	}

	system("pause");
	return 0;
}