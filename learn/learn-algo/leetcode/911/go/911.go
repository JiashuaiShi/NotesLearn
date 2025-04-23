package main

import (
	"fmt"
	"sort"
)

type TopVotedCandidate struct {
	winnerIdByTime []int
	timeArr        []int
}

func Constructor(persons []int, times []int) TopVotedCandidate {
	var (
		winnerIdByTime []int
		timeArr        []int
	)
	n := len(persons)
	var scores = make([]int, n)
	timeArr = times

	var scoreMax = 0
	var winnerId = 0

	for _, id := range persons {
		scores[id]++

		if scores[id] >= scoreMax {
			scoreMax = scores[id]
			winnerId = id
		}

		winnerIdByTime = append(winnerIdByTime, winnerId)
	}

	return TopVotedCandidate{winnerIdByTime, timeArr}
}

func (e *TopVotedCandidate) Q(t int) int {
	index := sort.Search(len(e.timeArr), func(i int) bool { return e.timeArr[i] > t })
	return e.winnerIdByTime[index-1]
}

/**
 * Your TopVotedCandidate object will be instantiated and called as such:
 * obj := Constructor(persons, times);
 * param_1 := obj.Q(t);
 */

func main() {
	persons := []int{0, 0, 0, 0, 1}
	times := []int{0, 6, 39, 52, 75}
	//queries := []int{0, 5, 10, 15, 20, 25, 30}
	queries := []int{45, 49, 59, 68, 42, 37, 99, 26, 78, 43}

	obj := Constructor(persons, times)

	fmt.Println(obj.timeArr)
	fmt.Println(obj.winnerIdByTime)

	for _, q := range queries {
		res := obj.Q(q)
		fmt.Print(res, " ")
	}
}
