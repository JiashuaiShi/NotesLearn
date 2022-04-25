package main

import (
	"fmt"
	"strings"
)

const INTMAX = int(^uint(0) >> 1)

func isAlpha(c byte) bool {
	if isUpAlpha(c) || isLowAlpha(c) {
		return true
	}
	return false
}

func isUpAlpha(c byte) bool {
	if c >= 65 && c <= 90 {
		return true
	}
	return false
}

func isLowAlpha(c byte) bool {
	if c >= 97 && c <= 122 {
		return true
	}
	return false
}

func isDigit(c byte) bool {
	if c >= 48 && c <= 57 {
		return true
	}
	return false
}

func shortestCompletingWord(licensePlate string, words []string) string {
	licensePlate = strings.ToLower(licensePlate)

	var getMap = func(s string) map[uint8]int {
		var kMap = make(map[byte]int)
		for _, v := range []byte(s) {
			if isAlpha(v) {
				kMap[v]++
			}
		}
		return kMap
	}

	kMap := getMap(licensePlate)

	var isTrue = func(uMap map[byte]int) bool {
		for k, v := range kMap {
			if uMap[k] < v {
				return false
			}
		}
		return true
	}

	var minLen = INTMAX
	var minLenIndex = len(licensePlate) - 1

	for i, word := range words {
		tMap := getMap(word)
		if isTrue(tMap) {
			if len(word) < minLen {
				minLen = len(word)
				minLenIndex = i
			}
		}
	}

	return words[minLenIndex]
}

func main() {
	var lic = string("Ah71752")
	var words = []string{"of", "husband", "easy", "education", "drug", "prevent", "writer", "old"}
	res := shortestCompletingWord(lic, words)
	fmt.Println(res)

}
