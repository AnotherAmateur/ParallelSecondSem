using System.Diagnostics;

namespace CS_Lab_2
{
    public class Program
    {
        static void Main(string[] args)
        {
            const int N = 200;
            var X = new double[N + 1];

            for (int n = 0; n <= N; n++)
            {
                X[n] = 100 * Math.Cos(n);
            }

            double S = 0;

            var stopwatch = new Stopwatch();
            stopwatch.Start();

            foreach (var x in X)
            {
                S += FFunction(x);
            }

            stopwatch.Stop();
            Console.WriteLine($"foreach: {stopwatch.Elapsed.TotalSeconds} sec");

            stopwatch = new();
            stopwatch.Start();

            Parallel.ForEach(X, new ParallelOptions { MaxDegreeOfParallelism = 6 }, (x) =>
            {
                double value = FFunction(x);
                lock (stopwatch)
                {
                    S += value;
                }
            });

            stopwatch.Stop();
            Console.WriteLine($"Parallel.ForEach: {stopwatch.Elapsed.TotalSeconds} sec");
        }

        private static double FFunction(double x)
        {
            double sum = 0;
            int iters = (int)Math.Max(20, 20 * Math.Abs(x));

            for (int k = 1; k <= iters; k++)
            {
                for (int j = 1; j <= iters; j++)
                {
                    double numerator = Math.Pow(x, 2) * j * Math.Sin((k + 2 * j) * x);
                    double denominator = Math.Pow(x, 2) + Math.Pow(k, 2) + Math.Pow(j, 3);
                    sum += numerator / denominator;
                }
            }

            return sum;
        }
    }
}