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
#include <mutex>

//Necesary includes for task based parallelism 
#include "task.h"
#include "BoyerMoore.h"
#include "RabinKarp.h"
#include <mutex> // to protect the queue of tasks
#include <thread>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;
using std::ofstream;
using std::to_string;
using std::thread;
using std::mutex;
using std::unique_lock;
using std::condition_variable;

//define the alias for the clock type we're going to use
typedef std::chrono::steady_clock the_clock;

//my file to save values to
ofstream my_file("Timings.csv");
#define d 256 //number of characters in the english alphabet
int BOOKLIMIT = 1;
const int PATTERNCOUNT = 20;

vector<Position> BooyerMooreResults;
vector<Position> RabinKarpeResults;
mutex save_mutex;

bool BooyerMoore_ready = false;
bool RabinKarp_ready = false;

condition_variable result_cv;
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
			save_mutex.lock();
			 // Matched here add to the vector
			BooyerMooreResults.push_back(i);
			save_mutex.unlock();
			//print results to the screen
			//cout << "Match found: " << Results[Results.size() - 1] << endl;
			NumberofMatches++;
		}
	}
	BooyerMoore_ready = true;
//	cout << pat << " was found: " << NumberofMatches << " time(s)" << endl;
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
				save_mutex.lock();
				// Matched here add to the vector
				RabinKarpeResults.push_back(i);
				save_mutex.unlock();
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
	RabinKarp_ready = true;
//	cout << pat << " was found: " << NumberofMatches << " time(s)" << endl;
}

void finished_book(int volume)
{
	unique_lock <mutex> lock(save_mutex);
	while (!BooyerMoore_ready  && !RabinKarp_ready)
	{
		result_cv.wait(lock);
	}
	//cout << "Book: " << volume +1<< "complete " << endl;	
}
void old_string_Search()
{
	//STRING SEARCH DEFAULT CODE

	string text;//declare text as a string
	string FileName = "";

	string pat[PATTERNCOUNT];
	pat[0] = "244";
	pat[1] = "Tohka";
	pat[2] = "Origami";
	pat[3] = "Yoshino";
	pat[4] = "Shido";
	pat[5] = "Spirit";
	pat[6] = "DEM";
	pat[7] = "Angel";
	pat[8] = "Battle";
	pat[9] = "Blade";
	pat[10] = "244";
	pat[11] = "Tohka";
	pat[12] = "Origami";
	pat[13] = "Yoshino";
	pat[14] = "Shido";
	pat[15] = "Spirit";
	pat[16] = "DEM";
	pat[17] = "Angel";
	pat[18] = "Battle";
	pat[19] = "Blade";

	for (int i = 0; i < BOOKLIMIT; i++)
	{	//load_jute_book(text); //call the load function and pass it the file .txt
		FileName = "DateALiveVolume" + std::to_string(i + 1) + ".txt";
		load_file(FileName, text);
		//cout << "String size: " << text.size() << endl;
		for (int j = 0; j < (PATTERNCOUNT/2); j++)
		{
			//cout << "Boyer Moore" << endl;

			find_bm_multiple(pat[j], text);
		
			Rabin_Karp(pat[j], text);

		}

		//cout << "book " << i << endl;
	}
	//system("pause");
}

void Threaded_String_search()
{
	string pat[PATTERNCOUNT];
	pat[0] = "244";
	pat[1] = "Tohka";
	pat[2] = "Origami";
	pat[3] = "Yoshino";
	pat[4] = "Shido";
	pat[5] = "Spirit";
	pat[6] = "DEM";
	pat[7] = "Angel";
	pat[8] = "Battle";
	pat[9] = "Blade";
	pat[10] = "244";
	pat[11] = "Tohka";
	pat[12] = "Origami";
	pat[13] = "Yoshino";
	pat[14] = "Shido";
	pat[15] = "Spirit";
	pat[16] = "DEM";
	pat[17] = "Angel";
	pat[18] = "Battle";
	pat[19] = "Blade";


	//THREAD BASED PARALLELEISM CODE
	thread myThread[PATTERNCOUNT];
	thread printThread;
	string text;//declare text as a string
	//float time_taken[2];
	string FileName = "";
	for (int i = 0; i < BOOKLIMIT; i++)
	{
		FileName = "DateALiveVolume" + std::to_string(i+1) + ".txt";
		load_file(FileName, text);
	//	cout << "String size: " << text.size() << endl;

		printThread = thread(finished_book, i);

		for (int j = 0; j < (PATTERNCOUNT/2); j++)
		{
			//create thread and give it the function to run
			myThread[j] = thread(find_bm_multiple, pat[j], text);
			myThread[j + 10] = thread(Rabin_Karp, pat[j + 10], text);
		}
		//join the threads
		for (int j = 0; j < PATTERNCOUNT; j++)
		{ //combine all threads
			myThread[j].join();
		}

		if (RabinKarp_ready && BooyerMoore_ready)
		{
			result_cv.notify_one();
		}

		printThread.join();

	}
}
int main(int argc, char *argv[]) 
{
	float time_taken[2];
	int counter = 0;
	//set up headers for the file
	my_file << "number of books"<<","<< "serial Time taken" << "," << "parallel Time taken" << endl;

	for (int i = 0; i < 10000000; i++)
	{
	//time how long it takes to Search via serialised searching
	the_clock::time_point start = the_clock::now();
	old_string_Search();
	the_clock::time_point end = the_clock::now();
	time_taken[0] = duration_cast<milliseconds>(end - start).count();

	//print the time taken
	cout << "no book(s): " << BOOKLIMIT << " time taken to Search in serial " << time_taken[0] << "ms" << endl;

	//time how long it takes to Search via parallel searching
	start = the_clock::now();
	Threaded_String_search();
	end = the_clock::now();
	time_taken[1] = duration_cast<milliseconds>(end - start).count();
	//print the time taken
	cout <<"no book(s): "<< BOOKLIMIT<< " time taken to Search in parallel " << time_taken[1] << "ms" << endl << endl;
	//save to our file
	my_file <<BOOKLIMIT<< "," << time_taken[0] << "," << time_taken[1] << endl;	
		if (counter == 1000000)
		{			
			BOOKLIMIT++;
			counter = 0;
			my_file << "empty" << "," << "empty" << "," << "empty" << endl;
		}
		counter++;
	}

	return 0;
}