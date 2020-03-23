#ifndef TASK_H
#define TASK_H

#include <string>

using std::string;

/** Abstract base class: a task to be executed. */
class Task
{
public:
	virtual ~Task()
	{
	}

	/** Perform the task. Subclasses must override this. */
	virtual void run(int volume,string& pat) = 0;
	virtual void loadBook(int volume) = 0;
	virtual void find_bm_multiple(const string& pat) = 0;
	virtual void Rabin_Karp(const string& pat) = 0;

private:

#define d 256 //number of characters in the english alphabet
};

#endif
