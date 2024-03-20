#pragma once
#include <vector>
#include <thread>
#include <stack>
#include <mutex>
#include <queue>

class WorkerThread;
class Request;

class Channel {
	std::mutex availableThreadsStackMutex;
	std::mutex requestQueueMutex;

	const static int THREAD_COUNT;
	bool started = false;
	std::vector<class WorkerThread*> threadPool;
	std::queue<class Request*> requestQueue;
	std::stack<int> availableThreads;

public:
	Channel();
	void startChannel();
	int getAvailableChannel();
	void setChannelAvailable(int channel);
	void executeRequest(Request* request);
	~Channel();

private:
	void invokeFirstRequest();
	void invokeFirstRequest(int channel);
	void invokeRequest(int id, Request* request);

	void enqueueRequest(Request* item);
	Request* dequeueRequest();
};

int getProperThread();