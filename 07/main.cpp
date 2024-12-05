#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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

int check_position(std::vector<std::string>& grid, int row, int col, int row_offset, int col_offset) {
	int rows = grid.size();
	int cols = grid.at(0).size();
	
	// Bounds checking
	if (row + (row_offset * 3) < 0 || row + (row_offset * 3) >= rows) {
		return (0);
	}
	if (col + (col_offset * 3) < 0 || col + (col_offset * 3) >= cols) {
		return (0);
	}

	// Check each character
	if (grid.at(row).at(col) != 'X')
		return (0);
	row += row_offset;
	col += col_offset;
	
	if (grid.at(row).at(col) != 'M')
		return (0);
	row += row_offset;
	col += col_offset;

	if (grid.at(row).at(col) != 'A')
		return (0);
	row += row_offset;
	col += col_offset;

	if (grid.at(row).at(col) != 'S')
		return (0);
	
	return (1);
}

int check_position(std::vector<std::string>& grid, int row, int col) {
	int found = 0;

	// Check all directions starting north and going clockwise
	found += check_position(grid, row, col, -1, 0);	// N
	found += check_position(grid, row, col, -1, 1);	// NE
	found += check_position(grid, row, col, 0, 1);	// E
	found += check_position(grid, row, col, 1, 1);	// SE
	found += check_position(grid, row, col, 1, 0);	// S
	found += check_position(grid, row, col, 1, -1);	// SW
	found += check_position(grid, row, col, 0, -1);	// W
	found += check_position(grid, row, col, -1, -1);// NW

	return (found);
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

	for (int row = 0; row < lines.size(); row++) {
		for (int col = 0; col < lines.at(0).size(); col++) {
			if (debug_mode) {
				std::cout << "Checking position (" << row << ", " << col << ")" << std::endl;
			}
			int xmas = check_position(lines, row, col);
			found += xmas;
			if (debug_mode) {
				std::cout << "\tFound " << xmas << " XMAS's" << std::endl;
			}
		}
	}

	std::cout << "Total XMAS found: " << found << std::endl;
}
