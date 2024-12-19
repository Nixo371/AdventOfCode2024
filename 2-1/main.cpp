#include <vector>
#include <iostream>
#include <fstream>
#include <string>

class ReportAnalyzer {
	private:
	public:
		bool check_report(std::vector<int> report, int min_diff = 1, int max_diff = 3) {
			int is_increasing = true;
			if (report[1] < report[0]) {
				is_increasing = false;
			}

			for (int i = 0; i < report.size() - 1; i++) {
				if (report[i + 1] > report[i] && is_increasing == false) {
					return (false);
				}
				if (report[i + 1] < report[i] && is_increasing == true) {
					return (false);
				}

				int diff = report[i + 1] - report[i];
				if (diff < 0) {
					diff = -diff;
				}

				if (diff < min_diff || diff > max_diff) {
					return (false);
				}
			}

			return (true);
		}
};

class ListParser {
	private:
		std::vector<std::string> split(std::string str, char delim = ' ') {
			std::vector<std::string> split;
			std::string s = "";
			for (char c : str) {
				if (c == ' ') {
					if (s.length() > 0) {
						split.push_back(s.substr(0));
						s = "";
					}
					continue;
				}

				s += c;
			}
			split.push_back(s);

			return (split);
		}
	public:
		std::vector<std::vector<int>> parse_list(std::ifstream& file) {
			std::vector<std::vector<int>> megalist;

			std::string line;
			
			while(std::getline(file, line)) {
				std::vector<int> list;
				std::vector<std::string> split_line = this->split(line);
				for (std::string s : split_line) {
					if (s.length() == 0) {
						break;
					}
					list.push_back(std::stoi(s));
				}
				megalist.push_back(std::vector(list));
/*
				std::cout << line << std::endl;
				for (int n : list) {
					std::cout << n << " ";
				}
				std::cout << std::endl;
*/				
				list.clear();
			}
			file.close();

			return (megalist);
		}
};

void print_report(std::vector<int> report, bool is_safe) {
	if (is_safe) {
		std::cout << "✓";
	}
	else {
		std::cout << "✗";
	}
	
	std::cout << '\t';

	for (int n : report) {
		std::cout << n << " ";
	}

	std::cout << std::endl;
}

int main() {
	ListParser parser = ListParser();
	ReportAnalyzer analyzer = ReportAnalyzer();
	std::vector<std::vector<int>> reports;

	std::ifstream file("data.in");
	if (!file.is_open()) {
		std::cerr << "Could not open file \"data.in\"" << std::endl;
		return (1);
	}

	reports = parser.parse_list(file);
	
	int safe_reports = 0;
	for (std::vector<int> report : reports) {
		if (report.size() == 0) {
			break;
		}

		if (analyzer.check_report(report)) {
			print_report(report, true);
			safe_reports++;
		}
		else {
			print_report(report, false);
		}
	}

	std::cout << "Safe Reports: " << safe_reports << std::endl;

}
