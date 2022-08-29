from enum import Enum

class FibonacciAlgorithm(Enum):
  Recursive = 0
  DynamicProgramming = 1
  DynamicProgramming2 = 2

class Fibonacci:
  def __call__(self, n, algorithm):
    switcher = {
      FibonacciAlgorithm.Recursive: lambda n, _ : self.__recursive(n),
      FibonacciAlgorithm.DynamicProgramming: lambda n, lookup : self.__dp(n, lookup),
      FibonacciAlgorithm.DynamicProgramming2: lambda n, _ : self.__dp2(n),
    }
    lookup = {}
    return switcher.get(algorithm, lambda n, lookup: None)(n, lookup)

  def __recursive(self, n):
    if n <= 1:
      return n
    return self.__recursive(n - 1) + self.__recursive(n - 2)

  def __dp(self, n, lookup):
    if n <= 1:
      return n

    if lookup.get(n-1) is None:
      lookup[n-1] = self.__dp(n-1, lookup)

    if lookup.get(n-2) is None:
      lookup[n-2] = self.__dp(n-2, lookup)

    return lookup[n-1] + lookup[n-2]

  def __dp2(self, n):
    if n <= 1:
      return n

    n2 = 0
    n1 = 1

    for i in range(2, n + 1):
      temp = n1 + n2
      n2 = n1
      n1 = temp

    return n1