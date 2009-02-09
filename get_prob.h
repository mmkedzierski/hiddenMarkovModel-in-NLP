#ifndef _GET_PROB_H_
#define _GET_PROB_H_

#include "ngram_tools.h"

double get_tag_trigram_prob(Ngram<3> trigram);
double get_tag_bigram_prob(Ngram<2> bigram);
double get_tag_unigram_prob(Ngram<1> unigram);
double get_word_tag_gram_prob(Ngram<3> word_tag_gram);
double get_beginner_tag_prob(Ngram<2> bigram);

#endif // _GET_PROB_H_
