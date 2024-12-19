#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>

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
				if (!this->file.eof()) {
					lines.push_back(line);
				}
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
		static void print(const std::set<T>& s) {
			std::cout << "{ ";
			for (T elem : s) {
				std::cout << elem << " ";
			}
			std::cout << "}";
		}
};

std::pair<long long, std::vector<int>> parse_line(std::string line) {
	if (line.empty()) {
		return {-1, {}};
	}

	size_t colon = line.find(':');
	std::string result = line.substr(0, colon);
	std::string numbers = line.substr(colon + 2);

	long long big_boi = 0;
	for (char c : result) {
		big_boi = (big_boi * 10) + (c - '0');
	}
	
	std::vector<int> values;
	int num = 0;
	for (char c : numbers) {
		if (c == ' ') {
			values.push_back(num);
			num = 0;
		}
		else {
			num = (num * 10) + (c - '0');
		}
	}
	values.push_back(num);

	return (std::pair<long long, std::vector<int>>(big_boi, values));
}

enum Op {
	NONE,
	ADD,
	MUL
};

long long calculate_solution(long long result, long long running_total, size_t num_index, const std::vector<int>& numbers, std::vector<Op>& operations) {

	// Check the last element and see if it is a factor of the total
	// If it isn't, we know it must be addition (if there is a solution)
	// If it is, we can't check anything else so we must now start checking all possible combinations

	if (debug_mode) {
		std::cout << "Result: " << result << std::endl;
		std::cout << "Running Total: " << running_total << std::endl;
	
		std::cout << "Numbers: ";
		Visualizer::print(numbers);
		std::cout << std::endl;
		
		std::cout << "Operations: ";
		Visualizer::print(operations);
		std::cout << std::endl;

		std::cout << std::endl;
	}

	if (num_index == 0) {
		if (running_total == numbers[0]) { // success
			return (result);
		}
		return (0);
	}

	if (numbers[num_index] != 0 && running_total % numbers[num_index] != 0) {
		operations[num_index - 1] = ADD;
		return (calculate_solution(result, running_total - numbers[num_index], num_index - 1, numbers, operations));
	}

	operations[num_index - 1] = MUL;
	long long multiplying = calculate_solution(result, running_total / numbers[num_index], num_index - 1, numbers, operations);
	
	if (multiplying == result) {
		return multiplying;
	}

	operations[num_index - 1] = ADD;
	long long adding = calculate_solution(result, running_total - numbers[num_index], num_index - 1, numbers, operations);

	if (adding == result) {
		return (adding);
	}

	return (0);
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

	long long total = 0;

	for (std::string line : lines) {
		auto data = parse_line(line);
		long long result = data.first;
		std::vector<int> numbers = data.second;
		std::vector<Op> operations(numbers.size() - 1, NONE);

		long long solution = calculate_solution(result, result, numbers.size() - 1, numbers, operations);
		
		if (debug_mode) {
			if (solution == 0) {
				std::cout << "ERROR " << line << std::endl;
			}
			else {
				std::cout << "GREAT " << line << std::endl;
			}
		}

		total += solution;
	}

	std::cout << "Total: " << total << std::endl;
	
}
