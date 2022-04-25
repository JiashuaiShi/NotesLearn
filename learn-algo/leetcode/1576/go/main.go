package main

import "fmt"

func modifyString(str string) string {
	arr := []uint8{'a', 'b', 'c'}
	s := []uint8(str)

	for i := 0; i < len(s); i++ {
		if s[i] == '?' {
			for j := 0; j < len(arr); j++ {
				if (i > 0 && s[i-1] == arr[j]) || (i < len(s)-1 && s[i+1] == arr[j]) {
					continue
				}
				s[i] = arr[j]
				break
			}
		}
	}
	return string(s)
}

func main() {
	var str = "??yw?ipkj?"
	res := modifyString(str)
	fmt.Println(res)
}
