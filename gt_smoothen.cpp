#include "tokenizer.h"
#include "ngram_tools.h"
#include "model_dumper.h"
#include <cmath>
#include <vector>

#define DEFAULT_THRESHOLD 5 // ile pierwszych czestosci wygladzamy?

#define update_freq() 		\
	do { \
		if (freq.size() <= itor->second) { \
				fflush(stdout); \
				freq.resize((int)ceil(itor->second) + 1); \
				freq[(int)itor->second] = 0; \
		} \
		++freq[(int)itor->second]; \
	} while (0)


trigram_map_t tag_trigrams; // c(t1 t2 t3) 
bigram_map_t tag_bigrams; // c(t1 t2)
unigram_map_t tag_unigrams; // c(t1)
trigram_map_t word_tag_grams; // c(t1 t2 w2)
bigram_map_t beginner_tag_bigrams;

vector<long long> freq(1); // frequencies of frequencies
vector<double> c_star;

double unknown_tag_trigrams;
double unknown_tag_bigrams;
double unknown_tag_unigrams;
double unknown_word_tag_grams;
double unknown_beginner_tag_bigrams;

long long dict_size;
long long tagset_size;



/** przeksztalc freq na c_star
 * @param space_size liczba wszystkich mozliwosci
 */
void freq_to_c_star(long long space_size) {
	c_star.resize(freq.size());
	
	assert(freq[1]);
	freq[0] = space_size;
	for (int i=1; i<freq.size(); ++i)
		freq[0] -= freq[i];
	assert(freq[0] > 0);
	
	int k = DEFAULT_THRESHOLD;
	for (int c=0; c<k; ++c)
		c_star[c] = (
				((c+1) * (double) freq[c+1] / freq[c] - c * (k + 1) * (double) freq[k+1] / freq[0])
				/ (1 - (k+1) * (double) freq[k+1] / freq[0]) 
			);
			
	for (int c=k; c<freq.size(); ++c)
		c_star[c] = c;
}


int main(int argc, char *argv[]) {
	FILE *fout, *fin;
	bool res;
	
	if (argc != 5) {
		printf("Uzycie: gt_smoothen <dict_size> <tagset_size> <input> <output>\n"
				"gdzie: dict_size = rozmiar slownika (liczba slow - <orth>)\n");
		return 1;
	}
	
	sscanf(argv[1], "%lld", &dict_size);
	sscanf(argv[2], "%lld", &tagset_size);
	fin = fopen(argv[3], "r");
	fout = fopen(argv[4], "w");
	assert(fout != NULL && fin != NULL);
	
	printf("\nProcessing '%s'...", argv[3]);
	fflush(stdout);
	
	input_maps(fin, tag_trigrams, tag_bigrams, tag_unigrams, word_tag_grams, beginner_tag_bigrams);
	
	// *** wygladzanie tag_trigrams
	
	freq.clear();
	for (trigram_map_t::iterator itor = tag_trigrams.begin();  itor != tag_trigrams.end();  ++itor)
		update_freq();
	freq_to_c_star(tagset_size * tagset_size * tagset_size);
	for (trigram_map_t::iterator itor = tag_trigrams.begin();  itor != tag_trigrams.end();  ++itor)
		itor->second = c_star[(int)itor->second];
	unknown_tag_trigrams = c_star[0];
	
	
	// *** wygladzanie tag_bigrams
	
	freq.clear();
	for (bigram_map_t::iterator itor = tag_bigrams.begin();  itor != tag_bigrams.end();  ++itor)
		update_freq();
	freq_to_c_star(tagset_size * tagset_size);
	for (bigram_map_t::iterator itor = tag_bigrams.begin();  itor != tag_bigrams.end();  ++itor)
		itor->second = c_star[(int)itor->second];
	unknown_tag_bigrams = c_star[0];
	
	
	// *** wygladzanie tag_unigrams
	
	freq.clear();
	for (unigram_map_t::iterator itor = tag_unigrams.begin();  itor != tag_unigrams.end();  ++itor)
		update_freq();
	freq_to_c_star(tagset_size);
	for (unigram_map_t::iterator itor = tag_unigrams.begin();  itor != tag_unigrams.end();  ++itor)
		itor->second = c_star[(int)itor->second];
	unknown_tag_unigrams = c_star[0];
	
	
	// *** wygladzanie word_tag_grams
	
	freq.clear();
	for (trigram_map_t::iterator itor = word_tag_grams.begin();  itor != word_tag_grams.end();  ++itor)
		update_freq();
	freq_to_c_star(dict_size * tagset_size * tagset_size);
	for (trigram_map_t::iterator itor = word_tag_grams.begin();  itor != word_tag_grams.end();  ++itor)
		itor->second = c_star[(int)itor->second];
	unknown_word_tag_grams = c_star[0];
	
	
	// *** wygladzanie beginner_tag_bigrams
	
	freq.clear();
	for (bigram_map_t::iterator itor = beginner_tag_bigrams.begin();  itor != beginner_tag_bigrams.end();  ++itor)
		update_freq();
	freq_to_c_star(tagset_size * tagset_size);
	for (bigram_map_t::iterator itor = beginner_tag_bigrams.begin();  itor != beginner_tag_bigrams.end();  ++itor)
		itor->second = c_star[(int)itor->second];
	unknown_beginner_tag_bigrams = c_star[0];
	
	
	// output
	output_unknowns(fout, unknown_tag_trigrams, unknown_tag_bigrams, unknown_tag_unigrams,
		unknown_word_tag_grams, unknown_beginner_tag_bigrams);
 	output_maps(fout, tag_trigrams, tag_bigrams, tag_unigrams, word_tag_grams, beginner_tag_bigrams);
		
	fclose(fout);
	
	printf("\n");
	
	return 0;
}
