package main

import "fmt"

func main() {
	slic := []int{1, 2, 3}
	cp := make([]*int, len(slic))
	for i, x := range slic {
		cp[i] = &slic[i]
		cp = append(cp, &x)

	}
	fmt.Println(cp)

}
