#include "CrateReader.h"
using namespace std;

namespace test_ltrapi {
int CrateReader::readModule(INT module, DWORD _len) {
	DWORD len = _len <= MAX_BUF ? _len : MAX_BUF;
	int err = 0;
	int receaved = 0;

	err = receaved = crate->getData(module, buf, buf_tmark, len, timeout);
	if (err < 0) {
		cerr << "CrateReader::readModule(), crate->getData: error " << err
				<< endl;
		return err;
	} else {
		cout << "Read done " << endl;
	}

	set<IRecord*>::iterator rec_it;
	for (rec_it = clients.begin(); rec_it != clients.end(); rec_it++) {
		(*rec_it)->record(buf, buf_tmark, receaved);
	}
	return 0;
}

int CrateReader::readTestModule(DWORD module, DWORD _len) {
//	cout << "CrateReader::readTestModule(), started, reading "<< len << " samples" << endl;
	set<IRecord*>::iterator rec_it;
	int err = 0;
	int receaved = 0;
	DWORD len = _len <= MAX_BUF ? _len : MAX_BUF;

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
} // namespace test_ltrapi
