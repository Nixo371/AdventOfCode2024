#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
#include <string>
#include <set>
#include <map>

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

void print_stones(const std::map<long long, long long>& stones) {
	std::cout << "Stones:" << std::endl;
	for (auto [stone, count] : stones) {
		std::cout << "\t{ " << stone << " -> " << count << " }" << std::endl;
	}
}

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

std::map<long long, long long> blink(const std::map<long long, long long>& stones) {
	std::map<long long, long long> new_stones;
	for (auto [stone, count] : stones) {
		if (debug_mode) {
			std::cout << "There are " << count << " stones with the number " << stone << " on them..." << std::endl;
		}
		if (stone == 0) {
			if (debug_mode) {
				std::cout << "\tThe stones turn into " << count << " 1's..." << std::endl;
			}
			new_stones[1] = count;
		}
		else if (std::to_string(stone).size() % 2 == 0) {
			auto [first, second] = split_number(stone);
			if (new_stones.count(first) == 0) {
				new_stones[first] = 0;
			}
			new_stones[first] += count;

			if (new_stones.count(second) == 0) {
				new_stones[second] = 0;
			}
			new_stones[second] += count;
			if (debug_mode) {
				std::cout << "\tThe stones turn into " << count << " " << first << "'s and " << count << " " << second << "'s..." << std::endl;
			}
		}
		else {
			if (new_stones.count(stone * 2024) == 0) {
				new_stones[stone * 2024] = 0;
			}
			new_stones[stone * 2024] += count;
			
			if (debug_mode) {
				std::cout << "\tThe stones turn into " << count << " " << stone * 2024 << "'s..." << std::endl;
			}
		}
	}
	
	return (new_stones);
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

	std::vector<long long> stones_line = parse_line(lines.at(0));
	std::map<long long, long long> stones;

	for (long long stone : stones_line) {
		if (stones.count(stone) == 0) {
			stones[stone] = 0;
		}
		stones[stone]++;
	}

	int blinks = 75;
	for (int i = 0; i < blinks; i++) {
		if (debug_mode) {
			std::cout << i + 1 << "/" << blinks << std::endl;
			print_stones(stones);
		}
		stones = blink(stones);
	}
	if (debug_mode) {
		std::cout << std::endl;
		print_stones(stones);
	}

	long long total_stones = 0;
	
	for (auto [stone, count] : stones) {
		total_stones += count;
	}

	std::cout << "Total Stones: " << total_stones << std::endl;
}
