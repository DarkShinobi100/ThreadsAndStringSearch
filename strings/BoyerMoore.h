#ifndef BOYERMOORE_H
#define BOYERMOORE_H

#include <string>
#include <vector>
#include <mutex>
#include "task.h"
using std::string;
using std::vector;
using std::mutex;
//needed to load book
#include "utils.h"

/** Task that prints a message to cout. */
class BoyerMoore : public Task
{
public:
	BoyerMoore(const std::string& message, const int& volume, const std::string& pat, vector<Position> BooyerMooreResults)
		: message_(message)
		, volume_(volume)
		, pat_(pat)
		, BooyerMooreResults_(BooyerMooreResults)		
	{
	}

	void run();
	void loadBook();
	void find_bm_multiple();
	void Rabin_Karp();

private:
	const std::string message_;
	std::string text = "";
	const int volume_;
	const std::string pat_;
	vector<Position> BooyerMooreResults_;
};

#endif