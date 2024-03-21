#pragma once
#include "workerThread.h"

class WorkerThreadNonstop :public WorkerThread {
protected:
	void run();

public:
	WorkerThreadNonstop(Channel* parentChannel, int id):WorkerThread(parentChannel,id){}
};