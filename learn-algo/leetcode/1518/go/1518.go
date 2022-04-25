package main

func numWaterBottles(numBottles int, numExchange int) int {
	var left = numBottles
	var res = numBottles

	for left >= numExchange {
		t := left % numExchange
		r := left / numExchange
		res += r
		left = t + r
	}

	return res
}
