#ifndef USER_H
#define USER_H
#include "Thread.h"

class UserThread : public Thread {
	int argc;
	char **argv;
	int retCode;	

public:
	UserThread(int argc, char** argv) : Thread() {
		this->argc = argc;
		this->argv = argv;
		retCode = 8;
	}

	int returnCode() { return retCode; }
	~UserThread() { waitToComplete(); }
protected:
	void run();
};
#endif