from enum import Enum

class Traversal(Enum):
  Breadth = 0
  DepthPreOrder = 1   # Parent -> Left -> Right
  DepthInOrder = 2    # Left -> Parent -> Right
  DepthPostOrder = 3  # Left -> Right -> Parent

class Node:
  def __init__(self, data = None, left = None, right = None):
    self.data = data
    self.left = left
    self.right = right

class Tree:
  def __init__(self, array):
    if len(array) == 0:
      return

    self.root = Node()
    self.root.data = array[0]

    q = []
    q.append(self.root)

    i = 1
    while i < len(array):
      current = q.pop(0)

      if array[i] is not None:
        current.left = Node()
        current.left.data = array[i]
        q.append(current.left)

      i += 1

      if array[i] is not None:
        current.right = Node()
        current.right.data = array[i]
        q.append(current.right)

      i += 1

  def traverse(self, traversal, functor):
    switcher = {
      Traversal.Breadth: lambda f : self.__traverse_breadth(f),
      Traversal.DepthPreOrder: lambda f : self.__traverse_preorder(f),
      Traversal.DepthInOrder: lambda f : self.__traverse_inorder(f),
      Traversal.DepthPostOrder: lambda f : self.__traverse_postorder(f),
    }
    switcher.get(traversal, lambda f : None)(functor)

  def __traverse_breadth(self, functor):
    if self.root is None:
      return

    q = []
    q.append(self.root)

    while len(q) > 0:
      current = q.pop(0)
      if functor(current.data):
        return

      if current.left is not None:
        q.append(current.left)

      if current.right is not None:
        q.append(current.right)

  def __traverse_preorder(self, functor):
    if self.root is None:
      return

    q = []
    current = self.root

    while current or len(q) > 0:
      if current is not None:
        if functor(current.data):
          return

        if current.right is not None:
          q.append(current.right)

        current = current.left
      else:
        current = q.pop()
  
  def __traverse_inorder(self, functor):
    if self.root is None:
      return

    q = []
    current = self.root

    while current or len(q) > 0:
      if current is not None:
        q.append(current)
        current = current.left
      else:
        current = q.pop()
        if functor(current.data):
          return
        current = current.right

  def __traverse_postorder(self, functor):
    if self.root is None:
      return

    q = []
    current = self.root
    last = None

    while current or len(q) > 0:
      if current is not None:
        q.append(current)
        current = current.left
      else:
        peek = q[-1]
        if peek.right is not None and peek.right is not last:
          current = peek.right
        else:
          if functor(peek.data):
            return
          last = peek
          current = None
          q.pop()