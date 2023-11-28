#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <queue>
#include <ppl.h>
#include <concurrent_vector.h>
#include <concurrent_queue.h>
#include <cmath>

using namespace std;

const int N1 = 400;
const int N2 = 50;
const double H = 2 * M_PI / N1;
const double EPS = 1e-9;

inline double func(double x, double y)
{
	double sum = 0;

	for (int k = 1; k < N2; k++)
	{
		for (int j = 1; j < N2; j++)
		{
			double numerator = cos(k * x) * sin(j * y);
			double denominator = (1 + k + j) * sqrt(1 + pow(k, 4) + pow(j, 4));
			sum += numerator / denominator;
		}
	}

	return sum;
}

struct PointVal {
	double x;
	double y;
	double f_value;

	PointVal(double x, double y, double f_value)
		: x(x), y(y), f_value(f_value) { }
};

double calculate_dfdy(double x, double y) {
	double h = 1e-5;
	return (func(x, y + h) - func(x, y - h)) / (2 * h);
}


int main()
{
	const int req_collection_size = N1 * N1;
	vector<PointVal> plain_vec;
	queue<PointVal> plain_que;
	concurrency::concurrent_vector<PointVal> conc_vec;
	concurrency::concurrent_queue<PointVal> conc_que;

	auto t_start = clock();
	for (int k = 0; k < N1; k++)
	{
		for (int j = 1; j < N1; j++)
		{
			double x = H * k;
			double y = H * j;
			double f_value = func(x, y);
			if (calculate_dfdy(x, y) + EPS < 0)
			{
				plain_vec.push_back(PointVal(x, y, f_value));
			}
		}
	}
	cout << "Single thread vector: " << (clock() - t_start) / CLOCKS_PER_SEC << " sec. \n\n";

	t_start = clock();
	for (int k = 0; k < N1; k++)
	{
		for (int j = 1; j < N1; j++)
		{
			double x = H * k;
			double y = H * j;
			double f_value = func(x, y);
			if (calculate_dfdy(x, y) + EPS < 0)
			{
				plain_que.push(PointVal(x, y, f_value));
			}
		}
	}
	cout << "Single thread queue: " << (clock() - t_start) / CLOCKS_PER_SEC << " sec. \n\n";

	t_start = clock();
	concurrency::parallel_for(0, N1, [&conc_vec](int k) {
		for (int j = 0; j < N1; j++)
		{
			double x = H * k;
			double y = H * j;
			double f_value = func(x, y);
			if (calculate_dfdy(x, y) + EPS < 0)
			{
				conc_vec.push_back(PointVal(x, y, f_value));
			}
		}
		});
	cout << "Parallel_for + concurrent_vector: " << (clock() - t_start) / CLOCKS_PER_SEC << " sec. \n\n";

	t_start = clock();
	concurrency::parallel_for(0, N1, [&conc_que](int k) {
		for (int j = 0; j < N1; j++)
		{
			double x = H * k;
			double y = H * j;
			double f_value = func(x, y);
			if (calculate_dfdy(x, y) + EPS < 0)
			{
				conc_que.push(PointVal(x, y, f_value));
			}
		}
		});
	cout << "Parallel_for + concurrent_queue: " << (clock() - t_start) / CLOCKS_PER_SEC << " sec. \n\n";
}