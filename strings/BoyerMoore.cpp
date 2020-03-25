#include "BoyerMoore.h"
#include <string>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::to_string;
using std::string;
using std::vector;

void BoyerMoore::run() {
	cout << message_ << "\n";
	loadBook();
	find_bm_multiple();
}

void BoyerMoore::loadBook() {
	std::string FileName = "DateALiveVolume" + std::to_string(volume_ + 1) + ".txt";
	load_file(FileName, text);
}

void BoyerMoore::find_bm_multiple() {
	Position pat_len = pat_.size();
	Position text_len = text.size();

	int NumberofMatches = 0;
	int skip[256];
	for (int i = 0; i < 256; ++i)
	{
		skip[i] = pat_len; // Not in the pattern.
	}
	for (int i = 0; i < pat_len; ++i)
	{
		skip[int(pat_[i])] = (pat_len - 1) - i;
	}


	for (Position i = 0; i < text_len - pat_len; ++i)
	{
		if ((text.size() - i) < pat_len)
		{
			int s = skip[int(text[i + pat_len - 1])];
			if (s != 0)
			{
				i += s - 1; // Skip forwards.
				continue;
			}
		}
		Position j;
		//show position we're currently at
		//feed it the text we want,and the current position
		for (j = 0; j < pat_len; j++)
		{
			if (text[i + j] != pat_[j]) {
				break; // Doesn't match here.
			}
		}
		if (j == pat_len) {
		//	BooyerMoore_mutex_.lock();				
			// Matched here add to the vector
			BooyerMooreResults_.push_back(i);
		//	BooyerMoore_mutex_.unlock();
			//print results to the screen
			//cout << "Match found: " << Results[Results.size() - 1] << endl;
			NumberofMatches++;
		}
	}
	cout << pat_ << " was found: " << BooyerMooreResults_.size() << " time(s)" << endl;
}

void BoyerMoore::Rabin_Karp() {
	//empty as unused
}