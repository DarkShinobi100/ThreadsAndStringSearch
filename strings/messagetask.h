#ifndef MESSAGETASK_H
#define MESSAGETASK_H

#include <string>

#include "task.h"
using std::string;

//needed to load book
#include "utils.h"

/** Task that prints a message to cout. */
class MessageTask : public Task
{
public:
	MessageTask(const std::string& message, const int& volume, const std::string& pat)
		: message_(message)
		, volume_(volume)
		, pat_(pat)
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
};

#endif