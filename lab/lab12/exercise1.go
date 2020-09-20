// Put package name here
package main
// Put imported packages here
import "fmt"
import "math/rand"


func main() {
	fmt.Println("Hello world!")
	random()
}

func random() {
	var x = rand.Intn(10)
	fmt.Println("Your random number of the day is ", x)
}