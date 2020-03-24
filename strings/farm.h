#ifndef FARM_H
#define FARM_H

#include "task.h"

// FIXME - You will need to add #includes here (probably <mutex> at least)
#include <queue>
#include <thread>
#include <vector>
#include <mutex>

using std::thread;
/** A collection of tasks that should be performed in parallel. */
class Farm {
public:
	// DO NOT CHANGE the public interface of this class.
	// You only need to implement the existing methods.

	/** Add a task to the farm.
	    The task will be deleted once it has been run. */
	void add_task(Task *task);

	/** Run all the tasks in the farm.
	    This method only returns once all the tasks in the farm
		have been completed. */
	void run();

private:
	std::queue<Task*> TaskQueue;
	std::vector<std::thread*> TaskThread;
	std::mutex lock_threads;
};

#endif
