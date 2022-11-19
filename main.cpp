#include <thread>
#include <array>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>    // std::min
#include <utility>

using namespace std;

using l_field = uint32_t;

void print_ranges(vector<pair<int, int>> ranges) {
	cout << "{\n";
	for(auto r : ranges) {
		cout << "\t(" << r.first << ", " << r.second << "),\n";
	}
	cout << "}" << endl;
}

// Returns the index of the l_fields, where the letter after the
inline vector<pair<int, int>> get_ranges(vector<int>& l_starts, l_field acc, char first_letter='a') {
	vector<pair<int, int>> result;
	int i = 0;
	acc = ~acc;
	acc &= 0x3FFFFFF;
	acc >>= first_letter - 'a';
	i += first_letter - 'a';
	while(acc != 0) {
		if(acc & 1) {
			result.push_back({l_starts[i], l_starts[i + 1]});
		}
		acc >>= 1;
		++i;
	}
	return result;
}

void calc(vector<l_field>& l_fields, vector<string>& words, vector<int>& l_starts, int start, int length) {
	l_field a_word;
	l_field ab_word;
	l_field abc_word;
	l_field abcd_word;
	l_field acc = 0;

	// print_ranges(get_ranges(l_starts, acc));
	for(int a = start; a < start + length; ++a) {
		acc = l_fields[a];
		a_word = l_fields[a];
		for(auto b_range : get_ranges(l_starts, acc, words[a][0])) {
			for(int b = b_range.first; b < b_range.second; ++b) {
				acc = a_word | l_fields[b];
				if(__builtin_popcount(acc) != 10) {
					continue;
				}
				ab_word = acc;
				for(auto c_range : get_ranges(l_starts, acc, words[b][0])) {
					for(int c = c_range.first; c < c_range.second; ++c) {
						acc = ab_word | l_fields[c];
						if(__builtin_popcount(acc) != 15) {
							continue;
						}
						abc_word = acc;
						for(auto d_range : get_ranges(l_starts, acc, words[c][0])) {
							for(int d = d_range.first; d < d_range.second; ++d) {
								acc = abc_word | l_fields[d];
								if(__builtin_popcount(acc) != 20) {
									continue;
								}
								abcd_word = acc;
								for(auto e_range : get_ranges(l_starts, acc, words[d][0])) {
									for(int e = e_range.first; e < e_range.second; ++e) {
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
			}
		}
	}
}

int main()
{
	vector<l_field> l_fields;
	vector<string> words;
	vector<int> l_starts;
	ifstream file("sorted_five_letter_words.txt");
	// ifstream file("test.txt");

	string line;
	uint32_t cur_l_field;
	l_field cur;
	char cur_letter = 0;
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
			if(line[0] != cur_letter) {
				// TODO: Adjust l_starts
				l_starts.push_back(l_fields.size() - 1);
				cur_letter = line[0];
			}
		}
	}
	l_starts.push_back(l_fields.size());

#ifdef THREAD
	const int NUM_THREADS = 32;
	int block_size = l_fields.size() / NUM_THREADS;
	// int block_size = 1;
	vector<thread> threads;
	for(int i = 0; i < NUM_THREADS; ++i) {
		threads.push_back(thread(calc, ref(l_fields), ref(words), ref(l_starts), i * block_size, block_size));
	}
	for(auto& th : threads) {
		th.join();
	}
#else
	// cout << "{\n";
	// for(auto e : l_starts) {
	// 	cout << "\t(" << e << "),\n";
	// }
	// cout << "}" << endl;
	// cout << l_starts.size() << endl;
	calc(l_fields, words, l_starts, 0, words.size());
#endif

	return 0;
}
