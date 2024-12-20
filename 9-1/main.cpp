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

std::vector<int> expand_diskmap(const std::string diskmap) {
	std::vector<int> disk;

	int id = 0;

	for (size_t i = 0; i < diskmap.size(); i++) {
		if (i % 2 == 0) { // data
			int val = diskmap.at(i) - '0';
			for (int j = 0; j < val; j++) {
				disk.push_back(id);
			}
			id++;
		}
		else { // empty
			int val = diskmap.at(i) - '0';
			for (int j = 0; j < val; j++) {
				disk.push_back(-1);
			}
		}
	}

	return (disk);
}

void print_data(const std::vector<int> data) {
	for (size_t i = 0; i < data.size(); i++) {
		if (data.at(i) == -1) {
			std::cout << '.';
		}
		else {
			std::cout << data.at(i);
		}
		std::cout << " ";
	}
}

void compact_disk_data(std::vector<int>& disk_data) {
	size_t empty_index = 0;
	size_t last_index = disk_data.size() - 1;

	if (debug_mode) {
		print_data(disk_data);
		std::cout << std::endl;
	}

	while (empty_index < last_index) {
		while (last_index >= 0 && disk_data[last_index] == -1) { // Find last data value
			last_index--;
		}
		while (empty_index < disk_data.size() && disk_data[empty_index] != -1) { // Find first empty value
			empty_index++;
		}
		if (last_index < 0 || empty_index >= disk_data.size() || empty_index >= last_index) {
			break;
		}

		disk_data.at(empty_index) = disk_data.at(last_index);
		disk_data.at(last_index) = -1;
	
		if (debug_mode) {
			print_data(disk_data);
			std::cout << std::endl;
		}
	}
}

long long calculate_checksum(const std::vector<int> data) {
	long long checksum = 0;

	for (size_t i = 0; i < data.size(); i++) {
		if (data.at(i) == -1) {
			break;
		}
		checksum += i * data.at(i);
	}

	return (checksum);
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
	
	std::string diskmap = lines.at(0);
	std::vector<int> disk_data = expand_diskmap(diskmap);
	compact_disk_data(disk_data);
	long long checksum = calculate_checksum(disk_data);

	std::cout << "Checksum: " << checksum << std::endl;
}
