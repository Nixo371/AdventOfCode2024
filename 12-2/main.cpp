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

void print_region(const std::vector<std::vector<char>>& map, const Region& region) {
	int rows = map.size();
	int cols = map.at(0).size();

	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			if (region.is_in(row, col)) {
				std::cout << "\033[48;5;196m";
			}
			std::cout << map.at(row).at(col);
			std::cout << "\033[0m";
		}
		std::cout << std::endl;
	}
}

void print_region_bounds(const std::vector<std::vector<char>>& map, int top, int bottom, int left, int right) {
	int rows = map.size();
	int cols = map.at(0).size();

	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			if (row >= top && row <= bottom && col >= left && col <= right) {
				std::cout << "\033[48;5;27m";
			}
			std::cout << map.at(row).at(col);
			std::cout << "\033[0m";
		}
		std::cout << std::endl;
	}
}

bool is_in(int rows, int cols, int row, int col) {
	return (row >= 0 && row < rows && col >= 0 && col < cols);
}

void get_inverse_region(const std::vector<std::vector<char>>& map, Region &region, int row, int col) {
	// Takes a position and returns the region that the position is in, AVOIDING the region type
	
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

		if (map.at(row + dr).at(col + dc) == region.type) {
			continue;
		}

		get_inverse_region(map, region, row + dr, col + dc);
	}

}

bool is_island(const std::vector<std::vector<char>>& map, const Region& region, const Region& island, std::pair<int, int> pos) {
	int rows = map.size();
	int cols = map.at(0).size();

	Region island_copy = Region(island.type);

	get_inverse_region(map, island_copy, pos.first, pos.second);

	int r_top = rows - 1;
	int r_bottom = 0;
	int r_left = cols - 1;
	int r_right = 0;
	
	for (auto [row, col] : region.plant_positions) {
		r_top = std::min(r_top, row);
		r_bottom = std::max(r_bottom, row);
		r_left = std::min(r_left, col);
		r_right = std::max(r_right, col);
	}

	int i_top = rows - 1;
	int i_bottom = 0;
	int i_left = cols - 1;
	int i_right = 0;

	for (auto [row, col] : island_copy.plant_positions) {
		i_top = std::min(i_top, row);
		i_bottom = std::max(i_bottom, row);
		i_left = std::min(i_left, col);
		i_right = std::max(i_right, col);
	}

	return (i_top > r_top && i_bottom < r_bottom && i_left > r_left && i_right < r_right);
}

int calculate_sides(const std::vector<std::vector<char>>& map, const Region &region) {
	// Go north until hit the edge
	// Turn right
	// Try turning left at every point, then straight, and finally right again
	// Count total turns
	// Hopefully that's the amount of sides

	int sides = 0;

	int rows = map.size();
	int cols = map.at(0).size();
	
	std::vector<std::pair<int, int>> dirs({
			{-1, 0},
			{0, 1},
			{1, 0},
			{0, -1}
			});
	int dir = 0;

	auto [row, col] = *(region.plant_positions.begin());

	while (region.is_in(row, col)) {
		row -= 1;
	}
	row++;

	auto start_pos = std::pair<int, int>(row, col);

	dir = 1;

	do {
		auto [dr, dc] = dirs[dir];

		dir = (dir + 3) % 4; // Turn left
		dr = dirs[dir].first;
		dc = dirs[dir].second;
		if (region.is_in(row + dr, col + dc)) {
			sides++;
			row += dr;
			col += dc;
			continue;
		}
		dir = (dir + 1) % 4; // Go back to original direction
		dr = dirs[dir].first;
		dc = dirs[dir].second;
		if (region.is_in(row + dr, col + dc)) {
			row += dr;
			col += dc;
			continue;
		}
		dir = (dir + 1) % 4; // Turn right
		dr = dirs[dir].first;
		dc = dirs[dir].second;
		if (region.is_in(row + dr, col + dc)) {
			sides++;
			row += dr;
			col += dc;
			continue;
		}
		sides++;
	} while (std::pair<int, int>(row, col) != start_pos);

	while (dir != 1) {
		dir = (dir + 1) % 4;
		sides++;
	}

	// TODO
	// Detect inner islands
	//
	// Go from all 4 "edges" of the region in and beam out to the edge of the map, if when it's out it's always out, no island there
	// Make a pseudo region with the island (there may be multiple different regions stuck together on an island) and calculate how many sides it has
	
	// Get bounds of region
	int top = rows - 1;
	int bottom = 0;
	int left = cols - 1;
	int right = 0;
	for (auto [row, col] : region.plant_positions) {
		top = std::min(top, row);
		bottom = std::max(bottom, row);
		left = std::min(left, col);
		right = std::max(right, col);
	}

	/*
	if (debug_mode) {
		print_region_bounds(map, top, bottom, left, right);
		std::cout << std::endl;
	}
	*/

	std::map<std::pair<int, int>, int> possible_islands;

	// top down
	for (int col = left; col <= right; col++) {
		int row = top;
		// Go down until region starts
		while (row <= bottom && !region.is_in(row, col)) {
			row++;
		}

		// Go down, if not in region it *could* be an island
		while (row <= bottom) {
			if (!region.is_in(row, col)) {
				if (possible_islands.count({row, col}) == 0) {
					possible_islands[{row, col}] = 0;
				}
				possible_islands[{row, col}]++;
			}
			row++;
		}
	}

	// bottom up
	for (int col = left; col <= right; col++) {
		int row = bottom;
		// Go up until region starts
		while (row >= top && !region.is_in(row, col)) {
			row--;
		}

		// Go up, if not in region it *could* be an island
		while (row >= top) {
			if (!region.is_in(row, col)) {
				if (possible_islands.count({row, col}) == 0) {
					possible_islands[{row, col}] = 0;
				}
				possible_islands[{row, col}]++;
			}
			row--;
		}
	}

	// left right
	for (int row = top; row <= bottom; row++) {
		int col = left;
		// Go right until region starts
		while (col <= right && !region.is_in(row, col)) {
			col++;
		}

		// Go right, if not in region it *could* be an island
		while (col <= right) {
			if (!region.is_in(row, col)) {
				if (possible_islands.count({row, col}) == 0) {
					possible_islands[{row, col}] = 0;
				}
				possible_islands[{row, col}]++;
			}
			col++;
		}
	}
	
	// right left
	for (int row = top; row <= bottom; row++) {
		int col = right;
		// Go left until region starts
		while (col >= left && !region.is_in(row, col)) {
			col--;
		}

		// Go left, if not in region it *could* be an island
		while (col >= left) {
			if (!region.is_in(row, col)) {
				if (possible_islands.count({row, col}) == 0) {
					possible_islands[{row, col}] = 0;
				}
				possible_islands[{row, col}]++;
			}
			col--;
		}
	}

	Region island = Region(region.type);
	for (auto [island_pos, count] : possible_islands) {
		if (count == 4 && is_island(map, region, island, island_pos)) {
			island.plant_positions.insert(island_pos);
		}
	}

	if (debug_mode) {
		if (island.plant_positions.size() != 0) {
			print_region(map, island);
		}
	}

	if (island.plant_positions.size() != 0) {
		int extra_sides = calculate_sides(map, island);
		if (debug_mode) {
			std::cout << "Sides: " << sides << " + " << extra_sides << std::endl;
		}
		sides += extra_sides;
	}

	return (sides);
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
		int sides = calculate_sides(map, region);
		if (debug_mode) {
			std::cout << "Region [" << region.type << "]: " << region.area << " * " << sides << " = " << region.area * sides << std::endl;
		}

		total_cost += region.area * sides;
	}

	std::cout << "Total Cost: " << total_cost << std::endl;
}
