#include <iostream>
#include <vector>
#include <ppl.h>

using namespace std;

inline double x_gen_fun(int n)
{
	double a = pow(n, 2);
	double b = cos(n);
	double c = (1 + pow(n, 2));

	return 4e9 * a * b / c;
}

int main()
{
	const int N = 50000000;

	vector<size_t> xn(N);
	vector<size_t> xn_cpy(N);
	for (int i = 0; i < N; i++)
	{
		xn[i] = (size_t)x_gen_fun(i);
		xn_cpy[i] = xn[i];
	}

	auto t_start = clock();
	sort(xn_cpy.begin(), xn_cpy.end());
	cout << "std::sort: " << (clock() - t_start) / CLOCKS_PER_SEC << " sec. \n\n";

	copy(xn.begin(), xn.end(), xn_cpy.begin());

	t_start = clock();
	concurrency::parallel_sort(xn_cpy.begin(), xn_cpy.end());
	cout << "concurrency::parallel_sort: " << (clock() - t_start) / CLOCKS_PER_SEC << " sec. \n\n";

	copy(xn.begin(), xn.end(), xn_cpy.begin());

	t_start = clock();
	concurrency::parallel_buffered_sort(xn_cpy.begin(), xn_cpy.end());
	cout << "concurrency::parallel_buffered_sort: " << (clock() - t_start) / CLOCKS_PER_SEC << " sec. \n\n";

	copy(xn.begin(), xn.end(), xn_cpy.begin());

	t_start = clock();
	concurrency::parallel_radixsort(xn_cpy.begin(), xn_cpy.end());
	cout << "concurrency::parallel_radixsort: " << (clock() - t_start) / CLOCKS_PER_SEC << " sec. \n\n";
}