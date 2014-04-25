/**
 * AllocDebug.cc
 */

#include "AllocDebug.h"
extern "C" {
#include <assert.h>
}
#include <iostream.h>
#include <map.h>

struct _AllocEntry {
	const char* file;
	int line;
	const char* exp;
	_AllocEntry() {}
	_AllocEntry(const char* inFile, int inLine, const char* inExp) : file(inFile), line(inLine), exp(inExp) {}
};
ostream& operator<<(ostream&, const _AllocEntry&);

ostream& operator<<(ostream& ostr, const _AllocEntry& x)
{
	return (ostr << x.file << '(' << x.line << "): " << x.exp);
}

typedef map<void*, _AllocEntry, less<const void*> > _AllocMap;

static _AllocMap _sAllocMap;

void*
_AddPtr(void* p, const char* file, int line, const char* exp)
{
	if (_sAllocMap.find(p) != _sAllocMap.end()) {
		cerr << "At: " << exp << endl
				<< "  pointer to " << p << " is already registered: " << endl
				<< "  " << _sAllocMap[p] << endl;
		::exit(1);
	}
	_sAllocMap[p] = _AllocEntry(file, line, exp);
	return p;
}

void*
_ResetPtr(void* dst, void* src, const char* file, int line, const char* exp)
{
	if (_sAllocMap.find(src) == _sAllocMap.end()) {
		cerr << "At: " << exp << endl
				<< "  original pointer to " << src << " is not registered." << endl;
		::exit(1);
	}
	if (dst != NULL) {
		_sAllocMap.erase(src);
		if (_sAllocMap.find(dst) != _sAllocMap.end()) {
			cerr << "At: " << exp << endl
					<< "  reallocated pointer to " << dst << " is already registered: " << endl
					<< "  " << _sAllocMap[dst] << endl;
			::exit(1);
		}
		_sAllocMap[dst] = _AllocEntry(file, line, exp);
	}
	return dst;
}

void*
_RemovePtr(void* p, const char* file, int line, const char* exp)
{
	if (_sAllocMap.find(p) == _sAllocMap.end()) {
		cerr << "At: " << _AllocEntry(file, line, exp) << endl
				<< "  pointer to " << p << " is not registered." << endl;
		::exit(1);
	}
	_sAllocMap.erase(p);
	return p;
}

int
_AllocCount()
{
	return _sAllocMap.size();
}

void
_ShowAllocs()
{
	cout << "_ShowAllocs: " << _sAllocMap.size() << " pointers are registered." << endl;
	for (_AllocMap::const_iterator it = _sAllocMap.begin();
			it != _sAllocMap.end();
			++it) {
		cout << "  " << (*it).first << ": " << (*it).second << endl;
	}
}
