#include <vector>
#include <iostream>
#include <fstream>
#include <string>

bool debug_mode = false;

class ReportAnalyzer {
	private:
	public:
		// Now returns the index of the first level that breaks
		// Returns -1 for safe
		int check_report(std::vector<int> report, int min_diff = 1, int max_diff = 3) {
			int is_increasing = true;
			if (report[1] < report[0]) {
				is_increasing = false;
			}

			for (int i = 0; i < report.size() - 1; i++) {
				if (report[i + 1] > report[i] && is_increasing == false) {
					return (i + 1);
				}
				if (report[i + 1] < report[i] && is_increasing == true) {
					return (i + 1);
				}

				int diff = report[i + 1] - report[i];
				if (diff < 0) {
					diff = -diff;
				}

				if (diff < min_diff || diff > max_diff) {
					return (i + 1);
				}
			}

			return (-1);
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
				
				if (debug_mode) {
					std::cout << "Parsed List: [ ";
					for (int n : list) {
						std::cout << n << " ";
					}
					std::cout << "]" << std::endl;
				}

				list.clear();
			}
			file.close();

			return (megalist);
		}
};

void print_report(std::vector<int> report, int flag) {
	if (flag == -1) {
		std::cout << "SAFE";
	}
	else if (flag == -2) {
		std::cout << "UNSAFE";
	}
	else {
		std::cout << "-[" << flag << "]";
	}
	
	std::cout << '\t';

	for (int n : report) {
		std::cout << n << " ";
	}

	std::cout << std::endl;
}

int main(int argc, char *argv[]) {
	if (argc >= 2) {
		std::string options = std::string(argv[1]);
		if (options.compare("-d") == 0) {
				debug_mode = true;
		}
	}

	ListParser parser = ListParser();
	ReportAnalyzer analyzer = ReportAnalyzer();
	std::vector<std::vector<int>> reports;

	std::string file_name = "input.data";
	std::ifstream file(file_name);
	if (!file.is_open()) {
		std::cerr << "Could not open file \"" << file_name << "\"" << std::endl;
		return (1);
	}

	reports = parser.parse_list(file);
	
	int safe_reports = 0;
	for (std::vector<int> report : reports) {
		if (report.size() == 0) {
			break;
		}
		
		int validation = analyzer.check_report(report);
		if (validation == -1) {
			print_report(report, validation);
			safe_reports++;
		}
		else { // This could mean either the first or second level is invalid
			std::vector<int> first_removed = std::vector<int>(report);
			first_removed.erase(first_removed.begin());

			std::vector<int> second_removed = std::vector<int>(report);
			second_removed.erase(++second_removed.begin());

			std::vector<int> modified = std::vector<int>(report);
			auto it = modified.begin();
			for (int i = 0; i < validation; i++)
				it++;
			modified.erase(it);
			
			int first = analyzer.check_report(first_removed);
			int second = analyzer.check_report(second_removed);
			int removed = analyzer.check_report(modified);

			if (debug_mode) {
				std::cout << "Original Vector" << std::endl;
				print_report(report, -2);
				
				std::cout << "First Removed" << std::endl;
				print_report(first_removed, 0);
			
				std::cout << "Second Removed" << std::endl;
				print_report(second_removed, 1);
				
				std::cout << "One Removed" << std::endl;
				print_report(modified, validation);
			}

			if (first == -1 || second == -1 || removed == -1) {
				print_report(report, validation);
				safe_reports++;
			}
			else {
				print_report(report, -2);
			}
		}
	}

	std::cout << "Safe Reports: " << safe_reports << std::endl;

}
