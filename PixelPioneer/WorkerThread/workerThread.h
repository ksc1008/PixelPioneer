#pragma once
#include <thread>

class Channel;
class Reuqest;

class WorkerThread {
	int m_id = -1;
	bool _halt = false;
	bool started = false;
	class Request* currentRequest = nullptr;
	class Channel* m_channel;
	std::thread m_thread;

public:
	~WorkerThread();
	WorkerThread(Channel* parentChannel, int id);
	void start();
	void stop();
	void processRequest(Request* rq);

	Request* getReqeust();
	bool halt();

private:
	void run();
	void invoke();
};