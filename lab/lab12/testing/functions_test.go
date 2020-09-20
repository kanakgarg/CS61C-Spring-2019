package functions

import "testing"

func TestAbsSumPos(t *testing.T) {
	x := []int{12, 14, 16}
	sum := AbsSum(x)
	if sum != 42 {
		t.Error("Expected 42, got", sum)
	}
}

func TestAbsSumNeg(t *testing.T) {
	x := []int{-12, -14, -16}
	sumNeg := AbsSum(x)
	if sum != 42 {
		t.Error("Expected 42, got", sum)
	}
}

func TestProd(t *testing.T) {
	x := []int{1,2,3}
	prod := Prod(x)
	if prod != 6 {
		t.Error("Expected 6, got", prod)
	}
}

func TestSub(t *testing.T) {
	x := []int{1,2,3}
	sub := Sub(x)
	if sub != -6 {
		t.Error("Expected -6, got", sub)
	}}
