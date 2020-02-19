#include "StdoutRecorder.h"
using namespace std;

void StdoutRecorder::record(const DWORD *buf, const DWORD *buf_tmark,
		DWORD len) {
	cout << "----New data block receaved by StdoutRecorder" << id << ":"
			<< endl;
	for (int i = 0; i < len; i++) {
		cout << "StdoutRecorder" << id << ": " << hex << buf_tmark[i] << ":"
				<< buf[i] << dec << endl;
	}
	cout << "----End of data block" << endl;
}
