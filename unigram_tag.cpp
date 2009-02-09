#include <cassert>
#include <cmath>
#include <map>
using namespace std;
#include "generic_tagger.h"
#include "get_prob.h"


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
	list<string>::iterator best;
	for (list<word_t>::iterator itor = word_list.begin();  itor != word_list.end();  ++itor) {
		assert(!itor->tags.empty());
		
		best = itor->tags.begin();
		for (list<string>::iterator it = itor->tags.begin();  it != itor->tags.end();  ++it)
			if (get_tag_unigram_prob(make_unigram(*it)) > get_tag_unigram_prob(make_unigram(*best)))
				best = it;
				
		itor->chosen_tag = *best;
	}
}
