package main

import (
	"container/heap"
	"fmt"
	"sort"
)

//func scheduleCourse(courses [][]int) int {
//	sort.Slice(courses, func(i, j int) bool {
//		return courses[i][1] < courses[j][1]
//	})
//
//	h := &Heap{}
//	total := 0 // 优先队列中所有课程的总时间
//	for _, course := range courses {
//		if t := course[0]; total+t <= course[1] {
//			total += t
//			heap.Push(h, t)
//		} else if h.Len() > 0 && t < h.IntSlice[0] {
//			total += t - h.IntSlice[0]
//			h.IntSlice[0] = t
//			heap.Fix(h, 0)
//		}
//	}
//	return h.Len()
//}

// ============实现heap===========
type Heap struct {
	sort.IntSlice
}

func (h Heap) Less(i, j int) bool {
	return h.IntSlice[i] > h.IntSlice[j]
}

func (h *Heap) Push(x interface{}) {
	h.IntSlice = append(h.IntSlice, x.(int))
	heap.Fix(h, 0)
}

func (h *Heap) Pop() interface{} {
	a := h.IntSlice
	x := a[len(a)-1]
	h.IntSlice = a[:len(a)-1]
	return x
}

func (h Heap) Top() int {
	return h.IntSlice[0]
}

func scheduleCourse(courses [][]int) int {
	sort.Slice(courses, func(i, j int) bool {
		return courses[i][1] < courses[j][1]
	})

	h := &Heap{}
	total := 0 // 优先队列中所有课程的总时间
	for _, course := range courses {
		if t := course[0]; total+t <= course[1] {
			total += t
			heap.Push(h, t)
		} else if h.Len() > 0 && t < h.IntSlice[0] {
			total += t - h.IntSlice[0]
			h.IntSlice[0] = t
			heap.Fix(h, 0)
		}
	}
	return h.Len()
}

func main() {
	var courses = [][]int{{5, 5}, {4, 6}, {2, 6}}
	res := scheduleCourse(courses)
	fmt.Println(res)
}
