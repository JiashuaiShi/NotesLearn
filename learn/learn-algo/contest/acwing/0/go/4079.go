package main

import (
	"fmt"
	"strconv"
)

func initString(n int) string {
	var res string
	for i := 0; i <= n; i++ {
		res += strconv.Itoa(i)
	}
	return res
}

func main() {
	str := initString(1000)

	count := 0
	fmt.Scanf("%d", &count)

	for  ; count > 0;count--{

		var i int
		fmt.Scanf("%d", &i)
		fmt.Println([]byte(str)[i] - '0')
	}
}

