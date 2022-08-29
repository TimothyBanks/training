package main

import "fmt"
import "training/fibonacci"
import "training/sort"
import "training/tree"

type SortFunctor func([]int)
func RunSortTests(functor SortFunctor) {
	v := []int{9, 7, 5, 3, 1, 0, 8, 6, 4, 2}
	functor(v)
	fmt.Printf("%v\n", v)
}

func PrintTreeNode(node *tree.TreeNode) bool {
	if node == nil {
		return false
	}
	fmt.Print((*node).Data)
	return false
}

func RunTreeTests(traversal tree.Traversal) {
	v := []string{"F", "B", "G", "A", "D", "\000", "I", "\000", "\000", "C", "E", "H", "\000"}
	t := tree.NewTree(v);
	(*t).Traverse(traversal, PrintTreeNode)
	fmt.Print("\n")
}

func RunFibonacciTests(n int, f fibonacci.FibonacciFunctor) {
	fmt.Println(f(n))
}

func main() {
	RunSortTests(sort.BubbleSort)
	RunSortTests(sort.InsertionSort)
	RunSortTests(sort.SelectionSort)
	RunSortTests(sort.MergeSort)
	RunSortTests(sort.QuickSort)
	RunTreeTests(tree.Breadth)
	RunTreeTests(tree.DepthPreOrder)
	RunTreeTests(tree.DepthInOrder)
	RunTreeTests(tree.DepthPostOrder)
	RunFibonacciTests(5, fibonacci.RecursiveOnly)
	RunFibonacciTests(25, fibonacci.RecursiveOnly)
	// RunFibonacciTests(50, fibonacci.RecursiveOnly)
	RunFibonacciTests(5, fibonacci.DynamicV1)
	RunFibonacciTests(25, fibonacci.DynamicV1)
	RunFibonacciTests(50, fibonacci.DynamicV1)
	RunFibonacciTests(100, fibonacci.DynamicV1)
	RunFibonacciTests(500, fibonacci.DynamicV1)
	RunFibonacciTests(5, fibonacci.DynamicV2)
	RunFibonacciTests(25, fibonacci.DynamicV2)
	RunFibonacciTests(50, fibonacci.DynamicV2)
	RunFibonacciTests(100, fibonacci.DynamicV2)
	RunFibonacciTests(500, fibonacci.DynamicV2)
}
