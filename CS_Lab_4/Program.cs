using System.Diagnostics;

namespace CS_Lab_4
{
    public class Program
    {
        private const int N = 200;

        static void Main(string[] args)
        {
            var XList = new List<double>(N + 1);
            var XStack = new Stack<double>(N + 1);
            var XQueue = new Queue<double>(N + 1);

            var YList = new List<double>(N + 1);
            var YStack = new Stack<double>(N + 1);
            var YQueue = new Queue<double>(N + 1);

            for (int n = 0; n <= N; n++)
            {
                double x = 100 * Math.Cos(n);
                XList.Add(x);
                XStack.Push(x);
                XQueue.Enqueue(x);
            }

            double time;

            Console.WriteLine($"foreach List:  {TransformSerialList(XList, YList)} sec");
            Console.WriteLine($"foreach Stack: {TransformSerialStack(XStack, YStack)} sec");
            Console.WriteLine($"foreach Queue: {TransformSerialQueue(XQueue, YQueue)} sec");
            Console.WriteLine();

            YList = new();
            YStack = new();
            YQueue = new();

            Console.WriteLine($"Parallel.Foreach List:  {TransformConcurrentList(XList, YList)} sec");
            Console.WriteLine($"Parallel.Foreach Stack: {TransformConcurrentStack(XStack, YStack)} sec");
            Console.WriteLine($"Parallel.Foreach Queue: {TransformConcurrentQueue(XQueue, YQueue)} sec");
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

        private static double TransformSerialList(List<double> collectionSource, List<double> collectionDest)
        {
            var stopwatch = new Stopwatch();
            stopwatch.Start();

            foreach (var x in collectionSource)
            {
                double value = FFunction(x);
                collectionDest.Add(value);
            }

            stopwatch.Stop();
            return stopwatch.Elapsed.Seconds;
        }

        private static double TransformSerialQueue(Queue<double> collectionSource, Queue<double> collectionDest)
        {
            var stopwatch = new Stopwatch();
            stopwatch.Start();

            foreach (var x in collectionSource)
            {
                double value = FFunction(x);
                collectionDest.Enqueue(value);
            }

            stopwatch.Stop();
            return stopwatch.Elapsed.Seconds;
        }

        private static double TransformSerialStack(Stack<double> collectionSource, Stack<double> collectionDest)
        {
            var stopwatch = new Stopwatch();
            stopwatch.Start();

            foreach (var x in collectionSource)
            {
                double value = FFunction(x);
                collectionDest.Push(value);
            }

            stopwatch.Stop();
            return stopwatch.Elapsed.Seconds;
        }

        private static double TransformConcurrentList(List<double> collectionSource, List<double> collectionDest)
        {
            var stopwatch = new Stopwatch();
            stopwatch.Start();

            Parallel.ForEach(collectionSource, new ParallelOptions { MaxDegreeOfParallelism = 6 }, (x) =>
            {
                double value = FFunction(x);
                lock (stopwatch)
                {
                    collectionDest.Add(value);
                }
            });

            stopwatch.Stop();
            return stopwatch.Elapsed.Seconds;
        }

        private static double TransformConcurrentQueue(Queue<double> collectionSource, Queue<double> collectionDest)
        {
            var stopwatch = new Stopwatch();
            stopwatch.Start();

            Parallel.ForEach(collectionSource, new ParallelOptions { MaxDegreeOfParallelism = 6 }, (x) =>
            {
                double value = FFunction(x);
                lock (stopwatch)
                {
                    collectionDest.Enqueue(value);
                }
            });

            stopwatch.Stop();
            return stopwatch.Elapsed.Seconds;
        }

        private static double TransformConcurrentStack(Stack<double> collectionSource, Stack<double> collectionDest)
        {
            var stopwatch = new Stopwatch();
            stopwatch.Start();

            Parallel.ForEach(collectionSource, new ParallelOptions { MaxDegreeOfParallelism = 6 }, (x) =>
            {
                double value = FFunction(x);
                lock (stopwatch)
                {
                    collectionDest.Push(value);
                }
            });

            stopwatch.Stop();
            return stopwatch.Elapsed.Seconds;
        }
    }
}