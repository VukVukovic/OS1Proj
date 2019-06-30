#include "userthr.h"
#include "utils.h"

int userMain(int argc, char* argv[]);

void UserThread::run() {
	retCode = userMain(argc,argv);
}