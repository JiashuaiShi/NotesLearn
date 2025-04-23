package main

import (
	"fmt"
)

func min(a, b int) int {
	if a < b {
		return a
	} else {
		return b
	}
}


func abs(a int) int {
	if a < 0 {
		a = -a
	}
	return a
}

func largestSumAfterKNegations(nums []int, k int) int {
	var minV = 200
	var tMap = map[int]int{}
	tMap = make(map[int]int)
	for _, v := range nums {
		tMap[v]++
		minV = min(abs(v), minV)
	}

	for i := -100; i < 0 && k > 0; i++ {
		for tMap[i] > 0 && k > 0 {
			tMap[i]--
			tMap[-i]++
			k--
		}
	}

	var res int

	for k, v := range tMap {
		res += v * k
	}

	if k%2 != 0 {
		res -= 2 * minV
	}

	return res
}

func main() {
	var in = []int{4, 2, 3}
	res := largestSumAfterKNegations(in, 1)
	fmt.Println(res)

}
