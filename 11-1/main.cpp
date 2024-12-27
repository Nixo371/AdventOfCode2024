#include <iostream>
#include <fstream>
#include <ostream>
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

std::vector<long long> parse_line(std::string line) {
	std::vector<long long> stones;

	long long num = 0;
	for (char c : line) {
		if (c == ' ') {
			stones.push_back(num);
			num = 0;
			continue;
		}
		num = (num * 10) + (c - '0');
	}

	stones.push_back(num);

	return (stones);
}

std::pair<long long, long long> split_number(long long num) {
	int size = 0;
	long long div = 1;
	while (num / div) {
		div *= 10;
		size++;
	}

	div = 1;
	for (int i = 0; i < size; i += 2) {
		div *= 10;
	}
	return {num / div, num % div};
}

void blink(std::vector<long long>& stones) {
	std::vector<long long> new_stones;
	for (int i = 0; i < stones.size(); i++) {
		if (stones[i] == 0) {
			stones[i] = 1;
		}
		else if (std::to_string(stones[i]).size() % 2 == 0) {
			auto [first, second] = split_number(stones[i]);
			new_stones.push_back(first);
			stones[i] = second;
		}
		else {
			stones[i] *= 2024;
		}
	}

	for (long long stone : new_stones) {
		stones.push_back(stone);
	}
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

	std::vector<long long> stones = parse_line(lines.at(0));

	int blinks = 25;
	for (int i = 0; i < blinks; i++) {
		if (debug_mode) {
			std::cout << i + 1 << "/" << blinks << std::endl;
		}
		blink(stones);
	}
	if (debug_mode) {
		std::cout << std::endl;
	}

	std::cout << "Total Stones: " << stones.size() << std::endl;
}
