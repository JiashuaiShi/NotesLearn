package main

import "fmt"

var dx = [4]int{1, 0, -1, 0}
var dy = [4]int{0, 1, 0, -1}

var g, st [][]int
var n, m int

func dfs(x int, y int) {
	for i := 0; i < 4; i++ {
		a := dx[i] + x
		b := dy[i] + y
		if a >= 0 && a < n && b >= 0 && b < m && g[a][b] == g[x][y] {
			if st[a][b] == 0 {
				st[a][b] = 1
				dfs(a, b)
			}
		} else {
			st[x][y] = 2
		}
	}
}

func colorBorder(grid [][]int, row int, col int, color int) [][]int {
	g = grid
	n = len(grid)
	m = len(grid[0])

	st = make([][]int, n)
	for i := 0; i < n; i++ {
		st[i] = make([]int, m)
	}

	st[row][col] = 1
	dfs(row, col)

	for i := 0; i < n; i++ {
		for j := 0; j < m; j++ {
			if st[i][j] == 2 {
				g[i][j] = color
			}
		}
	}
	return g
}

func main() {
	var grid = [][]int{{1, 1}, {1, 2}}
	res := colorBorder(grid, 0, 0, 3)
	fmt.Println(res)
}
