#pragma once

class Request {

protected:
	int id;

public:
	Request();

	virtual void execute();
};