#include "user.h"
//#include <iostream.h>
#include "utils.h"

int userMain(int argc, char* argv[]);

void UserThread::run() {
	int ret = userMain(argc,argv);
	lock;
	retCode = ret;
	unlock;
	//cout << "Retcode form run(): " << retCode << endl;
}