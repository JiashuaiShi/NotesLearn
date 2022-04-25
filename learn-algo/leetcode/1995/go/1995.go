package main

func countQuadruplets(nums []int) int {
	n := len(nums)
	tMap := map[int]int{}
	ans := 0

	for c := n - 2; c >= 2; c-- {
		tMap[nums[c+1]]++
		for a := 0; a < c-1; a++ {
			for b := a + 1; b < c; b++ {
				t := nums[a] + nums[b]
				ans += tMap[t]
			}
		}
	}

	return ans
}

func countQuadruplets2(nums []int) int {
	n := len(nums)
	tMap := map[int]int{}
	ans := 0

	for b := n - 3; b >= 1; b-- {
		for d := n - 1; d > b+1; d-- {
			tMap[nums[d]-nums[b+1]]++
		}

		for a := 0; a < b; a++ {
			ans += tMap[nums[a]+nums[b]]
		}
	}
	return ans
}
