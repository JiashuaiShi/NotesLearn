package main

import "fmt"

var maxl = 0

func maxPower(s string) int {
	for i, _ := range s {
		k := i
		count := 0
		for k < len(s) && s[i] == s[k] {
			count++
			k++
		}

		if count > maxl {
			maxl = count
		}
	}

	return maxl
}


func main() {
	ex := "tourist"
	res := maxPower(ex)
	fmt.Println(res)
}
