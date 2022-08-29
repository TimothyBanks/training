package fibonacci

type FibonacciFunctor func(int)int

func RecursiveOnly(n int) int {
	if n < 2 {
		return n
	}

	return RecursiveOnly(n - 1) + RecursiveOnly(n - 2)
}

func Dynamic(n int, lookup map[int]int) int {
	if n < 2 {
		return n
	}

	_, found := lookup[n - 1]
	if found == false {
		lookup[n - 1] = Dynamic(n - 1, lookup)
	}

	_, found = lookup[n - 2]
	if found == false {
		lookup[n - 2] = Dynamic(n - 2, lookup)
	}

	return lookup[n - 1] + lookup[n - 2]
}

func DynamicV1(n int) int {
	lookup := make(map[int]int)
	return Dynamic(n, lookup)
}

func DynamicV2(n int) int {
	if n < 2 {
		return n
	}

	n2 := 0
	n1 := 1

	for i := 2; i <= n; i++ {
		temp := n1 + n2
		n2 = n1
		n1 = temp
	}

	return n1
}