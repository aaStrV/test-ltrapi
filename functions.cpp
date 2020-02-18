/*
 * functions.cpp
 *
 *  Created on: 18 февр. 2020 г.
 *      Author: sergius
 */

#include "functions.h"

int getargs(int argc, char **argv, const char **ip, DWORD *port) {
	if (argc == 1) {
	} else if (argc == 2) {
		*ip = argv[1];
	} else if (argc == 3) {
		*ip = argv[1];
		int tmpport = atoi(argv[2]);
		if (tmpport > 0xFFFF || tmpport < 0) {
			cout << "Check port" << endl;
			return -1;
		} else {
			*port = (DWORD) tmpport;
		}
	} else {
		cout << "Wrong arguments, use that:" << endl;
		cout << "  <progname> [ip [port]]" << endl;
		cout << "or, for 127.0.0.1:11111:" << endl;
		cout << "  <progname> [ip [port]]" << endl;
		return -2;
	}
	return 0;
}
