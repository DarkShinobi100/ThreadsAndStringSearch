#include "messagetask.h"
#include <string>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::to_string;
using std::string;
using std::vector;

void MessageTask::run() {
	cout << message_ << "\n";
	loadBook();
	find_bm_multiple();
}

void MessageTask::loadBook() {
	std::string FileName = "DateALiveVolume" + std::to_string(volume_ + 1) + ".txt";
	load_file(FileName, text);
}

void MessageTask::find_bm_multiple() {
	Position pat_len = pat_.size();
	Position text_len = text.size();
	vector<Position> Results;
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
			// Matched here add to the vector
			Results.push_back(i);
			//print results to the screen
			//cout << "Match found: " << Results[Results.size() - 1] << endl;
			NumberofMatches++;
		}
	}
	cout << pat_ << " was found: " << NumberofMatches << " time(s)" << endl;
}

void MessageTask::Rabin_Karp() {
	int PatternLength = pat_.size();
	int TextLength = text.size();
	int PrimeNumber = 2;
	int j = 0; //counter
	int PatternHash = 0;
	int TextHash = 0;
	int Hash = 1;
	int NumberofMatches = 0;

	for (int i = 0; i < PatternLength - 1; i++)
	{
		Hash = (Hash * d) % PrimeNumber;
	}

	for (int i = 0; i < PatternLength; i++)
	{
		PatternHash = (d * PatternHash + pat_[i]) % PrimeNumber;
		TextHash = (d * TextHash + pat_[i]) % PrimeNumber;
	}

	for (int i = 0; i <= TextLength - PatternLength; i++)
	{
		if (PatternHash == TextHash)
		{
			for (j = 0; j < PatternLength; j++)
			{
				if (text[i + j] != pat_[j])
				{
					break;
				}
			}
			if (j == PatternLength)
			{
				//cout << "Pattern found at index: " << i << endl;
				NumberofMatches++;
			}
		}
		if (i < TextLength - PatternLength)
		{
			TextHash = (d * (TextHash - text[i] * Hash) + text[i + PatternLength]) % PrimeNumber;
			if (TextHash < 0)
			{
				TextHash = TextHash + PrimeNumber;
			}
		}
	}
	cout << pat_ << " was found: " << NumberofMatches << " time(s)" << endl;
}