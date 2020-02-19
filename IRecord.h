#ifndef IRECORD_H_
#define IRECORD_H_
//#include <cstdint>
#include "ltr/include/ltr24api.h"

class IRecord {
public:
	virtual void record(const DWORD *buf, const DWORD *buf_tmark,
			DWORD len) = 0;
	virtual ~IRecord(void) {
	}
};

#endif /* IRECORD_H_ */
