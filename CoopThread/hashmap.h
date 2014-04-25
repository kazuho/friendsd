/**
 * hashmap.h
 */

#ifndef HASHMAP_H
#define HASHMAP_H

#include <list.h>
#include <pair.h>
extern "C" {
#include <limits.h>
}

template <class Key, class T, class Hashcode>
class hashmap {
	typedef hashmap<Key, T, Hashcode> my_type;
friend bool operator==(const my_type& x, const my_type& y);
public:
	typedef const Key key_type;
	typedef pair<const Key, T> value_type;
	typedef list<value_type> backet_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef value_type* pointer;
	class iterator;
	class const_iterator;
	typedef backet_type::difference_type difference_type;
	typedef backet_type::size_type size_type;
	typedef Hashcode key_hashcode;
protected:
	const key_hashcode hashcode;
	const float load;
	backet_type* c;
	size_type tblsize, length;
	void rehash() {
		backet_type* oldbackets = c;
		int oldtblsize = tblsize;
		tblsize = tblsize * 2 + 1;
		c = new backet_type [tblsize];
		
		for (int j = 0; j < oldtblsize; j++)
			for (backet_type::const_iterator backetit = oldbackets[j].begin();
					backetit != oldbackets[j].end();
					++backetit)
				c[hashcode((*backetit).first) % tblsize].push_back(*backetit);
		
		delete [] oldbackets;
	}
public:
	class iterator : public bidirectional_iterator<value_type, difference_type> {
	friend class my_type;
	friend class const_iterator;
	protected:
		my_type *h;
		size_type offset;
		backet_type::iterator it;
		iterator(my_type* inHash, int inOffset, backet_type::iterator inIt) : h(inHash), offset(inOffset), it(inIt) {}
	public:
		iterator() {}
		bool operator==(const iterator& x) const { return (h == x.h && offset == x.offset && it == x.it); }
		reference operator*() const { return *it; }
		iterator& operator++() {
			++it;
			while (it == h->c[offset].end() && offset + 1 < h->tblsize)
				it = h->c[++offset].begin();
			return *this;
		}
		iterator operator++(int) {
			iterator tmp = *this;
			++*this;
			return tmp;
		}
		iterator& operator--() {
			if (it == h->c[offset].begin() && offset != 0) {
				--offset;
				while (h->c[offset].size() == 0 && offset != 0)
					--offset;
				it = h->c[offset].end();
				--it;
			} else
				--it;
			return *this;
		}
		iterator operator--(int) {
			iterator tmp = *this;
			--*this;
			return tmp;
		}
	};
	class const_iterator : public bidirectional_iterator<value_type, difference_type> {
	friend class my_type;
	protected:
		my_type const* h;
		size_type offset;
		backet_type::const_iterator it;
		const_iterator(const my_type* inHash, int inOffset, backet_type::const_iterator inIt) : h(inHash), offset(inOffset), it(inIt) {}
	public:
		const_iterator() {}
		const_iterator(const iterator& x) : h(x.h), offset(x.offset), it(x.it) {}
		bool operator==(const const_iterator& x) const { return (h == x.h && offset == x.offset && it == x.it); }
		const_reference operator*() const { return *it; }
		const_iterator& operator++() {
			++it;
			while (it == h->c[offset].end() && offset + 1 < h->tblsize)
				it = h->c[++offset].begin();
			return *this;
		}
		const_iterator operator++(int) {
			const_iterator tmp = *this;
			++*this;
			return tmp;
		}
		const_iterator& operator--() {
			if (it == h->c[offset].begin() && offset != 0) {
				--offset;
				while (h->c[offset].size() == 0 && offset != 0)
					--offset;
				it = h->c[offset].end();
				--it;
			} else
				--it;
			return *this;
		}
		const_iterator operator--(int) {
			const_iterator tmp = *this;
			--*this;
			return tmp;
		}
	};
	typedef reverse_bidirectional_iterator<const_iterator, value_type, const_reference, difference_type> const_reverse_iterator;
	typedef reverse_bidirectional_iterator<iterator, value_type, reference, difference_type> reverse_iterator;
	hashmap(key_hashcode inHashcode = key_hashcode(), size_type inTblsize = 17, float inLoad = 0.75f) : hashcode(inHashcode), load(inLoad), tblsize(inTblsize), length(0) {
		c = new backet_type [tblsize];
	}
	hashmap(const my_type& x, key_hashcode inHashcode = key_hashcode(), size_type inTblsize = 17, float inLoad = 0.75f) : hashcode(inHashcode), load(inLoad), tblsize(inTblsize), length(0) {
		c = new backet_type [tblsize];
		for (const_iterator it = x.begin(); it != x.end(); ++it)
			insert(*it);
	}
	hashmap(const_iterator begin, const_iterator end, key_hashcode inHashcode, size_type inTblsize = 17, float inLoad = 0.75f) : hashcode(inHashcode), load(inLoad), tblsize(inTblsize), length(0) {
		c = new backet_type [tblsize];
		while (begin != end)
			insert(*begin++);
	}
	~hashmap() {
		delete [] c;
	}
	my_type& operator=(const my_type& x) {
		if (this != &x) {
			delete [] c;
			
			tblsize = (size_type)(x.length / load), length = 0;
			c = new backet_type [tblsize];
			for (const_iterator it = x.begin(); it != x.end(); ++it)
				insert(*it);
		}
		return *this;
	}
	key_hashcode key_hash() const { return hashcode; }
	size_type size() const { return length; }
	size_type max_size() const { return max(size_type(1), UINT_MAX / sizeof(value_type)); }
	bool empty() const { return length == 0; }
	iterator begin() {
		size_type o = 0;
		while (c[o].size() == 0 && o != tblsize - 1)
			++o;
		return iterator(this, o, c[o].begin());
	}
	const_iterator begin() const {
		size_type o = 0;
		while (c[o].size() == 0 && o != tblsize - 1)
			++o;
		return const_iterator(this, o, ((const backet_type*)c)[o].begin());
	}
	iterator end() { return iterator(this, tblsize - 1, c[tblsize - 1].end()); }
	const_iterator end() const { return const_iterator(this, tblsize - 1, ((const backet_type*)c)[tblsize - 1].end()); }
	reverse_iterator rbegin() { return reverse_iterator(end()); }
	const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
	reverse_iterator rend() { return reverse_iterator(begin()); }
	const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
	
	iterator insert(const value_type& x) {
		if (length >= (float)tblsize * load)
			rehash();
		length++;
		int o = hashcode(x.first) % tblsize;
		c[o].push_back(x);
		backet_type::iterator backetit = c[o].end();
		--backetit;
		return iterator(this, o, backetit);
	}
	void insert(const_iterator begin, const_iterator end) {
		while (begin != end)
			insert(*begin++);
	}
	size_type erase(const key_type& x) {
		int o = hashcode(x) % tblsize;
		int r = 0;
		backet_type::iterator it = c[o].begin();
		while (it != c[o].end())
			if ((*it).first == x) {
				c[o].erase(it);
				length--;
				r++;
			} else
				++it;
		return r;
	}
	void erase(iterator position) {
		c[position.offset].erase(position.it);
		length--;
	}
	void erase(iterator begin, iterator end) {
		while (begin != end)
			erase(begin++);
	}
	void swap(my_type& x) {
		::swap(c, x.c);
		::swap(tblsize, x.tblsize);
		::swap(length, x.length);
	}
	iterator find(const key_type& x) {
		int o = hashcode(x) % tblsize;
		backet_type::iterator backetit;
		for (backetit = c[o].begin(); backetit != c[o].end(); ++backetit)
			if ((*backetit).first == x)
				break;
		return (backetit != c[o].end()) ? iterator(this, o, backetit) : end();
	}
	const_iterator find(const key_type& x) const {
		int o = hashcode(x) % tblsize;
		backet_type::const_iterator backetit;
		for (backetit = ((const backet_type*)c)[o].begin(); backetit != ((const backet_type*)c)[o].end(); ++backetit)
			if ((*backetit).first == x)
				break;
		return (backetit != ((const backet_type*)c)[o].end()) ? const_iterator(this, o, backetit) : end();
	}
	size_type count(const key_type& x) const {
		int o = hashcode(x) % tblsize;
		int r = 0;
		for (backet_type::const_iterator it = c[o].begin(); it != c[o].end(); ++it)
			if ((*it).first == x)
				r++;
		return r;
	}
};

template <class Key, class T, class Hashcode>
bool
operator==(const hashmap<Key, T, Hashcode>& x, const hashmap<Key, T, Hashcode>& y)
{
	bool result;
	
	if (x.size() != y.size()) {
		result = false;
		goto ON_END;
	}
	for (hashmap<Key, T, Hashcode>::const_iterator orgit = x.begin(); orgit != x.end(); ++orgit) {
		hashmap<Key, T, Hashcode>::const_iterator compit = y.find((*orgit).first);
		if (compit == y.end() || (*orgit).second != (*compit).second) {
			result = false;
			goto ON_END;
		}
	}
	result = true;
 ON_END:
	return result;
}

#ifndef PTR_HASHCODE
#define PTR_HASHCODE
template <class T>
struct ptr_hashcode {
	int operator()(const T& x) const {
		return ((int)(void*)x % (sizeof(long double) * 2));
	}
};
#endif

#endif
