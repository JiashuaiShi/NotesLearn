package main

import (
	"fmt"
	"time"
)

type signal struct{}

func worker() {
	println("worker is working...")
	time.Sleep(10 * time.Second)
}
func spawn(f func()) <-chan signal {
	c := make(chan signal)
	go func() {
		println("worker start to work...")
		f()
		c <- signal{}
	}()
	return c
}
func main() {
	println("start a worker...")
	c := spawn(worker)
	<-c
	fmt.Println("worker work done!")
}

func trySend(c chan<- int, i int) bool {
	select {
	case c <- i:
		return true
	default: // channel满了
		return false
	}
}

func worker() {
	select {
	case <-c:
		// ... do some stuff
	case <-time.After(30 * time.Second):
		return
	}
}

func worker() {
	heartbeat := time.NewTicker(30 * time.Second)
	defer heartbeat.Stop()
	for {
		select {
		case <-c:
			// ... do some stuff
		case <-heartbeat.C:
			//... do heartbeat stuff
		}
	}
}
