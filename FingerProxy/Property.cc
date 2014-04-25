/**
 * Property.cc
 */

#include "Property.h"
extern "C" {
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
}

//#define DEBUG_MSG

Property::Property(const sstring& inValue)
: mValueStr(inValue)
{
	mIsNum = (::sscanf(mValueStr, "%lf", &mValueNum) != 0);
}

bool
PropertyMap::Init(const sstring& inFilename)
{
	assert(size() == 0);
	int aErrorCnt = 0;
	FILE *aFp;
	
	if ((aFp = ::fopen(inFilename, "rt")) == NULL) {
		goto ON_OPENERROR;
	}
	
	for (int aLine = 1; ; aLine++) {
		static char sBuf[4096];
		char* aParserPt;
		
		::fgets(sBuf, sizeof(sBuf), aFp);
		if (feof(aFp) || ferror(aFp)) {
			goto ON_EOF;
		}
		if (sBuf[0] == '#') {
			continue;
		}
		for (int j = ::strlen(sBuf) - 1; j >= 0; j--) {
			if (sBuf[j] == '\r' || sBuf[j] == '\n') {
				sBuf[j] = '\0';
			}
		}
		if (sBuf[0] == '\0') {
			continue;
		}
		
		aParserPt = strchr(sBuf, ':');
		if (aParserPt == NULL) {
			cerr << "No parser at line " << aLine << endl;
		} else {
			*aParserPt = '\0';
			char* aValueFrom = aParserPt + 1;
			while (*aValueFrom != '\0' && isspace(*aValueFrom)) {
				aValueFrom++;
			}
			insert(value_type(sstring(sBuf), Property(aValueFrom)));
		}
	}
 ON_EOF:
	::fclose(aFp);
	return (aErrorCnt == 0);
 ON_OPENERROR:
	cerr << "Could not open file: " << inFilename << endl;
	return false;
}

const Property&
PropertyMap::operator[](const sstring& x) const
{
	static Property sDummy;
	const_iterator it = find(x);
	return (it != end()) ? (*it).second : sDummy;
}
