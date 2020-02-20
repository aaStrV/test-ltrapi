#ifndef STDOUTCHANNELRECORDER_H_
#define STDOUTCHANNELRECORDER_H_

#include <iostream>
#include "IRecord.h"

namespace test_ltrapi {
class StdoutChannelRecorder: public IRecord {
	int id;
	unsigned int channel;
public:
	StdoutChannelRecorder(unsigned int _channel = 0, int _id = 0) :
			channel(_channel & 0xFF), id(_id) {
	}
	void record(const DWORD *buf, const DWORD *buf_tmark, DWORD len);
	~StdoutChannelRecorder(void) {
	}
};
} // namespace test_ltrapi

#endif /* STDOUTCHANNELRECORDER_H_ */
