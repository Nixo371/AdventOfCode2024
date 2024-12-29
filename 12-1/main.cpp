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

std::vector<std::vector<char>> parse_lines(std::vector<std::string> lines) {
	int rows = lines.size();
	int cols = lines.at(0).size();

	std::vector<std::vector<char>> map(rows);
	for (int i = 0; i < rows; i++) {
		map.at(i) = std::vector<char>(cols);
	}


	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			map.at(row).at(col) = lines.at(row).at(col);
		}
	}

	return (map);
}

class Region {
	private:
	public:
		std::set<std::pair<int, int>> plant_positions;
		int area;
		int perimeter;
		char type;

		Region(char type) {
			this->type = type;
		}

		void calculate_area() {
			this->area = plant_positions.size();
		}

		void calculate_perimeter() {
			int perimeter = 4 * plant_positions.size();
			std::vector<std::pair<int, int>> dirs({
					{-1, 0},
					{1, 0},
					{0, -1},
					{0, 1}
			});

			for (auto [row, col] : plant_positions) {
				for (auto [dr, dc] : dirs) {
					perimeter -= plant_positions.count({row + dr, col + dc});
				}
			}

			this->perimeter = perimeter;
		}

		bool is_in(int row, int col) const {
			return (plant_positions.count({row, col}) == 1);
		}
};

bool is_in(int rows, int cols, int row, int col) {
	return (row >= 0 && row < rows && col >= 0 && col < cols);
}

void get_region(const std::vector<std::vector<char>>& map, Region &region, int row, int col) {
	// Takes a position and returns the region that the position is in
	
	region.plant_positions.insert({row, col});

	int rows = map.size();
	int cols = map.at(0).size();

	std::vector<std::pair<int, int>> dirs({
			{-1, 0},
			{1, 0},
			{0, -1},
			{0, 1}
	});

	for (auto [dr, dc] : dirs) {
		// Check that the result would be in bounds
		// Check that it's not already in the region
		// Check that it's the same letter
		// Add it to the region and start again

		if (!is_in(rows, cols, row + dr, col + dc)) {
			continue;
		}

		if (region.is_in(row + dr, col + dc)) {
			continue;
		}

		if (map.at(row + dr).at(col + dc) != region.type) {
			continue;
		}

		get_region(map, region, row + dr, col + dc);
	}

}

std::vector<Region> get_regions(const std::vector<std::vector<char>>& map) {
	std::vector<Region> regions;
	std::set<std::pair<int, int>> visited_positions;

	int rows = map.size();
	int cols = map.at(0).size();

	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			if (visited_positions.count({row, col}) == 1) {
				continue;
			}

			// New region
			Region region = Region(map.at(row).at(col));
			get_region(map, region, row, col);

			for (auto pos : region.plant_positions) {
				visited_positions.insert(pos);
			}

			regions.push_back(region);
		}
	}

	return (regions);
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

	std::vector<std::vector<char>> map = parse_lines(lines);
	
	std::vector<Region> regions = get_regions(map);

	int total_cost = 0;

	for (Region region : regions) {
		region.calculate_area();
		region.calculate_perimeter();
		if (debug_mode) {
			std::cout << "Region [" << region.type << "]: " << region.area << " * " << region.perimeter << " = " << region.area * region.perimeter << std::endl;
		}

		total_cost += region.area * region.perimeter;
	}

	std::cout << "Total Cost: " << total_cost << std::endl;
}
