using System.Diagnostics;

namespace CS_Lab_3
{
    public class Program
    {
        private const int N = 200;

        static void Main(string[] args)
        {
            var XList = new List<double>(N + 1);
            var XStack = new Stack<double>(N + 1);
            var XQueue = new Queue<double>(N + 1);

            for (int n = 0; n <= N; n++)
            {
                double x = 100 * Math.Cos(n);
                XList.Add(x);
                XStack.Push(x);
                XQueue.Enqueue(x);
            }

            double time;

            Console.WriteLine($"foreach List:  S = {CalculateSumSerial(XList, out time)},  time = {time} sec");
            Console.WriteLine($"foreach Stack: S = {CalculateSumSerial(XStack, out time)}, time = {time} sec");
            Console.WriteLine($"foreach Queue: S = {CalculateSumSerial(XQueue, out time)}, time = {time} sec");
            Console.WriteLine();
            Console.WriteLine($"Parallel.Foreach List:  S = {CalculateSumConcurrent(XList, out time)},  time = {time} sec");
            Console.WriteLine($"Parallel.Foreach Stack: S = {CalculateSumConcurrent(XStack, out time)}, time = {time} sec");
            Console.WriteLine($"Parallel.Foreach Queue: S = {CalculateSumConcurrent(XQueue, out time)}, time = {time} sec");
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

        private static double CalculateSumSerial(IEnumerable<double> collection, out double time)
        {
            double S = 0;
            var stopwatch = new Stopwatch();
            stopwatch.Start();

            foreach (var x in collection)
            {
                S += FFunction(x);
            }

            stopwatch.Stop();
            time = stopwatch.Elapsed.Seconds;

            return S;
        }

        private static double CalculateSumConcurrent(IEnumerable<double> collection, out double time)
        {
            double S = 0;
            var stopwatch = new Stopwatch();
            stopwatch.Start();

            Parallel.ForEach(collection, new ParallelOptions { MaxDegreeOfParallelism = 6 }, (x) =>
            {
                double value = FFunction(x);
                lock (stopwatch)
                {
                    S += value;
                }
            });

            stopwatch.Stop();
            time = stopwatch.Elapsed.Seconds;

            return S;
        }
    }
}