package main

func findJudge(n int, trust [][]int) int {
	var tIn = make([]int, n+1)
	var tOut = make([]int, n+1)
	res := -1

	for _, v := range trust {
		a := v[0]
		b := v[1]
		tIn[a]++
		tOut[b]++

	}

	for i := 1; i <= n; i++ {
		if tIn[i] == 0 && tOut[i] == n-1 {
			if res != -1 {
				return -1
			} else {
				res = i
			}
		}
	}
	return res
}
