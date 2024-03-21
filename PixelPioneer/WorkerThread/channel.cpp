#include "channel.h"
#include <iostream>
#include "workerThread.h"
#include "workerThreadNonstop.h"

const int Channel::THREAD_COUNT = getProperThread();
Channel c1(CHUNK_LOAD);
Channel c2(CHUNK_UPDATE);

Channel* Channel::_chunkUpdateChannel[ENUM_CHANNELTYPE_MAX] {
	&c1,&c2
};

int getProperThread() {
	auto tc = std::thread::hardware_concurrency();
	if(tc >= 2) {
		tc -= 1;
	}
	return tc;
}

Channel::Channel(ChannelType type)
{
	if (type == CHUNK_LOAD) {
		startChannel(THREAD_COUNT*4, false);
	}
	else if (type == CHUNK_UPDATE) {
		startChannel(1, true);
	}
	else {

	}
}

void Channel::startChannel(int threadNum, bool loopPermanent)
{
	if (started)
		return;
	for (int i = 0; i < threadNum; i++) {
		WorkerThread* th;
		if (loopPermanent) { 
			th = new WorkerThreadNonstop(this, i);
		}
		else {
			th = new WorkerThread(this, i);
		}
		threadPool.push_back(th);
		setChannelAvailable(i);
	}
}


int Channel::getAvailableChannel()
{
	auto result = -1;
	availableThreadsStackMutex.lock();

	if (!availableThreads.empty()) {
		result = availableThreads.top();
		availableThreads.pop();
	}

	availableThreadsStackMutex.unlock();
	return result;
}

void Channel::setChannelAvailable(int channel)
{
	availableThreadsStackMutex.lock();

	if (!invokeFirstRequest(channel)) {
		availableThreads.push(channel);
	}

	availableThreadsStackMutex.unlock();
}

void Channel::executeRequest(Request* request)
{
	int id = getAvailableChannel();
	if (id >= 0) {
		invokeRequest(id, request);
	}
	else {
		enqueueRequest(request);
	}
}

Channel::~Channel()
{
	if (started) {
		for (auto th: threadPool) {
			delete th;
		}
	}
}

bool Channel::invokeFirstRequest()
{
	int id = getAvailableChannel();	
	return invokeFirstRequest(id);
}

bool Channel::invokeFirstRequest(int channel)
{
	auto queue = dequeueRequest();
	if (queue == nullptr)
		return false;

	invokeRequest(channel, queue);
	return true;
}

void Channel::invokeRequest(int id, Request* request)
{
	threadPool[id]->processRequest(request);
}

void Channel::enqueueRequest(Request* item)
{
	requestQueueMutex.lock();
	requestQueue.push(item);
	requestQueueMutex.unlock();
}

Request* Channel::dequeueRequest()
{
	requestQueueMutex.lock();

	if (requestQueue.empty()) {
		requestQueueMutex.unlock();
		return nullptr;
	}

	auto queue = requestQueue.front();
	requestQueue.pop();

	requestQueueMutex.unlock();

	return queue;
}
