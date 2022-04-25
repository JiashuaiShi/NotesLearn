package main

func loudAndRich(richer [][]int, quiet []int) []int {
	n := len(quiet)

	var g = make([][]int, n)
	var ans = make([]int, n)

	for i := 0; i < n; i++ {
		ans[i] = -1
	}

	for _, v := range richer {
		a := v[0]
		b := v[1]

		g[b] = append(g[b], a)
	}

	var dfs func(u int)
	dfs = func(u int) {
		if ans[u] != -1 {
			return
		}

		ans[u] = u

		for _, v := range g[u] {
			dfs(v)
			if quiet[ans[v]] < quiet[ans[u]] {
				ans[u] = ans[v]
			}
		}
	}

	for i := range quiet {
		dfs(i)
	}

	return ans
}
