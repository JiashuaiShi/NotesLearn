package main

import (
	"fmt"
	"time"
)

func dayOfYear(date string) int {
	t, _ := time.Parse("2006-01-02", date)
	s, _ := time.Parse("2006-01-02", date[:4]+"-01-01")

	res := t.Sub(s).Hours()/24 + 1

	fmt.Println(t)
	fmt.Println(s)

	return int(res + 1)
}

func main() {
	res := dayOfYear("2021-12-24")
	fmt.Println(res)
}
