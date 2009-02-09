#include <cassert>
#include <cmath>
#include <map>
using namespace std;
#include "generic_tagger.h"
#include "get_prob.h"

struct dyn_array_entry_t;
typedef map<Ngram<2>, dyn_array_entry_t> dyn_array_t;

struct dyn_array_entry_t {
	 double log_prob; // logarytm prawdopodobienstwa
	 dyn_array_t::iterator prev; // optymalny poprzedni tag
};

// tablica dynamicznego algorytmu Viterbiego
// dyn_array[i][(t1,t2)] - wpis dla i-tego slowa w stanie (t1, t2)
// uwaga: nie utrzymujemy wpisow dla i=0
static dyn_array_t *dyn_array;

static Ngram<2> *results;
static int n;

void do_tag
(
	list<word_t>& word_list,
	trigram_map_t& tag_trigrams, 
	bigram_map_t& tag_bigrams,
	unigram_map_t& tag_unigrams,
	trigram_map_t& word_tag_grams, 
	bigram_map_t& beginner_tag_bigrams,
	double unknown_tag_trigrams,
	double unknown_tag_bigrams,
	double unknown_tag_unigrams,
	double unknown_word_tag_grams,
	double unknown_beginner_tag_bigrams
)
{
	dyn_array_entry_t tmp_entry;
	list<word_t>::iterator word_itor;
	dyn_array_t::iterator tmp_itor;
	Ngram<2> state;
	int i;

	n = word_list.size(); 
	if (!n) return;
	dyn_array = new dyn_array_t[n];
	results = new Ngram<2>[n];
	
	
	if (n < 2) {
		assert(n == 1);
		list<string>::iterator best;
		word_t wrd = word_list.front();
		assert(!wrd.tags.empty());
		
		best = wrd.tags.begin();
		for (list<string>::iterator it = wrd.tags.begin();  it != wrd.tags.end();  ++it)
			if (get_tag_unigram_prob(make_unigram(*it)) > get_tag_unigram_prob(make_unigram(*best)))
				best = it;
				
		word_list.front().chosen_tag = *best;
		return;
	}
	
	
	// *** algorytm Viterbiego

	// pierwsze dwa slowa
	word_itor = word_list.begin();
	list<string>& tags_in_1st = word_itor->tags;
	++word_itor;
	list<string>& tags_in_2nd = word_itor->tags;
	for (list<string>::iterator it1 = tags_in_1st.begin();  it1 != tags_in_1st.end();  ++it1) {
		for (list<string>::iterator it2 = tags_in_2nd.begin();  it2 != tags_in_2nd.end();  ++it2) {
			state = make_bigram(*it1, *it2);
			tmp_entry.log_prob = log(get_beginner_tag_prob(state));
 			tmp_entry.prev = dyn_array[0].end();
			dyn_array[1].insert(make_pair(state, tmp_entry));
		}
	}

	// nastepne slowa 
	i = 2;
	for (++word_itor;  i < n;  ++word_itor, ++i) {
		assert(word_itor != word_list.end());
		assert(!word_itor->tags.empty());
		
		list<string>& tags = word_itor->tags;
		for (list<string>::iterator cur_tag = tags.begin();  cur_tag != tags.end();  ++cur_tag) {
			for (dyn_array_t::iterator previous = dyn_array[i - 1].begin();  
				previous != dyn_array[i - 1].end();  ++previous) {
				
				const string& t0 = previous->first.t[0];
				const string& t1 = previous->first.t[1];
				const string& t2 = *cur_tag;
				const string& word = word_itor->word;
				Ngram<2> new_state = make_bigram(t1, t2);
				
				double log_p1 = previous->second.log_prob;
				double p2 = get_tag_trigram_prob(make_trigram(t0, t1, t2));
				double p3 = get_word_tag_gram_prob(make_trigram(t0, t1, word));
				
				tmp_entry.log_prob = log_p1 + log(p2) + log(p3);
				tmp_entry.prev = previous;
				
				tmp_itor = dyn_array[i].find(new_state);
				if (tmp_itor == dyn_array[i].end())
					dyn_array[i].insert(make_pair(new_state, tmp_entry));
				else if (tmp_itor->second.log_prob < tmp_entry.log_prob)
					dyn_array[i][new_state] = tmp_entry;
			}
		}
	}
	// odkodowywanie
	assert(!dyn_array[n-1].empty());
	Ngram<2> best_last_state = dyn_array[n-1].begin()->first;
	double best_prob = 0.0;
	for (dyn_array_t::iterator it = dyn_array[n-1].begin();  it != dyn_array[n-1].end();  ++it) {
		if (it->second.log_prob > log(best_prob)) {
			best_last_state = it->first;
			best_prob = it->second.log_prob;
		}
	}
	results[n-1] = best_last_state;
	for (i = n - 2;  i >= 1;  --i) {
		assert(dyn_array[i+1].find(results[i+1]) != dyn_array[i+1].end());
		results[i] = dyn_array[i+1][results[i+1]].prev->first;
	}
	
	// przepisywanie
	word_itor = word_list.begin();
	word_itor->chosen_tag = results[1].t[0];
	++word_itor;
	for (i = 1; i < n; ++i) {
		word_itor->chosen_tag = results[i].t[1];
		++word_itor;
	}
	
	
	delete[] dyn_array;
	delete[] results;
}
