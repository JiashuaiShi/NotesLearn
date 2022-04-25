package main

import "fmt"

func maxSumOfThreeSubarrays(nums []int, k int) []int {
	n := len(nums)
	var s = make([]int, n+1)
	for i := 1; i <= n; i++ {
		s[i] = s[i-1] + nums[i-1]
	}

	var x, y = n + 1, 3
	var f = make([][4]int, n+2)

	for i := n - k + 1; i > 0; i-- {
		for j := 1; j <= 3; j++ {
			f[i][j] = max(f[i+1][j], f[i+k][j-1]+s[i-1+k]-s[i-1])
		}

		if f[x][3] <= f[i][3] {
			x = i
		}
	}

	var res []int
	for y > 0 {
		for f[x][y] != f[x+k][y-1]+s[x+k-1]-s[x-1] {
			x++
		}

		res = append(res, x-1)
		y--
		x += k
	}
	return res
}

func max(x, y int) int {
	if x > y {
		return x
	}
	return y
}

func main() {
	var in = []int{1, 2, 1, 2, 6, 7, 5, 1}
	var res = maxSumOfThreeSubarrays(in, 2)
	fmt.Println(res)
}
