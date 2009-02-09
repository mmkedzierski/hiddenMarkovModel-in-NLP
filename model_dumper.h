#ifndef _MODEL_DUMPER_H_
#define _MODEL_DUMPER_H_

#include <map>
#include "ngram_tools.h"
using namespace std;

// funkcja wczytuje slowniki z pliku
void input_maps
(
	FILE *fin,
	trigram_map_t& tag_trigrams, 
	bigram_map_t& tag_bigrams, 
	unigram_map_t& tag_unigrams, 
	trigram_map_t& word_tag_grams, 
	bigram_map_t& beginner_tag_bigrams
);

// funkcja zapisuje slowniki do pliku
void output_maps
(
	FILE *fout,
	trigram_map_t& tag_trigrams, 
	bigram_map_t& tag_bigrams, 
	unigram_map_t& tag_unigrams, 
	trigram_map_t& word_tag_grams, 
	bigram_map_t& beginner_tag_bigrams
);

// funkcja wczytuje wygladzone czestosci/prawdopodobienstwa nieznanych obiektow
void input_unknowns
(
	FILE *fin,
	double& unknown_tag_trigrams, 
	double& unknown_tag_bigrams, 
	double& unknown_tag_unigrams, 
	double& unknown_word_tag_grams, 
	double& unknown_beginner_tag_bigrams
);

// funkcja zapisuje wygladzone czestosci/prawdopodobienstwa nieznanych obiektow
void output_unknowns
(
	FILE *fout,
	double& unknown_tag_trigrams, 
	double& unknown_tag_bigrams, 
	double& unknown_tag_unigrams, 
	double& unknown_word_tag_grams, 
	double& unknown_beginner_tag_bigrams
);

#endif // _MODEL_DUMPER_H_
