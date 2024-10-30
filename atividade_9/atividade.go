package main

import "fmt"

func main() {
	// cria canal n~ao-buferizado
	ch := make(chan string)
	go func() {
		ch <- "Hello world!" // escreve no canal
	}()
	a := <-ch // le o canal nao-buferizado (bloqueante)
	fmt.Println(a)
}
