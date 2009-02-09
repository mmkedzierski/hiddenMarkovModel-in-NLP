#ifndef _NGRAM_TOOLS_H_
#define _NGRAM_TOOLS_H_

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <map>
#include <string>
#include <list>
using namespace std;

template <int N>
class Ngram {
		public:
	string t[N];

	Ngram() {}
	bool operator<(const Ngram<N>& right) const {
		int res;
		for (int i=0; i<N; ++i) {
			res = strcmp(t[i].c_str(), right.t[i].c_str());
			if (res < 0) return true;
			if (res > 0) return false;
		}
		return false;
	}
};

typedef map<Ngram<3>, double> trigram_map_t;
typedef map<Ngram<2>, double> bigram_map_t;
typedef map<Ngram<1>, double> unigram_map_t;

Ngram<3> make_trigram(const string& t1, const string& t2, const string& t3);
Ngram<2> make_bigram(const string& t1, const string& t2);
Ngram<1> make_unigram(const string& t1);

// zwieksza wpis w slowniku o jeden, tworzac go, jesli jeszcze nie istnieje
template <int N>
void incr_in_map(map<Ngram<N>, double>& m, const Ngram<N>& key) {
	typename map<Ngram<N>, double>::iterator itor;
	itor = m.find(key);
	if (itor == m.end())
		m.insert(make_pair(key, 1));
	else
		++itor->second;
}

#endif // _NGRAM_TOOLS_H_
