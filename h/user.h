#ifndef USER_H
#define USER_H
#include "Thread.h"

class UserThread : public Thread {
	int argc;
	char **argv;
	volatile int retCode;
public:
	UserThread(int argc, char** argv) : Thread() {
		this->argc = argc;
		this->argv = argv;
	}
	int returnCode() { return retCode; }
protected:
	void run();
};
#endif