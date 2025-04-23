package main

import "fmt"

func canConstruct(r string, m string) bool {
	var tMap = map[int32]int{}
	var res = true

	for _, c := range m {
		tMap[c]++
	}

	for _, c := range r {
		tMap[c]--;
		if tMap[c] < 0 {
			res = false
		}
	}

	return res
}

func main() {
	r := "abcdfgh"
	m := "b"
	res := canConstruct(r, m)
	fmt.Println(res)
}