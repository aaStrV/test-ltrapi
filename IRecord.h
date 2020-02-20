#ifndef IRECORD_H_
#define IRECORD_H_

#include "ltr24api.h"

namespace test_ltrapi {
class IRecord {
public:
	virtual void record(const DWORD *buf, const DWORD *buf_tmark,
			DWORD len) = 0;
	virtual ~IRecord(void) {
	}
};
} // namespace test_ltrapi

#endif /* IRECORD_H_ */
