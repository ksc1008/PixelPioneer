#include "channel.h"
#include <iostream>
#include "workerThread.h"

const int Channel::THREAD_COUNT = getProperThread();
Channel Channel::_chunkUpdateChannel;

int getProperThread() {
	auto tc = std::thread::hardware_concurrency();
	if(tc >= 2) {
		tc -= 1;
	}
	return tc;
}

Channel::Channel()
{
	std::cout << "THREAD Count: " << THREAD_COUNT << std::endl;
	startChannel();
}

void Channel::startChannel()
{
	if (started)
		return;
	for (int i = 0; i < THREAD_COUNT; i++) {
		WorkerThread* th = new WorkerThread(this,i);
		th->start();
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
