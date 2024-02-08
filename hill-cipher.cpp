#include <iostream>
#include <vector>

std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

int getDet(int** matrix, int len_of_matrix) {
	if (len_of_matrix == 1) {
		return matrix[0][0];
	}
	else if (len_of_matrix == 2) {
		return matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
	}
	else {
		int det = 0;
		for (int k = 0; k < len_of_matrix; ++k) {
			int** new_matrix = new int* [len_of_matrix - 1];
		
			for (int i = 0; i < len_of_matrix - 1; ++i) {
				new_matrix[i] = new int[len_of_matrix - 1];
			}

			for (int i = 1; i < len_of_matrix; ++i) {
				for (int j = 0; j < len_of_matrix; ++j) {
					if (j == k) {
						continue;
					}
					else if (j < k) {
						new_matrix[i - 1][j] = matrix[i][j];
					}
					else {
						new_matrix[i - 1][j - 1] = matrix[i][j];
					}
				}
			}

			det += pow(-1, k + 2) * matrix[0][k] * getDet(new_matrix, len_of_matrix - 1);
		}

		return det;
	}
}

int letter_to_key(char letter) {
	for (int i = 0; i < alphabet.length(); ++i) {
		if (alphabet[i] == letter) {
			return i;
		}
	}

	return -1;
}

int find_nod(int a, int b) {
	while (a != b) {
		if (a > b) {
			a -= b;
		}
		else {
			b -= a;
		}
	}
	return a;
}

int findAlgebricComplement(int** matrix, int len_of_matrix, int i, int j) {
	int** new_matrix = new int* [len_of_matrix];

	for (int k = 0; k < len_of_matrix - 1; k++) {
		new_matrix[k] = new int[len_of_matrix - 1];
	}

	for (int k = 0; k < len_of_matrix; k++) {
		for (int h = 0; h < len_of_matrix; h++) {
			if (i == k || j == h) {
				continue;
			}
			else {
				if (k < i && h < j) {
					new_matrix[k][h] = matrix[k][h];
				}
				else if (k < i && h > j) {
					new_matrix[k][h - 1] = matrix[k][h];
				}
				else if (k > i && h < j) {
					new_matrix[k - 1][h] = matrix[k][h];
				}
				else {
					new_matrix[k - 1][h - 1] = matrix[k][h];
				}
			}
		}
	}

	return getDet(new_matrix, len_of_matrix - 1) * ((-1) ^ (i + j));
}

int main() {
	std::string plain_text;
	int len_of_matrix = 0;

	std::cout << "Enter new text: ";
	std::cin >> plain_text;

	for (auto& c : plain_text) c = toupper(c);
	for (auto& c : alphabet) c = toupper(c);

	std::cout << "Enter length of key: ";
	std::cin >> len_of_matrix;

	int left = plain_text.length() % len_of_matrix;
	if (left != 0) {
		for (int i = 0; i < len_of_matrix - left; ++i) {
			plain_text = plain_text + "A";
		}
	}

	int** matrix = new int* [len_of_matrix];

	std::cout << "Enter new matrix: " << '\n';

	for (int i = 0; i < len_of_matrix; i++) {
		matrix[i] = new int[len_of_matrix];
	}

	for (int i = 0; i < len_of_matrix; i++) {
		for (int j = 0; j < len_of_matrix; j++) {
			std::cin >> matrix[i][j];
		}
	}

	int det = getDet(matrix, len_of_matrix);

	while (det == 0) {
		std::cout << "Determinant equals zero. Please enter matrix again: " << '\n';

		for (int i = 0; i < len_of_matrix; i++) {
			for (int j = 0; j < len_of_matrix; j++) {
				std::cin >> matrix[i][j];
			}
		}

		det = getDet(matrix, len_of_matrix);
	}

	int nod = find_nod(abs(det), alphabet.length());

	while (nod != 1) {
		std::cout << "NOD of determinant and alphabet length does not equal 1. Please enter matrix again: " << '\n';

		for (int i = 0; i < len_of_matrix; i++) {
			for (int j = 0; j < len_of_matrix; j++) {
				std::cin >> matrix[i][j];
			}
		}

		det = getDet(matrix, len_of_matrix);

		nod = find_nod(abs(det), alphabet.length());
	}
		
	// Encoding

	std::string encoded_text = "";

	std::vector<std::string> blocks;

	std::string block = "";

	for (int k = 0; k < plain_text.length(); k++) {
		block += plain_text[k];

		if (k % len_of_matrix == 2) {
			blocks.push_back(block);
			block = "";
		}
	}

	// 1	1 2 3	x
	// 2 *	4 5 6 = y
	// 3    7 8 9	z

	int* numbers = new int [len_of_matrix];

	for (int i = 0; i < blocks.size(); i++) {
		block = blocks[i];
		int sum = 0;

		for (int j = 0; j < block.length(); j++) {
			for (int k = 0; k < block.length(); k++) {
				sum += letter_to_key(block[k]) * matrix[k][j];
			}

			numbers[j] = sum;
			encoded_text = encoded_text + alphabet[sum % alphabet.length()];
			sum = 0;
		}
	}

	// ------------------------------
	// Decoding

	std::string decoded_text = "";

	int** invert_matrix = new int* [len_of_matrix];

	for (int i = 0; i < len_of_matrix; i++) {
		invert_matrix[i] = new int[len_of_matrix];
	}

	//std::cout << (6 + (-40) % 6);

	for (int i = 0; i < len_of_matrix; i++) {
		for (int j = 0; j < len_of_matrix; j++) {
			std::cout << invert_matrix[i][j];
		}

		std::cout << '\n';
	}

	for (int i = 0; i < len_of_matrix; i++) {
		for (int j = 0; j < len_of_matrix; j++) {
			invert_matrix[i][j] = (findAlgebricComplement(matrix, len_of_matrix, i, j)) / det;
		}
	}

	for (int i = 0; i < blocks.size(); i++) {
		int raw = 0;
		for (int j = 0; j < blocks[i].length(); j++) {
			decoded_text = decoded_text + alphabet[(letter_to_key(blocks[i][j]) * invert_matrix[raw][j] % alphabet.length())];
		}
		raw += 1;
	}

	std::cout << "Decoded text: " << decoded_text;
	return 0;
}