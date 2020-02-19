#include "functions.h"
#include <stdio.h>

int getargs(int argc, char **argv, const char *&ip, DWORD &port) {
	if (argc == 1) {
	} else if (argc > 1) {
		ip = argv[1];
		int a[4];
		if (sscanf(argv[1], "%d.%d.%d.%d", &a[0], &a[1], &a[2], &a[3]) != 4) {
			cout << "Check IP" << endl;
			return -1;
		}

		for (int i = 0; i < 4; i++) {
			if ((a[i] < 0) || (255 < a[i])) {
				cout << "Wrong IP" << endl;
				return -1;
			}
		}
	} else if (argc == 3) {
		int tmpport = atoi(argv[2]);
		if (tmpport > 0xFFFF || tmpport < 0) {
			cout << "Check port" << endl;
			return -1;
		} else {
			port = (DWORD) tmpport;
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
