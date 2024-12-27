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

typedef std::pair<int, int> t_pos;

bool operator==(const t_pos& first, const t_pos& second) {
	return (first.first == second.first && first.second == second.second);
}

std::vector<t_pos> get_trailheads(std::vector<std::vector<char>> map) {
	std::vector<t_pos> trailheads;

	int rows = map.size();
	int cols = map.at(0).size();
	
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			if (map.at(row).at(col) == '0') {
				trailheads.push_back({row, col});
			}
		}
	}

	return (trailheads);
}

bool is_in(int row, int col, int rows, int cols) {
	return (row >= 0 && row < rows && col >= 0 && col < cols);
}

int get_valid_trails(const std::vector<std::vector<char>> map, t_pos pos, std::set<t_pos>& unique) {
	if (map.at(pos.first).at(pos.second) == '9') {
		unique.insert({pos.first, pos.second});
		return (1);
	}

	std::vector<t_pos> dirs({
		{-1, 0},
		{0, 1},
		{0, -1},
		{1, 0},
	});

	char current_value = map.at(pos.first).at(pos.second);
	
	int trails = 0;
	for (auto dir : dirs) {
		if (!is_in(pos.first + dir.first, pos.second + dir.second, map.size(), map.at(0).size())) {
			continue;
		}

		if (map.at(pos.first + dir.first).at(pos.second + dir.second) == current_value + 1) {
			trails += get_valid_trails(map, {pos.first + dir.first, pos.second + dir.second}, unique);
		}
	}

	if (debug_mode) {
		for (int row = 0; row < map.size(); row++) {
			for (int col = 0; col < map.at(0).size(); col++) {
			        if (unique.count({row, col})) {
					std::cout << "\x1B[48;5;196m" << map.at(row).at(col) << "\x1B[0m";
				} else {
					std::cout << map.at(row).at(col);
				}
			}
			std::cout << std::endl;
		}
	}

	return (unique.size());
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

	int rows = lines.size();
	int cols = lines.at(0).size();

	std::vector<std::vector<char>> map(rows);
	for (int i = 0; i < rows; i++) {
		map.at(i) = std::vector<char>(cols);
	}

	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			map.at(r).at(c) = lines.at(r).at(c);
		}
	}

	std::vector<t_pos> trailheads = get_trailheads(map);

	if (debug_mode) {
		std::cout << "Trailheads: [ ";
		for (t_pos pos : trailheads) {
			std::cout << "(" << pos.first << ", " << pos.second << ") ";
		}
		std::cout << "]" << std::endl;
	}

	int trails = 0;
	std::set<t_pos> unique;

	for (t_pos trailhead : trailheads) {
		unique.clear();
		int t = get_valid_trails(map, trailhead, unique);
		if (debug_mode) {
			std::cout << "Trailhead (" << trailhead.first << ", " << trailhead.second << "): " << t << std::endl;
		}
		trails += t;
	}

	std::cout << "Trails: " << trails << std::endl;
	
}
