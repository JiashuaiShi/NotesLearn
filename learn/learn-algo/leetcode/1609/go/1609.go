package main

import "math"

type TreeNode struct {
	Val   int
	Left  *TreeNode
	Right *TreeNode
}

func isEvenOddTree(root *TreeNode) bool {
	q := []*TreeNode{root}
	var prev int

	for level := 0; len(q) > 0; level++ {
		if isOdd(level) {
			prev = math.MaxInt32
		} else {
			prev = math.MinInt32
		}

		n := len(q)

		for _, v := range q {
			if isOdd(level) {
				if isOdd(v.Val) || v.Val >= prev {
					return false
				}
			} else {
				if isEven(v.Val) || v.Val <= prev {
					return false
				}
			}

			if v.Left != nil {
				q = append(q, v.Left)
			}

			if v.Right != nil {
				q = append(q, v.Right)
			}

			prev = v.Val
		}

		q = q[n:]

	}
	return true
}

func isOdd(n int) bool {
	return 1 == (n & 1)
}

func isEven(n int) bool {
	return 1 != (n & 1)
}
