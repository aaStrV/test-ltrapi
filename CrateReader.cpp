/*
 * DataDispetcher.cpp
 *
 *  Created on: 14 февр. 2020 г.
 *      Author: sergius
 */

#include "CrateReader.h"
using namespace std;

int CrateReader::readModule(DWORD module, DWORD len) {
	set<IRecord*>::iterator rec_it;
	int err = 0;
	int receaved = 0;

	if (!crate->isReady()) {
		cerr << "CrateReader::readModule(), crate is not ready" << endl;
		return -1;
	}
	err = receaved = crate->getData(module, buf, buf_tmark, len, timeout);
	if (err < 0) {
		cerr << "CrateReader::readModule(), crate->getData: error " << err
				<< endl;
		return err;
	}
	for (rec_it = clients.begin(); rec_it != clients.end(); rec_it++) {
		(*rec_it)->record(buf, buf_tmark, receaved);
	}
	return 0;
}

int CrateReader::readTestModule(DWORD module, DWORD len) {
//	cout << "CrateReader::readTestModule(), started, reading "<< len << " samples" << endl;
	set<IRecord*>::iterator rec_it;
	int err = 0;
	int receaved = 0;

	err = receaved = crate->getTestData(module, buf, buf_tmark, len, timeout);
	if (err < 0) {
		cerr << "CrateReader::readModule(), crate->getData: error " << err
				<< endl;
		return err;
	}
	for (rec_it = clients.begin(); rec_it != clients.end(); rec_it++) {
		(*rec_it)->record(buf, buf_tmark, receaved);
	}

	/*cout << "CrateReader::readTestModule(), finished: buf_tmark[0]:buf[0] = "
	 << hex << buf_tmark[0] << ":" << buf[0] << dec << endl;*/
	return 0;
}
