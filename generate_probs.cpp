#include "ngram_tools.h"
#include "model_dumper.h"
#include "get_prob.h"

trigram_map_t tag_trigrams; // c(t1 t2 t3) 
bigram_map_t tag_bigrams; // c(t1 t2)
unigram_map_t tag_unigrams; // c(t1)
trigram_map_t word_tag_grams; // c(t1 t2 w2)
bigram_map_t beginner_tag_bigrams;

trigram_map_t new_tag_trigrams; // c(t1 t2 t3) 
bigram_map_t new_tag_bigrams; // c(t1 t2)
unigram_map_t new_tag_unigrams; // c(t1)
trigram_map_t new_word_tag_grams; // c(t1 t2 w2)
bigram_map_t new_beginner_tag_bigrams;

double unknown_tag_trigrams;
double unknown_tag_bigrams;
double unknown_tag_unigrams;
double unknown_word_tag_grams;
double unknown_beginner_tag_bigrams;

double new_unknown_tag_trigrams;
double new_unknown_tag_bigrams;
double new_unknown_tag_unigrams;
double new_unknown_word_tag_grams;
double new_unknown_beginner_tag_bigrams;

long long dict_size;
long long tagset_size;

double gamma[3];


// dla kazdego trigramu abc zamienia jego czestosc (w nowej mapie) na c(abc) / c(ab)
void P_for_trigrams(trigram_map_t& new_map) {
	double cnt1, cnt2;
	bigram_map_t::iterator b_itor; 
	
 	for (trigram_map_t::iterator itor = new_map.begin(); itor != new_map.end(); ++itor) {
 		const string t[3] = itor->first.t;
 		cnt1 = itor->second;
 		b_itor = tag_bigrams.find(make_bigram(t[0], t[1]));
 		if (b_itor != tag_bigrams.end())
 			cnt2 = b_itor->second;
 		else
 			cnt2 = unknown_tag_bigrams;
		assert(cnt2 != 0.0);
 		itor->second = cnt1 / cnt2; // P(t3 | t1 t2)
 		// moze sie zdarzyc cnt1/cnt2 > 1 po wygladzaniu - w takim wypadku przed wygladzaniem
 		// ten iloraz czestosci byl rowny 1
 		itor->second = min(itor->second, 1.0); 
 		assert(0.0 < itor->second);
 	}
}


// dla kazdego unigramu a zamienia jego czestosc (w nowej mapie) na c(a) / <suma wszystkich c(i)>
void P_for_bigrams(bigram_map_t& new_map) {
	double cnt1, cnt2;
	unigram_map_t::iterator u_itor; 
	
 	for (bigram_map_t::iterator itor = new_map.begin(); itor != new_map.end(); ++itor) {
 		const string t[2] = itor->first.t;
 		cnt1 = itor->second;
 		u_itor = tag_unigrams.find(make_unigram(t[0]));
 		if (u_itor != tag_unigrams.end())
 			cnt2 = u_itor->second;
 		else
 			cnt2 = unknown_tag_unigrams;
		assert(cnt2 != 0.0);
 		itor->second = cnt1 / cnt2; // P(t2 | t1)
 		// moze sie zdarzyc cnt1/cnt2 > 1 po wygladzaniu - w takim wypadku przed wygladzaniem
 		// ten iloraz czestosci byl rowny 1
 		itor->second = min(itor->second, 1.0); 
 		assert(0.0 < itor->second);
 	}
}


// dla kazdego unigramu a zamienia jego czestosc (w nowej mapie) na c(a) / <suma wszystkich c(i)>
void P_for_unigrams(unigram_map_t& new_map) {
 	double sum = 0.0;
 	
	for (unigram_map_t::iterator itor = new_map.begin();  itor != new_map.end();  ++itor)
		sum += itor->second;
	
	for (unigram_map_t::iterator itor = new_map.begin();  itor != new_map.end();  ++itor) {
		itor->second /= sum;
		assert(0.0 < itor->second && itor->second <= 1.0);
	}
}


// dla kazdego bigramu ab zamienia jego czestosc (w nowej mapie) na c(ab) / <suma wszystkich c(ij)>
void P_for_beginner_tag_bigrams(bigram_map_t& new_map) {
 	double sum = 0.0;
 	
	for (bigram_map_t::iterator itor = new_map.begin();  itor != new_map.end();  ++itor)
		sum += itor->second;
	
	for (bigram_map_t::iterator itor = new_map.begin();  itor != new_map.end();  ++itor) {
		itor->second /= sum;
		assert(0.0 < itor->second && itor->second <= 1.0);
	}
}



int main(int argc, char *argv[]) {
	FILE *fout, *fin, *fgammas;
	
	if (argc != 6) {
		printf("Uzycie: generate_probs <dict_size> <tagset_size> <gammas> <input> <output>\n"
				"gdzie: dict_size = rozmiar slownika (liczba slow - <orth>)\n"
				"       gammas - plik z parametrami interpolacji liniowej\n");
		return 1;
	}
	
	sscanf(argv[1], "%lld", &dict_size);
	sscanf(argv[2], "%lld", &tagset_size);
	fgammas = fopen(argv[3], "r");
	fin = fopen(argv[4], "r");
	fout = fopen(argv[5], "w");
	assert(fout != NULL && fin != NULL);
	
	printf("\nProcessing '%s'...", argv[4]);
	fflush(stdout);
	
	// input
	fscanf(fgammas, "%lf%lf%lf", &gamma[0], &gamma[1], &gamma[2]);
	input_unknowns(fin, unknown_tag_trigrams, unknown_tag_bigrams, unknown_tag_unigrams, 
		unknown_word_tag_grams, unknown_beginner_tag_bigrams);
	input_maps(fin, tag_trigrams, tag_bigrams, tag_unigrams, word_tag_grams, beginner_tag_bigrams);
	
	
	// przeksztalcanie liczb wystapien c() na prawdopodobienstwa P()
	
	new_tag_trigrams = tag_trigrams;
	P_for_trigrams(new_tag_trigrams);
	
	new_tag_bigrams = tag_bigrams;
 	P_for_bigrams(new_tag_bigrams);
 	
 	new_tag_unigrams = tag_unigrams;
 	P_for_unigrams(new_tag_unigrams);
	
	new_word_tag_grams = word_tag_grams;
	P_for_trigrams(new_word_tag_grams);
 	
 	new_beginner_tag_bigrams = beginner_tag_bigrams;
 	P_for_beginner_tag_bigrams(new_beginner_tag_bigrams);
 	
	new_unknown_tag_trigrams = unknown_tag_trigrams / unknown_tag_bigrams;
	new_unknown_tag_bigrams = unknown_tag_bigrams / unknown_tag_unigrams;
	new_unknown_tag_unigrams = unknown_tag_unigrams;
	new_unknown_word_tag_grams = unknown_word_tag_grams / unknown_tag_unigrams;
	new_unknown_beginner_tag_bigrams = unknown_beginner_tag_bigrams;
 	
 	
 	// przeksztalcanie prawdopodobienstw P() na P*() - interpolacja liniowa dla trigamow tagow
 	
	tag_trigrams = new_tag_trigrams;
	tag_bigrams = new_tag_bigrams;
 	tag_unigrams = new_tag_unigrams;
	word_tag_grams = new_word_tag_grams;
 	beginner_tag_bigrams = new_beginner_tag_bigrams;
 	
 	for (trigram_map_t::iterator itor = new_tag_trigrams.begin(); itor != new_tag_trigrams.end(); ++itor) {
 		const string t[3] = itor->first.t;
 		itor->second = gamma[0] * get_tag_trigram_prob(itor->first) 
 			+ gamma[1] * get_tag_bigram_prob(make_bigram(t[1], t[2])) 
 			+ gamma[2] * get_tag_unigram_prob(make_unigram(t[2]));
 	}
	
	
	// output
	output_unknowns(fout, new_unknown_tag_trigrams, new_unknown_tag_bigrams, new_unknown_tag_unigrams, 
		new_unknown_word_tag_grams, new_unknown_beginner_tag_bigrams);
 	output_maps(fout, new_tag_trigrams, new_tag_bigrams, new_tag_unigrams, new_word_tag_grams, new_beginner_tag_bigrams);
		
	fclose(fout);
	
	printf("\n");
	
	return 0;
}
