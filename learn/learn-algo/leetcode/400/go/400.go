package _00

import "strconv"

func findNthDigit(n int) int {
	var t = 1
	var s = 1
	var k = 9

	for n-t*k > 0 {
		n -= t * k
		t++
		s *= 10
		k *= 10
	}

	s += (n+t-1)/t - 1
	if n%t != 0 {
		n = n % t
	} else {
		n = t
	}

	x := strconv.Itoa(s)

	return int([]uint8(x)[n-1]) - '0'
}
