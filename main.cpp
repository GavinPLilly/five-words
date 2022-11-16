#include <thread>
#include <array>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>    // std::min

using namespace std;

int NUM_WORDS;
vector<array<char, 5>> words;

void print_word(array<char, 5> word) {
	cout << word[0] << word[1] << word[2] << word[3] << word[4] << endl;
}


inline int fill_letters(uint32_t letters, array<char, 5> s)
{
	letters |= (1 << (s[0] - 'a'));
	letters |= (1 << (s[1] - 'a'));
	letters |= (1 << (s[2] - 'a'));
	letters |= (1 << (s[3] - 'a'));
	letters |= (1 << (s[4] - 'a'));
	return letters;
}

void calc(int thread_num, int start, int length) {
	// cout << string{"|"} + to_string(start) + string{" "} + to_string(length) + string{" |\n"};
	bool found = false;

	uint32_t copy1;
	uint32_t copy2;
	uint32_t copy3;
	uint32_t copy4;
	uint32_t copy5;

	int a;
	int b;
	int c;
	int d;
	int e;
	uint32_t letters = 0;
	copy1 = letters;
	for(a = start; a < min(start + length, NUM_WORDS - 4); ++a) {
		letters = fill_letters(copy1, words[a]);
		if(a == 4306) {
			cout << "Found 'dwarf'" << endl;
			cout << __builtin_popcount(letters) << endl;
		}
		if(__builtin_popcount(letters) != 5) {
			continue;
		}
		copy2 = letters;
		for(b = a + 1; b < NUM_WORDS - 3; ++b) {
			letters = fill_letters(copy2, words[b]);
			if(a == 4306 && b == 5781) {
				cout << "Found 'dwarf', 'glyph'" << endl;
			}
			if(__builtin_popcount(letters) != 10) {
				continue;
			}
			copy3 = letters;
			for(c = b + 1; c < NUM_WORDS - 2; ++c) {
				letters = fill_letters(copy3, words[c]);
				if(__builtin_popcount(letters) != 15) {
					continue;
				}
				copy4 = letters;
				for(d = c + 1; d < NUM_WORDS - 1; ++d) {
					letters = fill_letters(copy4, words[d]);
					if(__builtin_popcount(letters) != 20) {
						continue;
					}
					copy5 = letters;
					for(e = d + 1; e < NUM_WORDS; ++e) {
						letters = fill_letters(copy5, words[e]);
						if(__builtin_popcount(letters) != 25) {
							continue;
						}
						found = true;
						goto done;
					}
				}
			}
		}
	}
done:
	if(found) {
		cout << "FOUND" << endl;
		cout << "a: " << a << "\tb: " << b << "\tc: " << c << "\td: " << d << "\te: " << e << endl;
		print_word(words[a]);
		print_word(words[b]);
		print_word(words[c]);
		print_word(words[d]);
		print_word(words[e]);
	}
}

int main()
{
	// n^5 algorithm
	ifstream file("five_letter_words_alpha.txt");
	// ifstream file("test.txt");

	string line;
	uint32_t tmp_letters;
	int i = 0;
	while(getline(file, line)) {
		tmp_letters = 0;
		array<char, 5> tmp_array = {line[0], line[1], line[2], line[3], line[4]};
		tmp_letters = fill_letters(tmp_letters, tmp_array);
		if(__builtin_popcount(tmp_letters) == 5) {
			// words.push_back({line[0], line[1], line[2], line[3], line[4]});
			words.push_back(tmp_array);
		}
		++i;
	}
	cout << "Loaded " << words.size() << " words" << endl;
	NUM_WORDS = words.size();

	const int NUM_THREADS = NUM_WORDS;
	int thread_num = 0;
	// int block_size = NUM_WORDS / NUM_THREADS;
	int block_size = 1;
	vector<thread> threads;
	for(int i = 0; i < NUM_THREADS; ++i) {
	     threads.push_back(thread(calc, i, i * block_size, block_size));
	}
	for(auto& th : threads) {
		th.join();
	}

	// calc(0, 4306, words.size());
}
