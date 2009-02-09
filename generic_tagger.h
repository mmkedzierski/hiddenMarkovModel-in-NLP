#ifndef _GENERIC_TAGGER_H_
#define _GENERIC_TAGGER_H_

#include <string>
#include <list>
#include "ngram_tools.h"
using namespace std;

struct word_t {
	string word;
	list<string> tags;
	string chosen_tag;
};

// wykonuje wlasciwe tagowanie, tzn. ustawia chosen_tag we wszystkich
// elementach listy word_list
void do_tag
(
	list<word_t>& word_list,
	trigram_map_t& tag_trigrams, 
	bigram_map_t& tag_bigrams,
	unigram_map_t& tag_unigrams,
	trigram_map_t& word_tag_grams, 
	bigram_map_t& unknown_beginner_tag_bigrams,
	double unknown_tag_trigrams,
	double unknown_tag_bigrams,
	double unknown_tag_unigrams,
	double unknown_word_tag_grams,
	double unknown_beginner_tag_bigrams
);

#endif // _GENERIC_TAGGER_H_
