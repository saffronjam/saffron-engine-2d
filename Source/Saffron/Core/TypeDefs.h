#pragma once

namespace Se
{
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
#if defined(_MSC_VER)
typedef unsigned __int64 ulong;
#else
typedef unsigned long long ulong;
#endif

}
