package dbt

import (
	"bytes"
	"fmt"
	"math/rand"
	"time"
)

const (
	type_unknow   = 0
	type_singal   = 1
	type_pairs    = 2
	type_three    = 3
	type_atom     = 4
	type_boom     = 5
	type_sister   = 6
	type_threetwo = 7
	type_plane    = 8
)

var GRand *rand.Rand = rand.New(rand.NewSource(time.Now().UnixNano()))

//创建手牌
func Create4Cards() ([4]string, [4][]int) {
	arrCards := make([]int, 216)
	for i := 0; i < 216; i++ {
		arrCards[i] = i / 4
	}

	for i := 0; i < 2; i++ {
		for j := 0; j < cap(arrCards); j++ {
			n := GRand.Intn(215)
			arrCards[j], arrCards[n] = arrCards[n], arrCards[j]
		}
	}

	//拆分4组
	arrCardsString := [4]string{}
	arrCardsInt := [4][]int{}
	for i := 0; i < 4; i++ {
		buf := bytes.NewBufferString("")
		arrCardsInt[i] = make([]int, 54)
		for j := 0; j < 54; j++ {
			buf.WriteString(fmt.Sprintf("%d,", arrCards[i*54+j]))
			arrCardsInt[i] = append(arrCardsInt[i], arrCards[i*54+j])
		}
		arrCardsString[i] = buf.String()[:buf.Len()-2]
	}
	return arrCardsString, arrCardsInt
}
