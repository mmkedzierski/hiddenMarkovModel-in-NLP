#include "get_prob.h"

extern trigram_map_t tag_trigrams; 
extern bigram_map_t tag_bigrams; 
extern unigram_map_t tag_unigrams; 
extern trigram_map_t word_tag_grams;
extern bigram_map_t beginner_tag_bigrams;

extern double unknown_tag_trigrams;
extern double unknown_tag_bigrams;
extern double unknown_tag_unigrams;
extern double unknown_word_tag_grams;
extern double unknown_beginner_tag_bigrams;


double get_tag_trigram_prob(Ngram<3> trigram) {
	trigram_map_t::iterator itor = tag_trigrams.find(trigram);
	if (itor != tag_trigrams.end())
		return itor->second;
		
	return unknown_tag_trigrams;
}


double get_tag_bigram_prob(Ngram<2> bigram) {
	bigram_map_t::iterator itor = tag_bigrams.find(bigram);
	if (itor != tag_bigrams.end())
		return itor->second;
		
	return unknown_tag_bigrams;
}


double get_tag_unigram_prob(Ngram<1> unigram) {
	unigram_map_t::iterator itor = tag_unigrams.find(unigram);
	if (itor != tag_unigrams.end())
		return itor->second;
		
	return unknown_tag_unigrams;
}


double get_word_tag_gram_prob(Ngram<3> word_tag_gram) {
	trigram_map_t::iterator itor = word_tag_grams.find(word_tag_gram);
	if (itor != word_tag_grams.end())
		return itor->second;
		
	return unknown_word_tag_grams;
}


double get_beginner_tag_prob(Ngram<2> bigram) {
	bigram_map_t::iterator itor = beginner_tag_bigrams.find(bigram);
	if (itor != beginner_tag_bigrams.end())
		return itor->second;
		
	return unknown_beginner_tag_bigrams;
}
