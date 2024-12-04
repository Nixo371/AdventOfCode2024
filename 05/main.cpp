#include <iostream>
#include <fstream>
#include <string>
#include <vector>

bool debug_mode = false;

class MulParser {
	private:
	public:
		std::vector<std::string> parse_input(std::ifstream& input) {
			std::vector<std::string> lines;

			std::string data;
			while (!input.eof()) {
				std::string line;
				std::getline(input, line);
				data.append(line);
			}

			std::size_t found_index = data.find("mul(");
			while (found_index != std::string::npos) {
				lines.push_back(data.substr(found_index, 12)); // max length = 12 -> mul(xxx,yyy)
				data = data.substr(found_index + 1);
				found_index = data.find("mul(");
			}
	
			input.close();
			return (lines);
		}
		
		// Returns the value of the multiplication or -1 if it's invalid
		int check_mul(std::string str) {
			// First 4 characters are valid since we searched for them explicitly
			int num1;
			int num2;

			if (debug_mode) {
				std::cout << "Testing \"" << str << "\"";
			}

			str = str.substr(4);

			if (debug_mode) {
				std::cout << " -> " << str << std::endl;
			}

			int max_digits = 3;
			
			int digits_1 = 0;
			while (str.at(digits_1) >= '0' && str.at(digits_1) <= '9') {
				digits_1++;
			}

			if (digits_1 == 0 || digits_1 > max_digits) {
				return (-1);
			}
			
			// Set the first number
			num1 = std::stoi(str.substr(0, digits_1));

			if (debug_mode) {
				std::cout << "First Number: " << num1 << std::endl;
			}
			
			// Check for the comma
			if (str.at(digits_1) != ',') {
				return (-1);
			}

			// Shift string "xxx,yyy)" -> "yyy)"
			str = str.substr(digits_1 + 1);
			
			int digits_2 = 0;
			while (str.at(digits_2) >= '0' && str.at(digits_2) <= '9') {
				digits_2++;
			}

			if (digits_2 == 0 || digits_2 > max_digits) {
				return (-1);
			}
			
			// Set the second number
			num2 = std::stoi(str.substr(0, digits_2));

			if (debug_mode) {
				std::cout << "Second Number: " << num2 << std::endl;
			}
			
			// Check for the comma
			if (str.at(digits_2) != ')') {
				std::cout << "NO PAREN" << std::endl;
				return (-1);
			}

			if (debug_mode) {
				std::cout << "Returning: " << num1 << " * " << num2 << " = " << num1 * num2 << std::endl;
			}

			return (num1 * num2);
		}
};

int main(int argc, char *argv[]) {
	if (argc >= 2) {
		std::string options = std::string(argv[1]);
		if (options.compare("-d") == 0) {
				debug_mode = true;
		}
	}

	MulParser parser = MulParser();

	std::string file_name = "data.in";
	std::ifstream file(file_name);
	if (!file.is_open()) {
		std::cerr << "Could not open file \"" << file_name << "\"" << std::endl;
		return (1);
	}

	std::vector<std::string> lines = parser.parse_input(file);
	
	int total = 0;
	for (std::string line : lines) {
		int res = parser.check_mul(line);
		if (res == -1) {
			std::cout << "ERROR " << line << std::endl;
			continue;
		}
		total += res;
	}

	std::cout << "Total: " << total << std::endl;
}
