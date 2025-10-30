// DebugDict.h - Only for debug.C
#ifndef DEBUG_DICT_H
#define DEBUG_DICT_H

#include <vector>

#ifdef __CLING__
#pragma cling load("libTree")
#pragma link C++ class std::vector<std::vector<float>>+;
#endif

#endif
