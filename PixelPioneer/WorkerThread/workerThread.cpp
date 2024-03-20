#include "workerThread.h"
#include "channel.h"
#include "request.h"

WorkerThread::~WorkerThread()
{
	_halt = true;
	m_thread.join();
}

WorkerThread::WorkerThread(Channel* parentChannel, int id)
{
	m_channel = parentChannel;
	m_id = id;
}

void WorkerThread::start()
{
	m_thread = std::thread(&WorkerThread::run,this);
	started = true;
}

void WorkerThread::stop()
{
	_halt = true;
	m_thread.join();
}

void WorkerThread::run()
{
	while (!_halt) {
		if (currentRequest != nullptr) {	
			invoke();
		}
	}
}

void WorkerThread::invoke()
{
	currentRequest->execute();
	delete currentRequest;
	currentRequest = nullptr;

	m_channel->setChannelAvailable(m_id);
}

void WorkerThread::processRequest(Request* rq)
{
	currentRequest = rq;
}

Request* WorkerThread::getReqeust()
{
	return currentRequest;
}

bool WorkerThread::halt()
{
	return _halt;
}
