#ifndef CRATEREADER_H_
#define CRATEREADER_H_

#include <ltr24api.h>
#include <iostream>
#include <set>
#include "IRecord.h"
#include "CrateEu2.h"
#include "defines.h"
using namespace std;

/**
 * Читает модули крейта, рассылает данные на регистраторы.
 * Возможен запуск чтения модуля из отдельного потока. В теории
 */
class CrateReader {
private:
	DWORD buf[MAX_BUF];
	DWORD buf_tmark[MAX_BUF];
	set<IRecord*> clients;
	CrateEu2 *crate;
	DWORD timeout;

public:
	CrateReader(CrateEu2 &crate, DWORD _timeout = 2000) :
			timeout(_timeout) {
		this->crate = &crate;
	}
	void subscribe(IRecord &client) {
		clients.insert(&client);
	}
	void unsubscribe(IRecord &client) {
		std::set<IRecord*>::iterator it = clients.find(&client);
		if (it != clients.end()) {
			clients.erase(it);
		}
	}
	int readModule(INT module, DWORD len = 1024);
	int readTestModule(DWORD module, DWORD len = 10);
};

#endif /* CRATEREADER_H_ */
