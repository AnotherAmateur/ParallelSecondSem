#include <iostream>
#include <cmath>
#include <ppl.h>
#include <vector>
#include <deque>
#include <list>

using namespace std;


const double MULTIPLIER = 100;
const int N = 200;

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

void Print(vector<double> y) {

	for (int i = 0; i < y.size(); i++)
	{
		cout << i << ". " << y[i] << endl;
	}
}

void Print(list<double> y) {

	int i = 0;
	for (auto it : y)
	{
		cout << i << ". " << it << endl;
		i++;
	}
}

void Print(deque<double> y) {

	for (int i = 0; i < y.size(); i++)
	{
		cout << i << ". " << y[i] << endl;
	}
}


int main()
{
	vector<double> vec;
	list<double> lst;
	deque<double> deq;

	for (int i = 1; i <= N; i++)
	{
		double x = MULTIPLIER * cos(i);
		vec.push_back(x);
		lst.push_back(x);
		deq.push_back(x);
	}

	vector<double> vec_cpy(vec);
	list<double> lst_cpy(lst);
	deque<double> deq_cpy(deq);

	auto t_start = clock();
	for_each(vec.begin(), vec.end(), [](double& x) { x = Function(x); });
	cout << "Vector/for_each: " << (clock() - t_start) / CLOCKS_PER_SEC << " sec. \n\n";

	t_start = clock();
	for_each(lst.begin(), lst.end(), [](double& x) { x = Function(x); });
	cout << "List/for_each: " << (clock() - t_start) / CLOCKS_PER_SEC << " sec. \n\n";

	t_start = clock();
	for_each(deq.begin(), deq.end(), [](double& x) { x = Function(x); });
	cout << "Deque/for_each: " << (clock() - t_start) / CLOCKS_PER_SEC << " sec. \n\n";


	t_start = clock();
	concurrency::parallel_for_each(vec_cpy.begin(), vec_cpy.end(), [](double& x) { x = Function(x); });
	cout << "Vector/for_each_parallel: " << (clock() - t_start) / CLOCKS_PER_SEC << " sec. \n\n";

	t_start = clock();
	concurrency::parallel_for_each(lst_cpy.begin(), lst_cpy.end(), [](double& x) { x = Function(x); });
	cout << "List/for_each_parallel: " << (clock() - t_start) / CLOCKS_PER_SEC << " sec. \n\n";

	t_start = clock();
	concurrency::parallel_for_each(deq_cpy.begin(), deq_cpy.end(), [](double& x) { x = Function(x); });
	cout << "Deque/for_each_parallel: " << (clock() - t_start) / CLOCKS_PER_SEC << " sec. \n\n";
}