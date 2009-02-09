#include <cstdio>

int tp, tn, fp, fn;

int main(int argc, char *argv[]) {
	int a,b,c,d;
	while (scanf("%d %d %d %d", &a, &b, &c, &d) == 4) {
		tp += a;
		tn += b;
		fp += c;
		fn += d;
	}

	double r = 1.0 * tp / (tp + fn);
	double p = 1.0 * tp / (tp + fp);
	double f_measure = 2 * r * p / (r + p);
	
	printf("%lf\n", f_measure);

	return 0;
}
