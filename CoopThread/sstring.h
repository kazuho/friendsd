/**
 * sstring.h
 */

#ifndef SSTRING_H
#define SSTRING_H

#include <algobase.h>
#include <iterator.h>
extern "C" {
#include <limits.h>
#include <stddef.h>
}

class sstring {
public:
	typedef char value_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef value_type* pointer;
	typedef pointer iterator;
	typedef const pointer const_iterator;
	typedef ptrdiff_t difference_type;
	typedef unsigned int size_type;
	typedef reverse_iterator<iterator, value_type, reference, difference_type> reverse_iterator;
	typedef reverse_iterator<const_iterator, value_type, const_reference, difference_type> const_reverse_iterator;
protected:
	char* buf;
	int len, buflen;
public:
	sstring(int inBuflen = 64);
	sstring(const sstring& x);
	sstring(const char* inStr);
	sstring(size_type n, char x);
	sstring(const_iterator begin, const_iterator end);
	virtual ~sstring();
	sstring& operator=(const sstring& x);
	sstring& operator+=(const sstring& x);
	sstring& operator+=(const char* x);
	bool operator==(const sstring& x) const;
	bool operator==(const char* x) const;
	bool operator<(const sstring& x) const;
	bool operator<(const char* x) const;
	bool operator<=(const sstring& x) const;
	bool operator<=(const char* x) const;
	bool operator>(const sstring& x) const { return ! operator<=(x); }
	bool operator>(const char* x) const { return ! operator<=(x); }
	bool operator>=(const sstring& x) const { return ! operator<(x); }
	bool operator>=(const char* x) const { return ! operator<(x); }
	size_type size() const { return len; }
	size_type max_size() const { return max(size_type(1), size_type(UINT_MAX / sizeof(char))); }
	bool empty() const { return len == 0; }
	operator const char*() const { return buf; }
	void push_front(char x);
	void push_back(char x);
	void pop_front();
	void pop_back();
	iterator begin() { return buf; }
	const_iterator begin() const { return buf; }
	iterator end() { return buf + len; }
	const_iterator end() const { return buf + len; }
	reverse_iterator rbegin() { return reverse_iterator(end()); }
	const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
	reverse_iterator rend() { return reverse_iterator(begin()); }
	const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
	iterator insert(char* position, char x);
	void insert(char* position, size_type n, char x);
	void insert(char* position, const char* begin, const char* end);
	void erase(char* position);
	void erase(char* begin, char* end);
	reference operator[](const size_type n) { return buf[n]; }
	const_reference operator[](const size_type n) const { return buf[n]; }
	sstring left(int len) const { return sstring(begin(), begin() + len); }
	sstring right(int len) const { return sstring(end() - len, end()); }
	sstring middle(int offset, int len) { return sstring(begin() + offset, begin() + offset + len); }
	void ensure_size(int inLen);
	void release();
	void swap(sstring& x);
public:
	struct hashcode {
		int operator()(const sstring& x) const;
	};
};

sstring operator+(const sstring& x, const sstring& y);

#endif
