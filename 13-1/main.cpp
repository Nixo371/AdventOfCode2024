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

class Button {
	private:
		int x;
		int y;
	public:
		Button() {
			this->x = 0;
			this->y = 0;
		}

		Button(int x, int y) {
			this->x = x;
			this->y = y;
		}

		int get_x() const {
			return (this->x);
		}
		
		int get_y() const {
			return (this->y);
		}
};

class SlotMachine {
	private:
		Button a;
		Button b;
		std::pair<int, int> prize;
	public:
		SlotMachine(Button a, Button b, std::pair<int, int> prize) {
			this->a = a;
			this->b = b;
			this->prize = prize;
		}

		SlotMachine(int a_x, int a_y, int b_x, int b_y, std::pair<int, int> prize) {
			this->a = Button(a_x, a_y);
			this->b = Button(b_x, b_y);
			this->prize = prize;
		}
};

std::pair<int, int> operator*(const Button& a, int b) {
	return {a.get_x() * b, a.get_y() * b};
}

std::pair<int, int> operator+(const std::pair<int, int>& a, const std::pair<int, int>& b) {
	return {a.first + b.first, a.second + b.second};
}

// "Button X: X+[], Y+[]"
Button parse_button(const std::string line) {
	int comma = line.find(',');
	int x = std::stoi(line.substr(12, comma - 12));

	int last_plus = line.rfind('+');
	int y = std::stoi(line.substr(last_plus + 1));

	if (debug_mode) {
		std::cout << line << " -> " << "[" << x << ", " << y << "]" << std::endl;
	}

	return (Button(x, y));
}

// "Prize: X=[], Y=[]"
std::pair<int, int> parse_prize(const std::string line) {
	int comma = line.find(',');
	int x = std::stoi(line.substr(9, comma - 9));

	int last_eq = line.rfind('=');
	int y = std::stoi(line.substr(last_eq + 1));

	if (debug_mode) {
		std::cout << line << " -> " << "[" << x << ", " << y << "]" << std::endl;
	}

	return {x, y};
}

std::vector<SlotMachine> parse_lines(const std::vector<std::string>& lines) {
	std::vector<SlotMachine> machines;

	for (int i = 0; i < lines.size(); i += 4) {
		Button a = parse_button(lines.at(i + 0));
		Button b = parse_button(lines.at(i + 1));
		std::pair<int, int> prize = parse_prize(lines.at(i + 2));
	
		machines.push_back(SlotMachine(a, b, prize));
	}

	return (machines);
}

int main(int argc, char *argv[]) {
	if (argc >= 2) {
		std::string options = std::string(argv[1]);
		if (options.compare("-d") == 0) {
				debug_mode = true;
		}
	}
	
	std::string file_name = "test.in";
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

	std::vector<SlotMachine> machines = parse_lines(lines);
	
}
