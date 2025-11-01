using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;

public class AVLTree
{
    private class Node
    {
        public int Key;
        public int[,] Matrix;
        public int Height;
        public Node? Left;
        public Node? Right;

        public Node(int key, int m)
        {
            Key = key;
            Height = 1;

            int side = (int)Math.Sqrt(m);
            Matrix = new int[side, side];

            // Touch the memory to ensure real allocation
            Matrix[0, 0] = key;
            Matrix[side - 1, side - 1] = key;
        }
    }

    private Node? root;

    private static int HeightOf(Node? node) => node == null ? 0 : node.Height;
    private static int BalanceFactor(Node? node) => node == null ? 0 : HeightOf(node.Left) - HeightOf(node.Right);

    private static Node RotateRight(Node y)
    {
        Node x = y.Left!;
        Node t2 = x.Right!;

        x.Right = y;
        y.Left = t2;

        y.Height = Math.Max(HeightOf(y.Left), HeightOf(y.Right)) + 1;
        x.Height = Math.Max(HeightOf(x.Left), HeightOf(x.Right)) + 1;

        return x;
    }

    private static Node RotateLeft(Node x)
    {
        Node y = x.Right!;
        Node t2 = y.Left!;

        y.Left = x;
        x.Right = t2;

        x.Height = Math.Max(HeightOf(x.Left), HeightOf(x.Right)) + 1;
        y.Height = Math.Max(HeightOf(y.Left), HeightOf(y.Right)) + 1;

        return y;
    }

    private static Node Balance(Node node)
    {
        int balanceFactor = BalanceFactor(node);

        if (balanceFactor > 1)
        {
            if (BalanceFactor(node.Left) < 0)
                node.Left = RotateLeft(node.Left!);
            return RotateRight(node);
        }
        if (balanceFactor < -1)
        {
            if (BalanceFactor(node.Right) > 0)
                node.Right = RotateRight(node.Right!);
            return RotateLeft(node);
        }

        return node;
    }

    private static Node? Insert(Node? root, int key, int m)
    {
        if (root == null)
            return new Node(key, m);

        if (key < root.Key)
            root.Left = Insert(root.Left, key, m);
        else if (key > root.Key)
            root.Right = Insert(root.Right, key, m);
        else
            return root;

        root.Height = Math.Max(HeightOf(root.Left), HeightOf(root.Right)) + 1;
        return Balance(root);
    }

    private static Node MinValueNode(Node root)
    {
        Node current = root;
        while (current.Left != null)
            current = current.Left;
        return current;
    }

    private static Node? Delete(Node? root, int key)
    {
        if (root == null)
            return root!;

        if (key < root.Key)
            root.Left = Delete(root.Left, key);
        else if (key > root.Key)
            root.Right = Delete(root.Right, key);
        else
        {
            if (root.Left == null || root.Right == null)
            {
                Node? temp = root.Left ?? root.Right;
                return temp!;
            }
            else
            {
                Node minNode = MinValueNode(root.Right!);
                root.Key = minNode.Key;
                root.Matrix = minNode.Matrix;
                root.Right = Delete(root.Right, minNode.Key);
            }
        }

        if (root == null)
            return root!;

        root.Height = Math.Max(HeightOf(root.Left), HeightOf(root.Right)) + 1;
        return Balance(root);
    }

    public void Insert(int key, int m) => root = Insert(root, key, m);
    public void Delete(int key) => root = Delete(root, key);
}

public class Experiment
{
    private static readonly int[] M_VALUES = [1048576, 786432, 393216];
    private static readonly Random rng = new();
    private static int RandomInt(int max) => rng.Next(0, max + 1);

    public static void Main()
    {
        int n = 100000;
        int maxTreeSize = 50;
        AVLTree tree = new();
        Queue<int> keys = [];
        List<double> insertTimes = [];
        List<double> deleteTimes = [];

        Stopwatch sw = new();

        string csvPath = "results.csv";

        using (var writer = new StreamWriter(csvPath))
        {
            writer.WriteLine("Iteration,InsertTime(ms),DeleteTime(ms),Memory(MB),Gen0,Gen1,Gen2");

            for (int i = 0; i < n; i++)
            {
                int key = RandomInt(299);
                int m = M_VALUES[key % 3];

                sw.Restart();
                tree.Insert(key, m);
                sw.Stop();
                double insertTime = sw.Elapsed.TotalMilliseconds;
                insertTimes.Add(insertTime);
                keys.Enqueue(key);

                double deleteTime = double.NaN;

                if (keys.Count >= maxTreeSize)
                {
                    int delKey = keys.Dequeue();
                    sw.Restart();
                    tree.Delete(delKey);
                    sw.Stop();
                    deleteTime = sw.Elapsed.TotalMilliseconds;
                    deleteTimes.Add(deleteTime);
                }

                // Every 1000 ops, log GC/memory data
                if (i % 1000 == 0 && i > 0)
                {
                    long mem = GC.GetTotalMemory(false);
                    Console.WriteLine($"Iter {i} | Mem={mem / 1000000.0:F2} MB | " +
                              $"Gen0={GC.CollectionCount(0)} | " +
                              $"Gen1={GC.CollectionCount(1)} | " +
                              $"Gen2={GC.CollectionCount(2)}");
                }

                // Write to CSV
                writer.WriteLine($"{i},{insertTime}," +
                                 $"{(double.IsNaN(deleteTime) ? "" : deleteTime.ToString())}," +
                                 $"{GC.GetTotalMemory(false) / 1000000.0:F2}," +
                                 $"{GC.CollectionCount(0)}," +
                                 $"{GC.CollectionCount(1)}," +
                                 $"{GC.CollectionCount(2)}");
            }
        }

        double avgInsert = insertTimes.Average();
        double avgDelete = deleteTimes.Average();
        double maxInsert = insertTimes.Max();
        double maxDelete = deleteTimes.Max();

        Console.WriteLine($"Insertions: {insertTimes.Count} | Avg={avgInsert:F6} ms | Max={maxInsert:F6} ms");
        Console.WriteLine($"Deletions:  {deleteTimes.Count} | Avg={avgDelete:F6} ms | Max={maxDelete:F6} ms");
    }
}
