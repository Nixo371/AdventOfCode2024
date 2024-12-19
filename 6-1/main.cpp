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

class Simulator {
	private:
		std::vector<std::string> map;
		std::set<std::pair<int, int>> unique_positions;
		int guard_row;
		int guard_col;

		bool in_bounds(int row, int col) {
			return (row >= 0 && row < map.size() && col >= 0 && col < map.at(0).size()); 
		}

	public:
		Simulator(const std::vector<std::string>& map, int guard_row, int guard_col) {
			this->map = map;
			this->unique_positions = std::set<std::pair<int, int>>();
			this->guard_row = guard_row;
			this->guard_col = guard_col;
		}

		void simulate() {
			std::vector<std::pair<int, int>> dirs({{-1, 0}, {0, 1}, {1, 0}, {0, -1}}); // North, East, South, West
			int dir = 0;
			while (in_bounds(guard_row, guard_col)) {
				this->unique_positions.insert(std::pair<int, int>(guard_row, guard_col));
				int dr = dirs[dir].first;
				int dc = dirs[dir].second;
				
				if (!in_bounds(guard_row + dr, guard_col + dc)) {
					break;
				}

				if (this->map.at(guard_row + dr).at(guard_col + dc) == '#') { // Turn right
					dir = (dir + 1) % 4;
					continue;
				}

				guard_row += dr;
				guard_col += dc;
			}
		}

		int get_unique_positions() {
			return (this->unique_positions.size());
		}
};

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
	
	// Find the position of the guard
	
	int guard_row;
	int guard_col;

	for (int col = 0; col < cols; col++) {
		for (int row = 0; row < rows; row++) {
			if (lines.at(row).at(col) == '^') {
				guard_row = row;
				guard_col = col;
				break;
			}
		}
	}
	
	if (debug_mode) {
		Visualizer::print(lines);
	}

	Simulator sim = Simulator(lines, guard_row, guard_col);
	sim.simulate();

	std::cout << "Unique positions: " << sim.get_unique_positions() << std::endl;

	
}
