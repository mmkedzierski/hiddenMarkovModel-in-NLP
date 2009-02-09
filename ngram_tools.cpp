#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <map>
#include <string>
#include <list>
#include "ngram_tools.h"
using namespace std;


Ngram<3> make_trigram(const string& t1, const string& t2, const string& t3) {
	Ngram<3> ret;
	ret.t[0] = t1;
	ret.t[1] = t2;
	ret.t[2] = t3;
	return ret;
}


Ngram<2> make_bigram(const string& t1, const string& t2) {
	Ngram<2> ret;
	ret.t[0] = t1;
	ret.t[1] = t2;
	return ret;
}


Ngram<1> make_unigram(const string& t1) {
	Ngram<1> ret;
	ret.t[0] = t1;
	return ret;
}
