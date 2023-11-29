using System.Diagnostics;

namespace CS_Lab_1
{
    public class Program
    {
        static void Main(string[] args)
        {
            const int N = 200;
            var Y = new double[N + 1];

            var stopwatch = new Stopwatch();
            stopwatch.Start();

            for (int n = 0; n <= N; n++)
            {
                double x = 100 * Math.Cos(n);
                Y[n] = FFunction(x);
            }

            stopwatch.Stop();
            Console.WriteLine($"for: {stopwatch.Elapsed.TotalSeconds} sec");

            stopwatch = new Stopwatch();
            stopwatch.Start();

            Parallel.For(0, N + 1, new ParallelOptions { MaxDegreeOfParallelism = 5 }, (int n) =>
            {
                double x = 100 * Math.Cos(n);
                Y[n] = FFunction(x);
            });

            stopwatch.Stop();
            Console.WriteLine($"Parallel.For: {stopwatch.Elapsed.TotalSeconds} sec");
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