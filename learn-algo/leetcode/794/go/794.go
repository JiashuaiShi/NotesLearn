package main

import "fmt"

func validTicTacToe(board []string) bool {
	var getCount = func(c uint8) (num int) {
		for i := 0; i < 3; i++ {
			for j := 0; j < 3; j++ {
				if board[i][j] == c {
					num++
				}
			}
		}

		return num
	}

	var getRes = func(c uint8) bool {
		for i := 0; i < 3; i++ {
			if board[0][i] == c && board[1][i] == c && board[2][i] == c {
				return true
			}
			if board[i][0] == c && board[i][1] == c && board[i][2] == c {
				return true
			}
			if board[0][0] == c && board[1][1] == c && board[2][2] == c {
				return true
			}
			if board[0][2] == c && board[1][1] == c && board[2][0] == c {
				return true
			}

			return false
		}
		return true
	}

	var XCount = getCount('X')
	var OCount = getCount('O')
	var XRes = getRes('X')
	var ORes = getRes('O')

	if XRes == true && XCount != OCount+1 {
		return false
	}

	if ORes == true && OCount != XCount {
		return false
	}

	if XRes && ORes {
		return false
	}

	if XCount != OCount && XCount != OCount+1 {
		return false
	}

	return true
}

func main() {
	var in = []string{"O  ", "   ", "   "}
	var res = validTicTacToe(in)
	fmt.Println(res)
}
