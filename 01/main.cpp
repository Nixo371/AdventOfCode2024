#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>

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

class Sorter {
	private:
		int partition(std::vector<int>& vec, int low, int high) {
			int pivot = vec[high];
			int i = low;

			for (int j = low; j < high; j++) {
				if (vec[j] >= pivot)
					continue;
				std::swap(vec[i], vec[j]);
				i++;
			}
			std::swap(vec[i], vec[high]);
			
			return (i);
		}

		void quicksort(std::vector<int>& vec, int low, int high) {
			if (low >= high) {
				return;
			}

			int part = this->partition(vec, low, high);

			quicksort(vec, low, part - 1);
			quicksort(vec, part + 1, high);
		}
	public:
		void sort(std::vector<int>& vec) {
			this->quicksort(vec, 0, vec.size() - 1);
		}
};

int main() {

	std::ifstream file("input.data");
	ListParser parser = ListParser();
	Sorter sorter = Sorter();

	if (!file.is_open()) {
		std::cerr << "Could not open file \"input.data\"" << std::endl;
		return (1);
	}

	auto lists = parser.parse_list(file);
	std::vector<int> left = lists.first;
	std::vector<int> right = lists.second;

	sorter.sort(left);
	sorter.sort(right);

	int total_distance = 0;
	for (int i = 0; i < left.size(); i++) {
		int diff = left[i] - right[i];
		if (diff < 0)
			diff = -diff;

		total_distance += diff;
	}

	std::cout << "Total Distance: " << total_distance << std::endl;


}
