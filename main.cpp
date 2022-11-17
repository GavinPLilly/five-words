#include <thread>
#include <array>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>    // std::min

using namespace std;

using l_field = uint32_t;

void calc(vector<l_field>& l_fields, vector<string>& words, int start, int length) {
	l_field a_word;
	l_field ab_word;
	l_field abc_word;
	l_field abcd_word;
	l_field acc = 0;

	for(int a = start; a < min(start + length, static_cast<int>(l_fields.size() - 4)); ++a) {
		acc = l_fields[a];
		if(__builtin_popcount(acc) != 5) {
			continue;
		}
		a_word = acc;
		for(int b = a + 1; b < l_fields.size() - 3; ++b) {
			acc = a_word | l_fields[b];
			if(__builtin_popcount(acc) != 10) {
				continue;
			}
			ab_word = acc;
			for(int c = b + 1; c < l_fields.size() - 2; ++c) {
				acc = ab_word | l_fields[c];
				if(__builtin_popcount(acc) != 15) {
					continue;
				}
				abc_word = acc;
				for(int d = c + 1; d < l_fields.size() - 1; ++d) {
					acc = abc_word | l_fields[d];
					if(__builtin_popcount(acc) != 20) {
						continue;
					}
					abcd_word = acc;
					for(int e = d + 1; e < l_fields.size(); ++e) {
						acc = abcd_word | l_fields[e];
						if(__builtin_popcount(acc) != 25) {
							continue;
						}
						cout << "----------FOUND----------" << endl;
						cout << words[a] << endl;
						cout << words[b] << endl;
						cout << words[c] << endl;
						cout << words[d] << endl;
						cout << words[e] << endl;
						cout << "-------------------------" << endl;
					}
				}
			}
		}
	}
}

int main()
{
	vector<l_field> l_fields;
	vector<string> words;
	ifstream file("five_letter_words_alpha.txt");
	// ifstream file("test.txt");

	string line;
	uint32_t cur_l_field;
	l_field cur;
	while(getline(file, line)) {
		cur = 0;
		cur |= 1 << (line[0] - 'a')
			| 1 << (line[1] - 'a')
			| 1 << (line[2] - 'a')
			| 1 << (line[3] - 'a')
			| 1 << (line[4] - 'a');
		if(__builtin_popcount(cur) == 5) {
			l_fields.push_back(cur);
			words.push_back(line);
		}
	}

	const int NUM_THREADS = 32;
	int block_size = l_fields.size() / NUM_THREADS;
	// int block_size = 1;
	vector<thread> threads;
	for(int i = 0; i < NUM_THREADS; ++i) {
		threads.push_back(thread(calc, ref(l_fields), ref(words), i * block_size, block_size));
	}
	for(auto& th : threads) {
		th.join();
	}

	return 0;
}
