#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <iostream>
#include <ltr24api.h>
#include <stdio.h>
using namespace std;

int getargs(int argc, char **argv, const char *&ip, WORD &port);
int getIp(const char * s);

#endif /* FUNCTIONS_H_ */
