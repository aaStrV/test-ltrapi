#ifndef STDOUTRECORDER_H_
#define STDOUTRECORDER_H_
#include <iostream>
#include "IRecord.h"

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

#endif /* STDOUTRECORDER_H_ */
