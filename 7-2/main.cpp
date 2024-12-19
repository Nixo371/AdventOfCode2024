#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>

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

std::pair<long long, std::vector<int>> parse_line(std::string line) {
	if (line.empty()) {
		return {-1, {}};
	}

	size_t colon = line.find(':');
	std::string result = line.substr(0, colon);
	std::string numbers = line.substr(colon + 2);

	long long big_boi = 0;
	for (char c : result) {
		big_boi = (big_boi * 10) + (c - '0');
	}
	
	std::vector<int> values;
	int num = 0;
	for (char c : numbers) {
		if (c == ' ') {
			values.push_back(num);
			num = 0;
		}
		else {
			num = (num * 10) + (c - '0');
		}
	}
	values.push_back(num);

	return (std::pair<long long, std::vector<int>>(big_boi, values));
}

enum Op {
	NONE,
	ADD,
	MUL,
	UNION
};

int num_digits(int num) {
	int size = std::to_string(num).size();
	long long factor = 1;
	while (size--) {
		factor *= 10;
	}

	return (factor);
}

long long calculate_value(const std::vector<int> numbers, const std::vector<Op> operations) {
	long long result = numbers[0];
	for (size_t i = 0; i < operations.size(); i++) {
		switch (operations[i]) {
			case ADD:
				result += numbers[i + 1];
				break;
			case MUL:
				result *= numbers[i + 1];
				break;
			case UNION:
				result = (result * num_digits(numbers[i + 1])) + numbers[i + 1];
				break;
			case NONE:
				std::cerr << "Missing operation!!!" << std::endl;
				break;
		}
	}
	return (result);
}

long long calculate_solution(long long result, size_t index, const std::vector<int>& numbers, std::vector<Op>& operations) {

	// Check the last element and see if it is a factor of the total
	// If it isn't, we know it must be addition (if there is a solution)
	// If it is, we can't check anything else so we must now start checking all possible combinations

	if (index == operations.size()) {
		long long res = calculate_value(numbers, operations);
		/*
		if (debug_mode) {
			std::cout << "Result: " << result << std::endl;
			std::cout << "Res: " << res << std::endl;

			std::cout << "Numbers: ";
			Visualizer::print(numbers);
			std::cout << std::endl;

			std::cout << "Operations: ";
			Visualizer::print(operations);
			std::cout << std::endl;

			std::cout << std::endl;
		}
		*/

		if (res == result) { // success
			return (result);
		}
		return (0);
	}

	operations[index] = MUL;
	long long multiplying = calculate_solution(result, index + 1, numbers, operations);
	
	if (multiplying == result) {
		return multiplying;
	}

	operations[index] = UNION;
	long long join = calculate_solution(result, index + 1, numbers, operations);

	if (join == result) {
		return join;
	}

	operations[index] = ADD;
	long long adding = calculate_solution(result, index + 1, numbers, operations);

	if (adding == result) {
		return (adding);
	}

	return (0);
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

	long long total = 0;

	std::vector<int> correct = {0, 1, 3, 5, 6, 7, 8, 9, 11, 13, 15, 18, 19, 20, 22, 23, 24, 26, 28, 30, 31, 32, 34, 35, 36, 38, 39, 41, 43, 44, 45, 46, 47, 48, 53, 54, 55, 56, 58, 59, 60, 61, 62, 64, 67, 68, 69, 71, 72, 74, 75, 76, 78, 79, 80, 82, 83, 85, 86, 88, 89, 91, 92, 95, 97, 98, 100, 102, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 120, 121, 122, 123, 124, 125, 127, 128, 133, 137, 138, 139, 140, 141, 142, 143, 144, 145, 147, 148, 149, 150, 151, 152, 153, 155, 156, 159, 160, 161, 162, 164, 166, 167, 168, 169, 170, 172, 173, 175, 176, 177, 178, 179, 182, 183, 186, 189, 190, 191, 192, 193, 195, 197, 198, 199, 201, 202, 203, 204, 205, 206, 210, 212, 213, 214, 215, 216, 217, 218, 220, 221, 222, 223, 224, 225, 226, 228, 229, 230, 231, 232, 233, 237, 239, 240, 241, 242, 243, 245, 246, 248, 249, 253, 254, 257, 258, 260, 261, 263, 264, 265, 266, 267, 268, 269, 272, 273, 277, 278, 279, 280, 282, 283, 284, 285, 287, 288, 289, 290, 291, 292, 294, 295, 297, 298, 299, 300, 301, 302, 303, 305, 306, 308, 310, 313, 314, 317, 320, 323, 324, 325, 326, 329, 332, 333, 334, 335, 336, 339, 340, 341, 344, 346, 347, 349, 350, 352, 353, 355, 357, 358, 359, 360, 361, 362, 363, 365, 366, 367, 368, 369, 370, 372, 375, 376, 377, 378, 379, 382, 384, 385, 386, 387, 388, 394, 395, 396, 397, 398, 399, 401, 402, 403, 405, 406, 407, 408, 409, 410, 413, 414, 417, 418, 419, 420, 421, 423, 424, 425, 426, 427, 430, 432, 433, 435, 436, 438, 439, 441, 442, 443, 445, 446, 447, 448, 449, 450, 451, 452, 454, 455, 456, 457, 458, 459, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 475, 476, 477, 478, 479, 480, 481, 482, 483, 484, 485, 487, 491, 493, 494, 495, 497, 498, 500, 501, 502, 503, 504, 505, 506, 507, 509, 510, 511, 512, 513, 514, 515, 518, 519, 520, 527, 529, 531, 532, 533, 534, 535, 536, 538, 539, 541, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 562, 564, 567, 568, 570, 571, 572, 573, 575, 576, 577, 578, 579, 580, 582, 583, 584, 586, 589, 591, 593, 594, 595, 596, 597, 598, 600, 601, 602, 603, 605, 608, 609, 610, 611, 612, 615, 618, 621, 622, 623, 625, 627, 628, 629, 630, 632, 633, 635, 636, 638, 639, 640, 641, 642, 643, 644, 645, 646, 647, 648, 649, 651, 652, 653, 654, 655, 657, 658, 660, 661, 663, 664, 665, 666, 667, 668, 669, 670, 671, 672, 673, 674, 677, 678, 679, 680, 682, 683, 685, 686, 687, 688, 689, 691, 694, 695, 696, 697, 698, 699, 703, 704, 706, 707, 709, 710, 711, 712, 713, 714, 715, 716, 717, 718, 720, 721, 724, 727, 728, 729, 730, 731, 732, 733, 736, 737, 738, 739, 740, 741, 742, 743, 744, 745, 747, 748, 749, 751, 755, 756, 757, 758, 760, 761, 762, 764, 765, 766, 767, 769, 770, 773, 776, 777, 779, 781, 784, 785, 787, 788, 789, 790, 791, 792, 793, 795, 798, 799, 800, 802, 803, 804, 805, 806, 807, 808, 809, 810, 811, 812, 813, 814, 816, 818, 819, 821, 823, 824, 825, 830, 831, 834, 835, 836, 838, 846, 848};

	for (size_t i = 0; i < lines.size(); i++) {
		std::string line = lines.at(i);
		auto data = parse_line(line);
		long long result = data.first;
		std::vector<int> numbers = data.second;
		std::vector<Op> operations(numbers.size() - 1, NONE);

		long long solution = calculate_solution(result, 0, numbers, operations);
		
		if (debug_mode) {
			if (std::find(correct.begin(), correct.end(), i) != correct.end()) {
				std::cout << "**";
			}
			if (solution == 0) {
				std::cout << "ERROR " << line << std::endl;
			}
			else {
				std::cout << "GREAT " << line << std::endl;
			}
		}

		total += solution;
	}

	std::cout << "Total: " << total << std::endl;
	
}
