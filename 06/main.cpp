#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

bool debug_mode = false;

class MulParser {
	private:
	public:
		std::vector<std::string> parse_input(std::ifstream& input) {
			std::vector<std::string> lines;
			bool enabled = true;

			std::string data;
			while (!input.eof()) {
				std::string line;
				std::getline(input, line);
				data.append(line);
			}

			std::size_t mul_found_index;
			std::size_t do_found_index;
			std::size_t dont_found_index;
			do {
				mul_found_index = data.find("mul(");
				do_found_index = data.find("do()");
				dont_found_index = data.find("don't()");

				if (mul_found_index == std::string::npos) {
					mul_found_index = data.length() - 1;
				}
				if (do_found_index == std::string::npos) {
					do_found_index = data.length() - 1;
				}
				if (dont_found_index == std::string::npos) {
					dont_found_index = data.length() - 1;
				}
			
				if (debug_mode) {
					std::cout << "Found: " << std::endl;
					std::cout << "\t" << data.substr(mul_found_index, 12) << "..." << std::endl;
					std::cout << "\t" << data.substr(do_found_index, 4) << "..." << std::endl;
					std::cout << "\t" << data.substr(dont_found_index, 7) << "..." << std::endl;
				}

				// Get the first keyword
				std::size_t found_index = std::min(std::min(mul_found_index, do_found_index), dont_found_index); 
				
				if (debug_mode) {
					std::cout << "Closest Keyword:" << std::endl;
					std::cout << "\t" << data.substr(found_index, 7) << "..." << std::endl;
				}

				// Since the 3rd character is unique we'll switch based off of that
				data = data.substr(found_index);
				
				if (debug_mode) {
					std::cout << "Resized String:" << std::endl;
					std::cout << "\t" << data.substr(0, 12) << "..." << std::endl;
					std::cout << "Found: '" << data.at(2) << "'" << std::endl;
				}

				if (data.length() < 3) {
					break;
				}

				switch (data.at(2)) {
					case 'l': // mul
						if (enabled) {
							lines.push_back(data.substr(0, 12)); // max length = 12 -> mul(xxx,yyy)
						}
						data = data.substr(4); // We don't know where this "mul" ends so we remove what we know can't be used
						break;
					case '(': // do 
						enabled = true;
						data = data.substr(4); // "do()".len == 4
						break;
					case 'n': // don't
						enabled = false;
						data = data.substr(7); // "don't()".len == 7
						break;
				}

				if (debug_mode) {
					std::cout << "Resized String after switch:" << std::endl;
					std::cout << "\t" << data.substr(0, 12) << "..." << std::endl;
					std::cout << "Found: '" << data.at(2) << "'" << std::endl;
				}

			} while (mul_found_index != std::string::npos && data.length() > 8);

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
