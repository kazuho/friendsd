extern "C" {
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
}
#include "SockUtils.h"
#include "sstring.h"

#ifndef NBLOCK_TAG
#error define NBLOCK_TAG (typically FNDELAY or O_NONBLOCK)
#endif

const char*
SockUtils::Ip2Str(unsigned long inAddr)
{
	static char aBuffer[16];
	::sprintf(aBuffer, "%d.%d.%d.%d",
						(int)(inAddr >> 24) & 255,
						(int)(inAddr >> 16) & 255,
						(int)(inAddr >> 8) & 255,
						(int)(inAddr >> 0) & 255);
	return aBuffer;
}

bool
SockUtils::SetNonBlockingMode(int inFd)
{
	int aArg;
	if ((aArg = ::fcntl(inFd, F_GETFL, 0)) == -1) {
		goto ON_ERROR;
	}
	if ((aArg & NBLOCK_TAG) == 0) {
		if (::fcntl(inFd, F_SETFL, aArg | NBLOCK_TAG) == -1) {
			goto ON_ERROR;
		}
	}
	return true;
 ON_ERROR:
	return false;
}

bool
SockUtils::SetBlockingMode(int inFd)
{
	int aArg;
	if ((aArg = ::fcntl(inFd, F_GETFL, 0)) == -1) {
		goto ON_ERROR;
	}
	if ((aArg & NBLOCK_TAG) != 0) {
		if (::fcntl(inFd, F_SETFL, aArg ^ NBLOCK_TAG) == -1) {
			goto ON_ERROR;
		}
	}
	return true;
 ON_ERROR:
	return false;
}

bool
SockUtils::IsBlockingMode(int inFd)
{
	int aArg = ::fcntl(inFd, F_GETFL, 0);
	return (aArg == -1 || (aArg & NBLOCK_TAG) == 0);
}

unsigned long
SockUtils::GetHostByName(const sstring& inHost)
{
	unsigned long aAddr = ::inet_addr(inHost);
	if (aAddr == 0xffffffff) {
		hostent* aHostent = ::gethostbyname(inHost);
		if (aHostent != 0) {
			aAddr = *(typeof(aAddr)*)aHostent->h_addr;
		}
	}
	return ntohl(aAddr);
}

void
SockUtils::IgnoreSigPipe()
{
	::signal(SIGPIPE, SIG_IGN);
}

