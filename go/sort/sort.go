package sort

func BubbleSort(v []int) {
	l := len(v)

	if l <= 1 {
		return
	}

	for {
		sorted := true
	
    for i := 1; i < l; i++ {
			if v[i - 1] > v[i] {
				v[i], v[i - 1] = v[i - 1], v[i]
				sorted = false
			}
		} 

		if sorted {
			break
		}
	}
}

func InsertionSort(v []int) {
	l := len(v)

	if l <= 1 {
		return
	}

	for i := 1; i < l; i++ {
		for j := i; j > 0; j-- {
			if v[j] < v[j-1] {
				v[j], v[j - 1] = v[j - 1], v[j]
			}
		}
	}
}

func SelectionSort(v []int) {
	l := len(v)

	if l <= 1 {
		return
	}

	for i := 0; i < l - 1; i++ {
		min := i
		for j := i + 1; j < l; j++ {
			if v[j] < v[min] {
				min = j
			}
		}
		v[min], v[i] = v[i], v[min]
	}
}

func MergeSort(v []int) {
	l := len(v)

	if l <= 1 {
		return
	}

	mid := l / 2
	left := make([]int, mid)
	right := make([]int, l - mid)
	copy(left, v[:mid])
	copy(right, v[mid:])
	MergeSort(left)
	MergeSort(right)

	i := 0
	ll := len(left)
	rl := len(right)
	li := 0
	ri := 0

	for {
		if left[li] < right[ri] {
			v[i] = left[li]
			li++
		} else {
			v[i] = right[ri]
			ri++
		}

    i++

		if li == ll || ri == rl {
			break
		}		
	}

	exhaust := func(dest []int, di int, source []int, si int) {
		dl := len(dest)
		sl := len(source)

		if si >= sl || di >= dl {
			return
		}

		for {
			dest[di] = source[si]
			di++
			si++
			if si == sl {
				return
			}
		}
	}

	exhaust(v, i, left, li)
	exhaust(v, i, right, ri)
}

func Partition(v []int, start int, end int) int {
	if end <= start {
		return -1
	}

	// The pivot value will be the value held at the "end" index of the array.
	begin := start
	for i := start; i < end; i++ {
		if v[i] <= v[end] {
			v[i], v[begin] = v[begin], v[i]
			begin++
		}
	}

	// Move the pivot value into its final sorted position
	v[begin], v[end] = v[end], v[begin]
	return begin
}

func QuickSort2(v []int, start int, end int) {
	if end <= start {
		return
	}

	partition := Partition(v, start, end)
	QuickSort2(v, start, partition - 1)
	QuickSort2(v, partition + 1, end)
}

func QuickSort(v []int) {
	QuickSort2(v, 0, len(v) - 1)
}
