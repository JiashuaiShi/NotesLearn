package main

import (
	"fmt"
	"sort"
	"strconv"
)

var t []string = []string{"Gold Medal", "Silver Medal", "Bronze Medal"}

func findRelativeRanks(score []int) []string {
	type pair struct {
		index int
		value int
	}

	var sli []pair

	for i, v := range score {
		sli = append(sli, pair{i, v})
	}

	sort.Slice(sli, func(a, b int) bool {
		return sli[a].value > sli[b].value
	})

	var res = make([]string, len(score))

	for i, _ := range sli {
		if i < 3 {
			res[sli[i].index] = t[i]
		} else {
			res[sli[i].index] = strconv.Itoa(i + 1)
		}
	}

	return res
}

func main() {
	var in = []int{5, 4, 3, 2, 1}
	res := findRelativeRanks(in)
	fmt.Println(res)
}
