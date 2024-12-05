#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

bool debug_mode = false;

class FileReader {
	private:
		std::ifstream file;
		bool open;
	public:
		FileReader(std::string file_name) {
			this->file = std::ifstream(file_name);
			this->open = true;
			if (!file.is_open()) {
				this->open = false;
				std::cerr << "Could not open file \"" << file_name << "\"" << std::endl;
			}
		}

		std::string get_next_line() {
			std::string line;

			if (file.eof()) {
				return ("");
			}
			std::getline(this->file, line);
			return (line);
		}

		std::vector<std::string> get_all_lines() {
			std::vector<std::string> lines;
			while (!this->file.eof()) {
				std::string line;
				std::getline(this->file, line);
				lines.push_back(line);
			}

			return (lines);
		}

		std::string get_string() {
			std::string str;
			while (!this->file.eof()) {
				std::string line;
				std::getline(this->file, line);
				str.append(line);
			}

			return (str);
		}

		bool is_open() {
			return (this->open);
		}
};

class Parser {
	private:
	public:
		static std::pair<int, int> parse_rule(std::string rule) {
			// Rule has the format:
			// x|y
			int line_index = rule.find('|');
			
			std::string num1 = rule.substr(0, line_index);
			std::string num2 = rule.substr(line_index + 1);

			if (debug_mode) {
				std::cout << rule << " -> " << num1 << "|" << num2 << std::endl;
			}

			int n1 = std::stoi(num1);
			int n2 = std::stoi(num2);

			return (std::pair<int, int>(n1, n2));
		}

		static std::vector<int> parse_number_list(std::string str, char delim) {
			std::vector<int> list;

			while (!str.empty()) {
				int delim_index = str.find(delim);
				if (delim_index == std::string::npos) {
					list.push_back(std::stoi(str));
					break;
				}

				list.push_back(std::stoi( str.substr(0, delim_index) ));

				str = str.substr(delim_index + 1);
			}

			return (list);
		}
};

class Visualizer {
	private:
	public:
		template <typename T>
		static void print(const std::vector<T>& v) {
			std::cout << "[ ";
			for (int i = 0; i < v.size(); i++) {
				std::cout << v[i];
				if (i != v.size() - 1) {
					std::cout << ", ";
				}
			}
			std::cout << " ]";
		}

		template <typename T>
		static void print(const std::unordered_set<T>& s) {
			std::cout << "{ ";
			for (T elem : s) {
				std::cout << elem << " ";
			}
			std::cout << "}";
		}
};

void print_rules(std::unordered_map<int, std::unordered_set<int>>& rules) {
	for (auto rule : rules) {
		int key = rule.first;
		std::unordered_set<int> value = rule.second;
		
		std::cout << key << ": ";
		Visualizer::print(value);
		std::cout << std::endl;
	}
}

int validate_update(const std::unordered_map<int, std::unordered_set<int>> rules, std::vector<int>& update);
void fix_update(const std::unordered_map<int, std::unordered_set<int>> rules, std::vector<int>& update, int num1, int num2) {
	auto it_1 = update.begin();
	auto it_2 = update.begin();

	for (it_1 = update.begin(); it_1 != update.end(); it_1++) {
		if (*it_1 == num1) {
			break;
		}
	}
	for (it_2 = update.begin(); it_2 != update.end(); it_2++) {
		if (*it_2 == num2) {
			break;
		}
	}

	std::iter_swap(it_1, it_2);

	if (debug_mode) {
		Visualizer::print(update);
		std::cout << std::endl;
	}

	validate_update(rules, update);
}

// This returns the middle number if the update is valid, 0 if not
int validate_update(const std::unordered_map<int, std::unordered_set<int>> rules, std::vector<int>& update) {
	std::unordered_set<int> previous_pages;

	for (int page : update) {
		// Check the number's rules (if they exist)
		if (rules.count(page) == 0) {
			previous_pages.insert(page);
			continue;
		}
		
		std::unordered_set<int> blacklist = rules.at(page); // This is a set of pages that cannot appear BEFORE the current page
		
		// Check that this page doesn't break the rules
		for (int prev_page : previous_pages) {
			if (blacklist.count(prev_page) == 1) {
				fix_update(rules, update, prev_page, page);
				return (0);
			}
		}
		previous_pages.insert(page);
	}
		if (debug_mode) {
			Visualizer::print(update);
			std::cout << " -> " << update.at(update.size() / 2) << std::endl;
		}

		return (update.at(update.size() / 2));
}

int main(int argc, char *argv[]) {
	if (argc >= 2) {
		std::string options = std::string(argv[1]);
		if (options.compare("-d") == 0) {
				debug_mode = true;
		}
	}
	
	std::string file_name = "data.in";
	FileReader reader = FileReader(file_name);
	if (!reader.is_open()) {
		return (1);
	}

	int found = 0;
	
	if (debug_mode) {
		std::cout << "Reading lines..." << std::endl;
	}

	std::vector<std::string> lines = reader.get_all_lines(); // [line][col]
	
	if (debug_mode) {
		std::cout << "Lines read successfully" << std::endl;
	}

	
	auto it = lines.begin();
	for (it = lines.begin(); it != lines.end(); it++) {
		// Check if the line is empty (switch to update page lists)
		if ((*it).empty()) {
			break;
		}
	}

	std::vector<std::string> rules = std::vector<std::string>(lines.begin(), it);
	std::vector<std::string> updates = std::vector<std::string>(++it, --lines.end());

	std::unordered_map<int, std::unordered_set<int>> rule_map;

	for (std::string rule : rules) {
		std::pair<int, int> rule_values = Parser::parse_rule(rule);
		int key = rule_values.first;
		int value = rule_values.second;

		if (rule_map.count(key) == 0) {
			rule_map[key] = std::unordered_set<int>();
		}
		rule_map[key].insert(value);
	}
	if (debug_mode) {
		std::cout << "Rules:" << std::endl;
		print_rules(rule_map);
	}

	int total = 0;

	for (std::string update : updates) {
		if (update.empty()) {
			break;
		}
		std::vector<int> update_v = Parser::parse_number_list(update, ',');
		
		int value = validate_update(rule_map, update_v);
		if (value == 0) {
			total += update_v.at(update_v.size() / 2);
		}
	}

	std::cout << "Total: " << total << std::endl;
}
