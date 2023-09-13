#include <iostream>
#include <cmath>
#include <ppl.h>

using namespace std;

double Function(double x) {

	double iters = max(20., floor(20. * abs(x)));
	double sum = 0;

	for (int k = 0; k < iters; k++)
	{
		for (int j = 0; j < iters; j++)
		{
			sum += pow(x, 2) * j * sin((k + 2 * j) * x) /
				(pow(x, 2) + pow(k, 2) + pow(j, 3));
		}
	}

	return sum;
}

void Print(double* y, int len) {

	for (int i = 0; i < len; i++)
	{
		cout << i << ". " << y[i] << endl;
	}
}


int main()
{
	const double MULTIPLIER = 100;
	const int N = 2000;
	double* y = new double[N];

	auto t_start = clock();

	for (int n = 0; n < N; n++)
	{
		y[n] = Function(MULTIPLIER * cos(n + 1));
	}

	cout << "Solo thread duration: " << (clock() - t_start) / CLOCKS_PER_SEC << " sec. \n\n";


	t_start = clock();
	Concurrency::parallel_for(0, N,
		[&y, MULTIPLIER](int n) { y[n] = Function(MULTIPLIER * cos(n + 1)); });

	cout << "Multi thread duration: " << (clock() - t_start) / CLOCKS_PER_SEC << " sec. \n\n";

	Print(y, N);
}
