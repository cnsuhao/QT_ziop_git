// StdAfx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
//#include <set>
#include "targetver.h"
#include <map>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/chrono.hpp>


#define BREAKPOINT { _asm int 3 }
#define CAT(x, y) x##y
#define FORWARD_DECLARE(x) struct x; typedef boost::shared_ptr<x> CAT(x, Ptr); typedef std::vector<CAT(x, Ptr)> CAT(x, PtrList)
#define FORWARD_DECLARE_1(T1) FORWARD_DECLARE(T1)
#define FORWARD_DECLARE_2(T1, T2) FORWARD_DECLARE_1(T1); FORWARD_DECLARE(T2)
#define FORWARD_DECLARE_3(T1, T2, T3) FORWARD_DECLARE_2(T1, T2); FORWARD_DECLARE(T3)
#define FORWARD_DECLARE_4(T1, T2, T3, T4) FORWARD_DECLARE_3(T1, T2, T3); FORWARD_DECLARE(T4)
#define FORWARD_DECLARE_5(T1, T2, T3, T4, T5) FORWARD_DECLARE_4(T1, T2, T3, T4); FORWARD_DECLARE(T5)
#define FORWARD_DECLARE_6(T1, T2, T3, T4, T5, T6) FORWARD_DECLARE_5(T1, T2, T3, T4, T5); FORWARD_DECLARE(T6)
#define FORWARD_DECLARE_7(T1, T2, T3, T4, T5, T6, T7) FORWARD_DECLARE_6(T1, T2, T3, T4, T5, T6); FORWARD_DECLARE(T7)
#define FORWARD_DECLARE_8(T1, T2, T3, T4, T5, T6, T7, T8) FORWARD_DECLARE_7(T1, T2, T3, T4, T5, T6, T7); FORWARD_DECLARE(T8)
#define FORWARD_DECLARE_9(T1, T2, T3, T4, T5, T6, T7, T8, T9) FORWARD_DECLARE_8(T1, T2, T3, T4, T5, T6, T7, T8); FORWARD_DECLARE(T9)
