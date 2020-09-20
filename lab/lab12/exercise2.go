package main

import (
	"fmt"
)

func main() {
	print_squares(10)
	fmt.Println()
	print_even_squares(10)
	fmt.Println()
	print_fibonacci(10)
	fmt.Println()
}

func square(n int) int {
	return n * n
}

func print_squares(n int) {
	for i := 1; i <= n; i++ {
		fmt.Printf("%d ", square(i))
	}
}

func print_even_squares(n int) {
	for i := 1; i <= n; i++ {
		if i%2 == 0 {
			fmt.Printf("%d ", square(i))
		}
	}
}

func print_fibonacci(n int) {
	one := 0
	two := 1
	for i := 0; i <= n; i++ {
		fmt.Printf("%d ", two)
		one, two = two, one + two
	}	
}
