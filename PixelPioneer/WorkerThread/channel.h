#pragma once
#include <vector>
#include <thread>
#include <stack>
#include <mutex>
#include <queue>

class WorkerThread;
class Request;

class Channel {
	static Channel _chunkUpdateChannel;

	std::mutex availableThreadsStackMutex;
	std::mutex requestQueueMutex;

	const static int THREAD_COUNT;
	bool started = false;
	std::vector<class WorkerThread*> threadPool;
	std::queue<class Request*> requestQueue;
	std::stack<int> availableThreads;

public:
	static Channel* GetChunkUpdateChannel() { return &_chunkUpdateChannel; }

	Channel();
	void startChannel();
	int getAvailableChannel();
	void setChannelAvailable(int channel);
	void executeRequest(Request* request);
	~Channel();

private:
	bool invokeFirstRequest();
	bool invokeFirstRequest(int channel);
	void invokeRequest(int id, Request* request);

	void enqueueRequest(Request* item);
	Request* dequeueRequest();
};

int getProperThread();