package main

/* producer-consumer problem in Go */

import (
	"fmt"
	"time"
)

var done = make(chan bool, 2)
var msgs = make(chan int, 10)

func produce() {
	for i := 0; i < 10; i++ {
		msgs <- i // Send the item to the consumer1
		time.Sleep(time.Second)
	}
	done <- true // Let main know that producer is done
	fmt.Println("Producer is done.\n")
}

func consume() {
	for {
		msg := <-msgs // read the item from the channel
		fmt.Println(msg)
		time.Sleep(time.Second * 2)
		if 9 == msg {
			done <- true // Let main know that consumer is done
			fmt.Println("Consumer is done.\n")
		}
	}
}

func main() {
	go produce()
	go consume()
	<-done
	<-done
}
