#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <iostream>
#include <ltr24api.h>

namespace test_ltrapi {
int getargs(int argc, char **argv, const char *&ip, WORD &port);
int getIp(const char *s);
} // namespace test_ltrapi

#endif /* FUNCTIONS_H_ */
