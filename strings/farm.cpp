#include "farm.h"

// FIXME - You may need to add #includes here (e.g. <thread>)

void Farm::add_task(Task *task)
{
	TaskQueue.push(task);
}


void Farm::run()
{
	auto lambda = [=] {
		while (!TaskQueue.empty()) //while queue is not null
		{
			lock_threads.lock();
			//take a copy of 1st value in the queue
			Task* task = TaskQueue.front();
			//remove it from the queue as it's now worked on
			TaskQueue.pop();
			lock_threads.unlock();
			//call the run function from messagetask on that task
			task->run();
			//delete our copy
			delete task;
		}
	};

	//std::vector<std::thread> TaskThread;
	for (int i = 0; i < std::thread::hardware_concurrency(); i++)
	{
		//create thread
		//add thread to vector
		TaskThread.push_back(new thread(lambda));
	}

	//includes all variables in local scope by val
	//thread worker_functor();

	//// Wait for TaskThread to finish.
	for (int i = 0; i <TaskThread.size(); i++)
	{
		TaskThread[i]->join();
	}		

	//worker_functor.join();
}