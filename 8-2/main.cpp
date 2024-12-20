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

/*
std::vector<std::pair<int, int>> find_antenna(const std::vector<std::vector<char>> map, char frequency) {
	std::vector<std::pair<int, int>> antenna;

	size_t rows = map.size();
	size_t cols = map.at(0).size();

	for (size_t row = 0; row < rows; row++) {
		for (size_t col = 0; col < cols; col++) {
			if (map.at)
		}
	}

	return (antenna);
}
*/

bool is_in(int row, int col, int rows, int cols) {
	return (row >= 0 && row < rows && col >= 0 && col < cols);
}

std::map<char, std::vector<std::pair<int, int>>> get_antenna_positions(const std::vector<std::vector<char>> map) {
	std::map<char, std::vector<std::pair<int, int>>> antenna_positions;
	
	size_t rows = map.size();
	size_t cols = map.at(0).size();
	
	for (size_t row = 0; row < rows; row++) {
		for (size_t col = 0; col < cols; col++) {
			char frequency = map.at(row).at(col);
			if (frequency == '.') {
				continue;
			}

			if (antenna_positions.count(frequency) == 0) {
				antenna_positions[frequency] = std::vector<std::pair<int, int>>();
			}
		
			antenna_positions[frequency].push_back({row, col});
		}
	}

	return (antenna_positions);
}

std::set<std::pair<int, int>> calculate_antinodes(const std::vector<std::vector<char>> map, const std::vector<std::pair<int, int>> antenna) {
	std::set<std::pair<int, int>> antinodes;

	int rows = map.size();
	int cols = map.at(0).size();

	for (size_t first = 0; first < antenna.size() - 1; first++) {
		for (size_t second = first + 1; second < antenna.size(); second++) {
			std::pair<int, int> pos_1 = antenna.at(first);
			std::pair<int, int> pos_2 = antenna.at(second);

			std::pair<int, int> diff = {pos_2.first - pos_1.first, pos_2.second - pos_1.second};

			int dist = 1;
			std::pair<int, int> antinode = {pos_1.first, pos_1.second};
			while (is_in(antinode.first, antinode.second, rows, cols)) {
				antinodes.insert(antinode);
				antinode = {pos_1.first + diff.first * dist, pos_1.second + diff.second * dist};
				dist++;
			}
			dist = 1;
			antinode = {pos_1.first - diff.first, pos_1.second - diff.second};
			while (is_in(antinode.first, antinode.second, rows, cols)) {
				antinodes.insert(antinode);
				antinode = {pos_1.first - diff.first * dist, pos_1.second - diff.second * dist};
				dist++;
			}
		}
	}

	return (antinodes);
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

	size_t rows = lines.size();
	size_t cols = lines.at(0).size();

	std::vector<std::vector<char>> map(rows); // .at(row).at(col)
	for (int i = 0; i < rows; i++) {
		map.at(i) = std::vector<char>(cols);
	}
	
	for (size_t row = 0; row < rows; row++) {
		for (size_t col = 0; col < cols; col++) {
			map.at(row).at(col) = lines.at(row).at(col);
		}
	}

	// maps a frequency to a list of positions
	std::map<char, std::vector<std::pair<int, int>>> antenna_positions = get_antenna_positions(map);
	
	std::set<std::pair<int, int>> antinodes;

	for (const auto [key, value] : antenna_positions) {
		std::vector<std::pair<int, int>> antenna = value;
		antinodes.merge(calculate_antinodes(map, antenna));
	}

	if (debug_mode) {
		for (auto antinode : antinodes) {
			std::cout << "(" << antinode.first << ", " << antinode.second << ")" << std::endl;
		}
	}

	std::cout << "Total Antinodes: " << antinodes.size() << std::endl;
}
