#include "workerThreadNonstop.h"
#include<iostream>
void WorkerThreadNonstop::run()
{
	started = true;
	while (!_halt) {
		if(currentRequest != nullptr)
			invoke();
	}
	started = false;
}
