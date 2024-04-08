#pragma once
#include <vector>
#include <thread>
#include <stack>
#include <mutex>
#include <queue>

class WorkerThread;
class WorkerThreadNonstop;
class Request;

enum ChannelType{CHUNK_LOAD, CHUNK_UPDATE, ENUM_CHANNELTYPE_MAX};

class Channel {
	static Channel* _chunkUpdateChannel[ENUM_CHANNELTYPE_MAX];

	std::mutex availableThreadsStackMutex;
	std::mutex requestQueueMutex;

	const static int THREAD_COUNT;
	bool started = false;
	int channel_count;
	std::vector<class WorkerThread*> threadPool;
	std::queue<class Request*> requestQueue;
	std::stack<int> availableThreads;

public:
	static Channel& GetChunkLoadChannel() { return *_chunkUpdateChannel[CHUNK_UPDATE]; }
	static Channel& GetChunkUpdateChannel() { return *_chunkUpdateChannel[CHUNK_LOAD]; }

	Channel(ChannelType type);
	void startChannel(int threadNum, bool loopPermanent);
	int getAvailableChannel();
	void setChannelAvailable(int channel);
	void executeRequest(Request* request);
	void waitAll();
	~Channel();

private:
	bool invokeFirstRequest();
	bool invokeFirstRequest(int channel);
	void invokeRequest(int id, Request* request);

	void enqueueRequest(Request* item);
	Request* dequeueRequest();
};

int getProperThread();