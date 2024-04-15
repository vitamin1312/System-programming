#pragma once

#include "stdafx.h"


inline void WriteLn()
{
	std::cout << std::endl;
}

template <class T, typename... Args> inline void WriteLn(T& value, Args... args)
{
	std::cout << value << " ";
	WriteLn(args...);
}

static CCriticalSection cs;
template <typename... Args> inline void SafeWriteLn(Args... args)
{
	cs.Lock();
	WriteLn(args...);
	cs.Unlock();
}
