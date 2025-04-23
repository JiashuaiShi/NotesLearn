package main

import "fmt"

func max(x, y int) int {
	if x > y {
		return x
	}
	return y
}

func min(x, y int) int {
	if x < y {
		return x
	}
	return y
}

func maxIncreaseKeepingSkyline(grid [][]int) int {
	var cnt1, cnt2 int

	r := len(grid)
	c := len(grid[0])

	var ud = make([]int, c)
	var lr = make([]int, r)

	for i := 0; i < r; i++ {
		for j := 0; j < c; j++ {
			lr[i] = max(lr[i], grid[i][j])
			cnt1 += grid[i][j]
		}
	}

	for i := 0; i < c; i++ {
		for j := 0; j < r; j++ {
			ud[i] = max(ud[i], grid[j][i])
		}
	}

	for i := 0; i < r; i++ {
		for j := 0; j < c; j++ {
			grid[i][j] = min(ud[j], lr[i])
			cnt2 += grid[i][j]
		}
	}

	return cnt2 - cnt1
}

func main() {
	var grid = [][]int{{3, 0, 8, 4}, {2, 4, 5, 7}, {9, 2, 6, 3}, {0, 3, 1, 0}}
	res := maxIncreaseKeepingSkyline(grid)
	fmt.Println(res)
}
