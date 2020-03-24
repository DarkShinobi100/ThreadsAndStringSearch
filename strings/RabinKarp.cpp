#include "RabinKarp.h"
#include <string>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::to_string;
using std::string;
using std::vector;

void RabinKarp::run() {
	cout << message_ << "\n";
	loadBook();
	Rabin_Karp();
}

void RabinKarp::loadBook() {
	std::string FileName = "DateALiveVolume" + std::to_string(volume_ + 1) + ".txt";
	load_file(FileName, text);
}

void RabinKarp::find_bm_multiple() {
	//empty as unused
}

void RabinKarp::Rabin_Karp() {
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