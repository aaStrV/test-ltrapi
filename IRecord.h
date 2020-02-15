/*
 * IRecord.h
 *
 *  Created on: 14 февр. 2020 г.
 *      Author: sergius
 */

#ifndef IRECORD_H_
#define IRECORD_H_
#include <cstdint>
#include "ltr/include/ltrapi.h"

class IRecord {
public:
	virtual void record(DWORD *buf, DWORD *buf_tmark, DWORD len) = 0;
	virtual ~IRecord(void) {
	}
};

#endif /* IRECORD_H_ */
