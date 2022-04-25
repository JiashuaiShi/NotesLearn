package main

import "container/heap"

type pair struct {
	end  int
	left int
}

type hp []pair

func (h hp) Len() int {
	return len(h)
}

func (h hp) Less(i, j int) bool {
	return h[i].end < h[j].end
}

func (h hp) Swap(i, j int) {
	h[i], h[j] = h[j], h[i]
}

func (h *hp) Push(v interface{}) {
	*h = append(*h, v.(pair))
}

func (h *hp) Pop() interface{} {
	a := *h
	v := a[len(a)-1]
	*h = a[:len(a)-1]

	return v
}

func min(i, j int) int {
	if i < j {
		return i
	}
	return j
}

func eatenApples(apples, days []int) (ans int) {
	i := 0
	h := hp{}

	for ; i < len(apples); i++ {
		for len(h) > 0 && h[0].end > 0 {
			heap.Pop(&h)
		}

		if apples[i] > 0 {
			heap.Push(&h, pair{days[i] + i, apples[i]})
		}

		if h[0].left > 0 {
			h[0].left--
			if h[0].left == 0 {
				heap.Pop(&h)
			}

			ans++
		}
		i++
	}

	for len(h) > 0 {
		for len(h) > 0 && h[0].end <= i {
			heap.Pop(&h)
		}

		if len(h) == 0 {
			break
		}

		minv := min(h[0].left, h[0].end-i)
		ans += minv
		i += minv
	}
	return
}
