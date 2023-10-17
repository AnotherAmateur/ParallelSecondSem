#include <iostream>
#include <vector>
#include <ppl.h>

using namespace std;

inline double func(int x)
{
	double iters = max(20, (int)(20 * abs(x)));
	double sum = 0;

	for (int k = 1; k < iters; k++)
	{
		for (int j = 1; j < iters; j++)
		{
			sum += pow(x, 2) * j * sin((k + 2 * j) * x) /
				(pow(x, 2) + pow(k, 2) + pow(j, 3));
		}
	}

	return sum;
}

int main()
{
	const int N = 200;

	vector<double> xn(N);
	vector<double> xn_cpy(N);
	for (int i = 1; i <= N; i++)
	{
		xn[i] = 100 * cos(i);
		xn_cpy[i] = xn[i];
	}

	auto t_start = clock();
	for (int i = 0; i < N; i++)
	{
		xn[i] = func(i);
	}
	cout << "plain for cycle: " << (clock() - t_start) / CLOCKS_PER_SEC << " sec. \n\n";

	concurrency::task_group Tg;
	t_start = clock();
	for (int i = 0; i < N; i++)
	{
		Tg.run([&xn_cpy, i]() {
			xn_cpy[i] = func(i);
			});
	}
	Tg.wait();
	cout << "concurrency::task_group: " << (clock() - t_start) / CLOCKS_PER_SEC << " sec. \n\n";
}