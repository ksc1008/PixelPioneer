#pragma once
#include <thread>

class Channel;
class Reuqest;

class WorkerThread {
	int m_id = -1;
	class Channel* m_channel;
	std::thread m_thread;
protected:
	bool _halt = false;
	class Request* currentRequest = nullptr;
	bool started = false;

	void invoke();
	virtual void run();

public:
	~WorkerThread();
	WorkerThread(Channel* parentChannel, int id);
	void stop();
	void processRequest(Request* rq);

	Request* getReqeust();
	bool halt();

private:
	void start();
};