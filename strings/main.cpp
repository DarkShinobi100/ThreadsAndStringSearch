// String searching lab exercise.
// Edwyn Mckie

#include <chrono>
#include <cassert>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <fstream> // needed for file
#include "utils.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;
using std::ofstream;

//define the alias for the clock type we're going to use
typedef std::chrono::steady_clock the_clock;

//my file to save values to
ofstream my_file("Timings.csv");
#define d 256 //number of characters in the english alphabet

/** Return first position of pat in text, or -1 if not found. */
Position find_bruteforce(const string& pat, const string& text) {
	Position pat_len = pat.size();
	Position text_len = text.size();

	for (Position i = 0; i < text_len - pat_len; ++i) {
		Position j;
		//show position we're currently at
		//feed it the text we want,and the current position
		show_context(text, i); 
		for (j = 0; j < pat_len; j++) {
			if (text[i + j] != pat[j]) {
				break; // Doesn't match here.
			}
		}
		if (j == pat_len) {
			return i; // Matched here.
		}
	}
	return -1; // Not found.
}

/** skip ahead by the length of the string if we do not match. */
Position find_skipping(const string& pat, const string& text) {
	Position pat_len = pat.size();
	Position text_len = text.size();

	bool in_pattern[256];
	for (int i = 0; i < 256; ++i)
	{
		in_pattern[i] = false;
	}
	
	for (char c : pat)
	{
		in_pattern[int(c)] = true;
	}

	for (Position i = 0; i < text_len - pat_len; ++i) {
		if (!in_pattern[int(text[i + pat_len - 1])])
		{
			i += pat_len - 1; //skip forwards
			continue;
		}
		
		Position j;
		//show position we're currently at
		//feed it the text we want,and the current position
		show_context(text, i);
		for (j = 0; j < pat_len; j++) {
			if (text[i + j] != pat[j]) {
				break; // Doesn't match here.
			}
		}
		if (j == pat_len) {
			return i; // Matched here.
		}
	}
	return -1; // Not found.
}

/** skip ahead by the length of the string if we do not match. */
Position find_bm(const string& pat, const string& text) {
	Position pat_len = pat.size();
	Position text_len = text.size();

	int skip[256];
	for (int i = 0; i < 256; ++i)
	{
		skip[i] = pat_len; // Not in the pattern.
	}
	for (int i = 0; i < pat_len; ++i)
	{
		skip[int(pat[i])] = (pat_len - 1) - i;
	}


	for (Position i = 0; i < text_len - pat_len; ++i) {
		int s = skip[int(text[i + pat_len - 1])]; 
		if (s != 0) 
		{
			i += s - 1; // Skip forwards.
			continue;
		}
		Position j;
		//show position we're currently at
		//feed it the text we want,and the current position
		show_context(text, i);
		for (j = 0; j < pat_len; j++) {
			if (text[i + j] != pat[j]) {
				break; // Doesn't match here.
			}
		}
		if (j == pat_len) {
			return i; // Matched here.
		}
	}
	return -1; // Not found.
}

/* skip ahead by the length of the string if we do not match.but keep looping through the whole file */
void find_bm_multiple(const string& pat, const string& text) 
{
	Position pat_len = pat.size();
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
		skip[int(pat[i])] = (pat_len - 1) - i;
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
			if (text[i + j] != pat[j]) {
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
	cout << pat << " was found: " << NumberofMatches << " time(s)" << endl;
}

/** read the patern and calculate a value for it, read the next cunk of the file we are searching and compare the two. slide along by 1 and calculate again */
void Rabin_Karp(const string& pat, const string& text) 
{
	int PatternLength = pat.size();
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
		PatternHash = (d * PatternHash + pat[i]) % PrimeNumber;
		TextHash = (d * TextHash + pat[i]) % PrimeNumber;
	}

	for (int i = 0; i <= TextLength - PatternLength; i++)
	{
		if (PatternHash == TextHash)
		{
			for (j = 0; j < PatternLength; j++)
			{
				if (text[i + j] != pat[j])
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
	cout << pat << " was found: " << NumberofMatches << " time(s)" << endl;
}

int main(int argc, char *argv[]) {
	string text;//declare text as a string
	float time_taken[2];
	string FileName = "";

	string pat[100]; //pat = pattern we are looking for
	pat[0] ="244";
	pat[1] = "244";
	pat[2] = "244";
	pat[3] = "244";
	pat[4] = "244";
	pat[5] = "244";
	pat[6] = "244";
	pat[7] = "244";
	pat[8] = "244";
	pat[9] = "244"; 
	// repeated these 10 values to get a consistent average for each one
	pat[10] = "Tohka";
	pat[11] = "Tohka";
	pat[12] = "Tohka";
	pat[13] = "Tohka";
	pat[14] = "Tohka";
	pat[15] = "Tohka";
	pat[16] = "Tohka";
	pat[17] = "Tohka";
	pat[18] = "Tohka";
	pat[19] = "Tohka";
	pat[20] = "Origami";
	pat[21] = "Origami";
	pat[22] = "Origami";
	pat[23] = "Origami";
	pat[24] = "Origami";
	pat[25] = "Origami";
	pat[26] = "Origami";
	pat[27] = "Origami";
	pat[28] = "Origami";
	pat[29] = "Origami";
	pat[30] = "Yoshino";
	pat[31] = "Yoshino";
	pat[32] = "Yoshino";
	pat[33] = "Yoshino";
	pat[34] = "Yoshino";
	pat[35] = "Yoshino";
	pat[36] = "Yoshino";
	pat[37] = "Yoshino";
	pat[38] = "Yoshino";
	pat[39] = "Yoshino";
	pat[40] = "Shido";
	pat[41] = "Shido";
	pat[42] = "Shido";
	pat[43] = "Shido";
	pat[44] = "Shido";
	pat[45] = "Shido";
	pat[46] = "Shido";
	pat[47] = "Shido";
	pat[48] = "Shido";
	pat[49] = "Shido";
	pat[50] = "Spirit";
	pat[51] = "Spirit";
	pat[52] = "Spirit";
	pat[53] = "Spirit";
	pat[54] = "Spirit";
	pat[55] = "Spirit";
	pat[56] = "Spirit";
	pat[57] = "Spirit";
	pat[58] = "Spirit";
	pat[59] = "Spirit";
	pat[60] = "DEM";
	pat[61] = "DEM";
	pat[62] = "DEM";
	pat[63] = "DEM";
	pat[64] = "DEM";
	pat[65] = "DEM";
	pat[66] = "DEM";
	pat[67] = "DEM";
	pat[68] = "DEM";
	pat[69] = "DEM";
	pat[70] = "Angel";
	pat[71] = "Angel";
	pat[72] = "Angel";
	pat[73] = "Angel";
	pat[74] = "Angel";
	pat[75] = "Angel";
	pat[76] = "Angel";
	pat[77] = "Angel";
	pat[78] = "Angel";
	pat[79] = "Angel";
	pat[80] = "Battle";
	pat[81] = "Battle";
	pat[82] = "Battle";
	pat[83] = "Battle";
	pat[84] = "Battle";
	pat[85] = "Battle";
	pat[86] = "Battle";
	pat[87] = "Battle";
	pat[88] = "Battle";
	pat[89] = "Battle";
	pat[90] = "Battle";
	pat[91] = "blade";
	pat[92] = "blade";
	pat[93] = "blade";
	pat[94] = "blade";
	pat[95] = "blade";
	pat[96] = "blade";
	pat[97] = "blade";
	pat[98] = "blade";
	pat[99] = "blade";
	
	//set up headers
	my_file << "Character limit " << "," << "Boyer Moore Time taken"  << "," << "Rabin Karp Time taken" << endl;
	for (int i = 0; i < 17; i++)
	{	//load_jute_book(text); //call the load function and pass it the file .txt
		FileName ="DateALiveVolume" + std::to_string(i + 1) + ".txt";
		load_file(FileName, text);
		cout << "String size: " << text.size() << endl;
		for (int j = 0; j <100; j++)
		{
			//Position pos = find_bruteforce(pat, text);
			//Position pos = find_skipping(pat, text);
			//Position pos = find_bm(pat, text);
			//cout << "Searching for:" << pat << endl;
			cout << "Boyer Moore" << endl;

			//time how long it takes to Search via Boyer Moore
			the_clock::time_point start = the_clock::now();
			find_bm_multiple(pat[j], text);
			the_clock::time_point end = the_clock::now();
			time_taken[0] = duration_cast<milliseconds>(end - start).count();

			//print the time taken
			cout << "time taken to Search " << time_taken[0] << "ms"<< endl;
		//	system("pause");

			cout << "Rabin Karp" << endl;
			// time how long it takes to Search via Rabin karp
			start = the_clock::now();
			Rabin_Karp(pat[j], text);
			end = the_clock::now();
			time_taken[1] = duration_cast<milliseconds>(end - start).count();
			//print the time taken
			cout << "time taken to Search " << time_taken[1] << "ms" << endl<<endl;
			my_file << text.size() << "," << time_taken[0] << "," << time_taken[1] << endl;

		//	cout << endl << endl;
			//show_context(text, pos);			
		}
		
		cout << "book " << i << endl;
	}	
	system("pause");
	return 0;
}