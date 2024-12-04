#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

class ListParser {
	private:
		std::pair<int, int> parse_pair(std::string pair) {
			int space = pair.find(" ");
			
			if (space == std::string::npos) {
				return std::pair(-1, -1);
			}

			std::string s_left = pair.substr(0, space);
			std::string s_right = pair.substr(space);

			int left = std::stoi(s_left);
			int right = std::stoi(s_right);

			return (std::pair(left, right));
		}
	public:
		std::pair<std::vector<int>, std::vector<int>> parse_list(std::ifstream& file) {
			std::vector<int> left;
			std::vector<int> right;

			std::string line;
			
			while(std::getline(file, line)) {
				std::pair<int, int> pair = this->parse_pair(line);
				if (pair.first == -1) {
					break;
				}
				left.push_back(pair.first);
				right.push_back(pair.second);

				// std::cout << line << " -> " << pair.first << ", " << pair.second << std::endl;
			}
			file.close();

			return (std::pair(left, right));
		}
};

int main() {
	ListParser parser = ListParser();
	int similarity_score = 0;

	std::ifstream file("data.in");
	if (!file.is_open()) {
		std::cerr << "Could not open file \"data.in\"" << std::endl;
		return (1);
	}

	auto lists = parser.parse_list(file);
	std::vector<int> left = lists.first;
	std::vector<int> right = lists.second;

	std::unordered_map<int, int> right_count;

	for (int i : right) {
		if (right_count.count(i) == 0) {
			right_count[i] = 0;
		}
		right_count[i]++;
	}

	for (int i : left) {
		if (right_count.count(i) == 1) {
			similarity_score += i * right_count.at(i);
		}
	}

	std::cout << "Similarity Score: " << similarity_score << std::endl;
}
