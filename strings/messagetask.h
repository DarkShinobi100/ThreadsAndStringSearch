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
	MessageTask(const std::string& message)
		: message_(message)
	{
	}

	void run(int volume, string& pat);
	void loadBook(int volume);
	void find_bm_multiple(const string& pat);
	void Rabin_Karp(const string& pat);

private:
	const std::string message_;
	std::string text = "";
};

#endif