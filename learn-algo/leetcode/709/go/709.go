package main

import "fmt"

func toLowerCase(s string) string {
	str := []byte(s)
	for i := 0; i < len(s); i++ {
		if str[i] <= 'Z' && str[i] >= 'A' {
			str[i] += 'a' - 'A'
		}
	}

	return string(str)
}

func main() {
	s := "AaBbCc"
	res := toLowerCase(s)
	fmt.Println(res)
}
