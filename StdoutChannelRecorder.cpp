#include "StdoutChannelRecorder.h"
using namespace std;

void StdoutChannelRecorder::record(const DWORD *buf, const DWORD *buf_tmark,
		DWORD len) {
	cout << "----New data block receaved by StdoutChannelRecorder" << id << ":"
			<< endl;
	for (DWORD i = 0; i < len; i++) {
		// допустим каналов 16, и они кодируются в 4-х
		// младших битах. Тогда маска 0xF
		// UPD: в LTR24 каналов 4, кодируются в 4, 5 битах,
		// маска 0x30;
		DWORD _chan = (buf[i] & 0x30) >> 4;
		if (_chan != channel)
			continue;
		cout << "StdoutChannelRecorder" << id << ", channel = " << channel
				<< ": " << hex << buf_tmark[i] << ":" << buf[i] << dec << endl;
	}
	cout << "----End of data block" << endl;
}
