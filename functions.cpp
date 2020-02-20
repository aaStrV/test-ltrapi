#include "functions.h"

int getargs(int argc, char **argv, const char *&ip, WORD &port) {
	if (argc == 1) {
	}
	if (argc > 1) {
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
	}
	if (argc > 2) {
		int tmpport = atoi(argv[2]);
		if (tmpport > 0xFFFF || tmpport < 0) {
			cout << "Check port" << endl;
			return -1;
		} else {
			port = (WORD) tmpport;
		}
	}
	if (argc > 3) {
		cout << "Wrong arguments, use that:" << endl;
		cout << "  <progname> [ip [port]]" << endl;
		cout << "or, for 127.0.0.1:11111:" << endl;
		cout << "  <progname> [ip [port]]" << endl;
		return -2;
	}
	return 0;
}

int getIp(const char *s) {
	int a[4];
	int ip;
	sscanf(s, "%d.%d.%d.%d", &a[0], &a[1], &a[2], &a[3]);
	ip = (a[0] << 24) | (a[1] << 16) | (a[2] << 8) | a[3];
	return ip;
}
