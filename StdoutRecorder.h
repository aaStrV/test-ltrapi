#ifndef STDOUTRECORDER_H_
#define STDOUTRECORDER_H_

#include <iostream>
#include "IRecord.h"

namespace test_ltrapi {
class StdoutRecorder: public IRecord {
	int id;
public:
	StdoutRecorder(int _id = 0) :
			id(_id) {
	}
	void record(const DWORD *buf, const DWORD *buf_tmark, DWORD len);
	~StdoutRecorder(void) {
	}
};
} // namespace test_ltrapi

#endif /* STDOUTRECORDER_H_ */
