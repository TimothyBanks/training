class Bubble:
  def sort(self, list):
    l = len(list)
    if l <= 1:
      return

    swapped = True
    while swapped:
      swapped = False
      for i in range(1, l):
        if list[i] < list[i - 1]:
          list[i], list[i - 1] = list[i - 1], list[i]
          swapped = True

      if not swapped:
        break

  def __call__(self, list):
    self.sort(list)

class Selection:
  def sort(self, list):
    l = len(list)
    if l <= 1:
      return
    
    for i in range(0, l - 1):
      min = i
      for j in range(i + 1, l):
        if list[j] < list[min]:
          min = j
      list[i], list[min] = list[min], list[i]

  def __call__(self, list):
    self.sort(list)

class Insertion:
  def sort(self, list):
    l = len(list)
    if l <= 1:
      return

    for i in range(1, l):
      for j in range(i, 0, -1):
        if list[j] < list[j-1]:
          list[j], list[j-1] = list[j-1], list[j]


  def __call__(self, list):
    self.sort(list)

class Merge:
  def sort(self, list):
    size = len(list)

    if size <= 1:
      return

    mid = size // 2
    left = list[:mid]
    right = list[mid:]

    self.sort(left)
    self.sort(right)

    i = l = r = 0
    while l < len(left) and r < len(right):
      if left[l] < right[r]:
        list[i] = left[l]
        l += 1
      else:
        list[i] = right[r]
        r += 1
      i += 1

    self.__exhaust(list, i, left, l)
    self.__exhaust(list, i, right, r)

  def __exhaust(self, destination, destination_index, source, source_index):
    while source_index < len(source):
      destination[destination_index] = source[source_index]
      destination_index += 1
      source_index += 1

  def __call__(self, list):
    self.sort(list)

class Quick:
  def sort(self, list):
    self.__sort(list, 0, len(list) - 1)

  def __sort(self, list, start, end):
    if start >= end:
      return

    pivot = self.__partition(list, start, end)
    self.__sort(list, start, pivot - 1)
    self.__sort(list, pivot + 1, end)

  def __partition(self, list, start, end):
    begin = start
    for i in range(start, end):
      if list[i] <= list[end]:
        list[i], list[begin] = list[begin], list[i]
        begin += 1

    list[begin], list[end] = list[end], list[begin]
    return begin  

  def __call__(self, list):
    self.sort(list)