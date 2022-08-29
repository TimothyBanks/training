from fibonacci import FibonacciAlgorithm
from fibonacci import Fibonacci
from sort import Bubble
from sort import Insertion
from sort import Merge
from sort import Quick
from sort import Selection
from tree import Tree
from tree import Traversal

def run_sort(s):
  list = [1, 3, 5, 7, 9, 2, 4, 6, 8, 0]
  s(list)
  print(*list)

def run_tree(traversal):
  values = ['F', 'B', 'G', 'A', 'D', None, 'I', None, None, 'C', 'E', 'H', None]
  tree = Tree(values)
  functor = lambda x : print(x, end='')
  tree.traverse(traversal, functor)
  print('\n')
  
def run_fibonacci(n, algorithm):
  print(Fibonacci()(n, algorithm))

def main():
  # run_fibonacci(50, FibonacciAlgorithm.Recursive)
  run_fibonacci(20, FibonacciAlgorithm.Recursive)
  run_fibonacci(30, FibonacciAlgorithm.Recursive)
  # run_fibonacci(35, FibonacciAlgorithm.Recursive)
  run_fibonacci(50, FibonacciAlgorithm.DynamicProgramming)
  run_fibonacci(100, FibonacciAlgorithm.DynamicProgramming)
  # run_fibonacci(1000, FibonacciAlgorithm.DynamicProgramming)
  run_fibonacci(50, FibonacciAlgorithm.DynamicProgramming2)
  run_fibonacci(100, FibonacciAlgorithm.DynamicProgramming2)
  run_fibonacci(1000, FibonacciAlgorithm.DynamicProgramming2)
  run_sort(Bubble())
  run_sort(Insertion())
  run_sort(Merge())
  run_sort(Quick())
  run_sort(Selection())
  run_tree(Traversal.Breadth)
  run_tree(Traversal.DepthPreOrder)
  run_tree(Traversal.DepthInOrder)
  run_tree(Traversal.DepthPostOrder)

if __name__ == "__main__":
    main()