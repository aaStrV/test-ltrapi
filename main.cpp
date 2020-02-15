#include <iostream>
#include "ltr/include/ltrapi.h"
#include "CrateEU2.h"
#include "StdoutRecorder.h"
#include "StdoutChannelRecorder.h"
#include "CrateReader.h"
//#include <ctime>
using namespace std;

int main(int argc, char **argv) {
	cout << "Program started" << endl;

	CrateEU2 crate("1234567812345678", "127.0.0.1", LTRD_PORT_DEFAULT);
	StdoutRecorder rec(0);
	StdoutChannelRecorder rec1(1, 100);
	StdoutChannelRecorder rec2(2, 231);
	StdoutChannelRecorder rec3(3, 56);
	CrateReader creader(crate);
	creader.subscribe(rec);
	creader.subscribe(rec1);
	creader.subscribe(rec2);
	creader.subscribe(rec3);

	crate.init();
	crate.resetModules();
	sleep(5);

	creader.readTestModule(0, 50);
	sleep(1);
	creader.readModule(0);
	sleep(2);

	cout << "Program finished" << endl;
	return 0;
}
