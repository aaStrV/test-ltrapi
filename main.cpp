#include <iostream>
#include "ltr/include/ltrapi.h"
#include "CrateEU2.h"
#include "StdoutRecorder.h"
#include "StdoutChannelRecorder.h"
#include "CrateReader.h"
#include "defines.h"
#include "functions.h"
//#include <ctime>
using namespace std;

int main(int argc, char **argv) {
	// Обработка аргументов
	// Формат ввода: <progname> [ip [port]]
	const char *ip = IP_LTRD;
	DWORD port = PORT_LTRD;
	if (!(getargs(argc, argv, ip, port) == 0))
		return -1;

	CrateEU2 crate("1234567887654321", ip, port);
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
	sleep(1);
	crate.startSyncMarks(LTR_MARK_EXT_DIGIN1_RISE, LTR_MARK_EXT_DIGIN2_RISE);

//	creader.readTestModule(0, 50);
//	sleep(1);
	creader.readModule(0);
	return 0;
}
