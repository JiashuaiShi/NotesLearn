package main

func countBattleships(board [][]byte) int {
	r := len(board)
	c := len(board[0])

	ans := 0

	for i := 0; i < r; i++ {
		for j := 0; j < c; j++ {
			if i > 0 && board[i-1][j] == 'X' {
				continue
			}

			if j > 0 && board[i][j-1] == 'X' {
				continue
			}

			if board[i][j] == 'X' {
				ans++
			}
		}
	}

	return ans
}
