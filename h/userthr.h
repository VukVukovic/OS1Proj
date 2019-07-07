#ifndef USERTHR_H
#define USERTHR_H
#include "Thread.h"

class UserThread : public Thread {
	int argc;
	char **argv;
	int retCode;	

public:
	UserThread(int argc, char** argv) : Thread() {
		this->argc = argc;
		this->argv = argv;
		retCode = 0;
	}

	int returnCode() { return retCode; }
	~UserThread() { waitToComplete(); }
protected:
	void run();
};
#endif