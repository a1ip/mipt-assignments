using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using NUnit.Framework;

namespace merge_sort {
  class MergeSorter {
    private List<int> array;
    private int depth;

    private int left;
    private int right;
    private List<int> buffer = null;

    public MergeSorter(List<int> array, int depth, int left = -1, int right = -1, List<int> buffer = null) {
      this.array = array;
      this.depth = depth;
      if (left == -1) {
        this.left = 0;
      } else {
        this.left = left;
      }
      if (right == -1) {
        this.right = array.Count;
      } else {
        this.right = right;
      }
      if (buffer == null) {
        this.buffer = new List<int>(array);
      } else {
        this.buffer = buffer;
      }
    }

    public void MergeSort() {
      int size = right - left;
      if (size == 0 || size == 1) {
        return;
      }

      int firstSize = size / 2;
      int secondSize = size - firstSize;

      int firstLeft = left;
      int firstRight = left + firstSize;

      int secondLeft = left + firstSize;
      int secondRight = right;

      MergeSorter firstSorter = new MergeSorter(array, depth - 1, firstLeft, firstRight, buffer);
      MergeSorter secondSorter = new MergeSorter(array, depth - 1, secondLeft, secondRight, buffer);

      if (depth > 0) {
        Thread firstThread = new Thread(firstSorter.MergeSort);
        Thread secondThread = new Thread(secondSorter.MergeSort);
        firstThread.Start();
        secondThread.Start();
        firstThread.Join();
        secondThread.Join();
      } else {
        firstSorter.MergeSort();
        secondSorter.MergeSort();
      }

      int first = 0;
      int second = 0;
      for (int i = 0; i < size; i++) {
        if (first == firstSize) {
          buffer[left + i] = array[secondLeft + second++];
          continue;
        }
        if (second == secondSize) {
          buffer[left + i] = array[firstLeft + first++];
          continue;
        }
        if (array[firstLeft + first] <= array[secondLeft + second]) {
          buffer[left + i] = array[firstLeft + first++];
        } else {
          buffer[left + i] = array[secondLeft + second++];
        }
      }

      for (int i = 0; i < size; i++) {
        array[left + i] = buffer[left + i];
      }
    }
  }

  [TestFixture]
  public class MergeSortTests {
    [Test]
    public static void TestSingle() {
      List<int> array = new List<int>();
      array.Add(42);

      MergeSorter sorter = new MergeSorter(array, 1);
      sorter.MergeSort();

      Assert.AreEqual(array[0], 42);
    }

    [Test]
    public static void TestCouple() {
      List<int> array = new List<int>();
      array.Add(123);
      array.Add(-42);

      MergeSorter sorter = new MergeSorter(array, 1);
      sorter.MergeSort();

      Assert.AreEqual(array[0], -42);
      Assert.AreEqual(array[1], 123);
    }

    [Test]
    public static void TestEqual() {
      List<int> array = new List<int>();
      for (int i = 0; i < 100; i++) {
        array.Add(-4242);
      }

      MergeSorter sorter = new MergeSorter(array, 1);
      sorter.MergeSort();

      for (int i = 0; i < 100; i++) {
        Assert.AreEqual(array[i], -4242);
      }
    }

    [Test]
    public static void TestMany() {
      List<int> array = new List<int>();
      for (int i = 0; i < 10000; i++) {
        array.Add((i * 43) % 983);
      }

      MergeSorter sorter = new MergeSorter(array, 1);
      sorter.MergeSort();

      for (int i = 1; i < 10000; i++) {
        Assert.LessOrEqual(array[i - 1], array[i]);
      }
    }

    [Test]
    public static void TestTwice() {
      List<int> array = new List<int>();
      for (int i = 0; i < 10000; i++) {
        array.Add((i * 43) % 983);
      }

      MergeSorter sorter = new MergeSorter(array, 1);
      sorter.MergeSort();
      sorter.MergeSort();

      for (int i = 1; i < 10000; i++) {
        Assert.LessOrEqual(array[i - 1], array[i]);
      }
    }
  }

  class Program {
    static void Main(string[] args) {
      if (args.Length != 1) {
        PrintUsage();
        return;
      }

      int depth;
      if (!Int32.TryParse(args[0], out depth)) {
        PrintUsage();
        return;
      }

      if (depth <= 1) {
        PrintUsage();
        return;
      }

      List<int> array = new List<int>();
      for (int i = 0; i < 100; i++) {
        array.Add(i % 23);
      }

      MergeSorter sorter = new MergeSorter(array, depth - 1);
      sorter.MergeSort();

      for (int i = 0; i < array.Count; i++) {
        Console.Write(array[i] + " ");
      }
      Console.WriteLine();
    }

    static void PrintUsage() {
      Console.WriteLine("Usage: <this app> <depth>");
      Console.WriteLine("<depth> is the number of threads that will run simultaneously.");
    }
  }
}
