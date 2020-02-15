/*
 * StdoutChannelRecorder.cpp
 *
 *  Created on: 14 февр. 2020 г.
 *      Author: sergius
 */

#include "StdoutChannelRecorder.h"
using namespace std;

void StdoutChannelRecorder::record(DWORD *buf, DWORD *buf_tmark, DWORD len) {
	cout << "----New data block receaved by StdoutChannelRecorder" << id << ":"
			<< endl;
	for (int i = 0; i < len; i++) {
		// допустим каналов 16, и они кодируются в 4-х
		// младших битах. Тогда маска 0xF
		DWORD _chan = buf[i] & 0xF;
		if (_chan != channel)
			continue;
		cout << "StdoutChannelRecorder" << id << ", channel = " << channel
				<< ": " << hex << buf_tmark[i] << ":" << buf[i] << dec << endl;
	}
	cout << "----End of data block" << endl;
}
