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

int check_position(std::vector<std::string>& grid, int row, int col) {
	int rows = grid.size();
	int cols = grid.at(0).size();
	
	// Bounds checking
	if (row - 1 < 0 || row + 1 >= rows) {
		return (0);
	}
	if (col - 1 < 0 || col + 1 >= cols) {
		return (0);
	}

	// Check that the middle letter is 'A'
	if (grid.at(row).at(col) != 'A') {
		return (0);
	}

	// Check NW -> SE line
	if (grid.at(row - 1).at(col - 1) == 'M' && grid.at(row + 1).at(col + 1) == 'S') {
		// valid
	}
	else if (grid.at(row - 1).at(col - 1) == 'S' && grid.at(row + 1).at(col + 1) == 'M') {
		// valid
	}
	else {
		return (0);
	}

	// Check NE -> SW line
	if (grid.at(row - 1).at(col + 1) == 'M' && grid.at(row + 1).at(col - 1) == 'S') {
		// valid
	}
	else if (grid.at(row - 1).at(col + 1) == 'S' && grid.at(row + 1).at(col - 1) == 'M') {
		// valid
	}
	else {
		return (0);
	}

	return (1);
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
