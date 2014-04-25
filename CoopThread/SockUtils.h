/**
 * SockUtils.h
 */

#ifndef SOCKUTILS_H
#define SOCKUTILS_H

class sstring;
class SockUtils {
public:
	static const char* Ip2Str(unsigned long inAddr);
	static bool SetNonBlockingMode(int inFd);
	static bool SetBlockingMode(int inFd);
	static bool IsBlockingMode(int inFd);
	static bool IsNonBlockingMode(int inFd) { return (! IsBlockingMode(inFd)); }
	static unsigned long GetHostByName(const sstring& inHost); // returned value is in host byte order
	static void IgnoreSigPipe();
};

#endif
