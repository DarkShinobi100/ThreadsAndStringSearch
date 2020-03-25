#ifndef RABINKARP_H
#define RABINKARP_H

#include <string>
#include <mutex>
#include "task.h"
#include <vector>
using std::string;
using std::mutex;
using std::vector;
//needed to load book
#include "utils.h"

/** Task that prints a message to cout. */
class RabinKarp : public Task
{
public:
	RabinKarp(const std::string& message, const int& volume, const std::string& pat, vector<Position> RabinKarpeResults)
		: message_(message)
		, volume_(volume)
		, pat_(pat)
		, RabinKarpeResults_(RabinKarpeResults)
		//, RabinKarp_mutex_(RabinKarp_mutex)
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
	vector<Position> RabinKarpeResults_;
	
};

#endif