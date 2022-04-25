package main

import "strings"

func repeatedStringMatch(a string, b string) int {
	cache := [26]bool{false}

	for _, v := range a {
		cache[v-'a'] = true
	}

	for _, v := range b {
		if cache[v-'a'] == false {
			return -1
		}
	}

	k := len(b) / len(a)

	var t string

	for i := 0; i < k; i++ {
		t += a
	}

	for i := 0; i < 3; i++ {
		if strings.Contains(t, b) {
			return k + i
		}

		t += a
	}

	return -1
}
