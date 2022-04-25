package main

import (
	"fmt"
)

//func truncateSentence(s string, k int) string {
//	strSplit := strings.Split(s, " ")
//
//	var res string
//	for _, str := range strSplit {
//		res += str
//		res += " "
//		k--
//		if k == 0 {
//			break
//		}
//	}
//
//	return res[:len(res)-1]
//}

func truncateSentence(s string, k int) string {
	var end = len(s)
	for i, c := range s {
		if i == len(s) || c == ' ' {
			if k--; k == 0 {
				end = i
			}

		}
	}

	return s[:end]
}

func main() {
	var str = "chopper is not a tanuki"
	res := truncateSentence(str, 5)
	fmt.Println(res)
}
