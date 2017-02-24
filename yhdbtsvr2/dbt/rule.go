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
			arrCardsInt[i][j] = arrCards[i*54+j]
		}
		arrCardsString[i] = buf.String()[:buf.Len()-2]
	}
	return arrCardsString, arrCardsInt
}

func IsBigger(per, now string) bool {
	return true
}

func IsOver() bool {
	return false
}

func GetCardScore(seq int) int {
	if seq/4 == 4 {
		return 5
	}
	if seq/4 == 12 || seq/4 == 9 {
		return 10
	}
	return 0
}

func CheckCardsVailds(cards []int) bool {
	for _, c := range cards {
		if c > 53 || c < 0 {
			return false
		}
	}
	return true
}

func GetValue(n int) int {
	return n / 4
}

func GetColor(n int) int {
	return n % 4
}

func IsJoker(n int) bool {
	return GetValue(n) == 13
}

func IsRedFive(n int) bool {
	return GetValue(n) == 4 && GetColor(n) == 3
}

func GetWeight(n int) int {
	if isRedFive(n) {
		return 10000
	}
	return GetWeightWithOutRedFive()
}

func GetWeightWithOutRedFive(n int) n {
	if isJoker(n) {
		return n + 100
	}
	if getValue(n) == 0 || getValue(n) == 1 {
		return n + 54
	}
	return n
}

func IsNormalCard(t int) (bool, int) {
	return t != type_atom && t != type_boom
}

func IsSingle(cards []int) (bool, int) {
	if len(cards) == 1 {
		return true, GetWeight(cards[0])
	}
	return false, 0
}

func IsPairs(cards []int) (bool, int) {
	if len(cards) != 2 {
		return false, 0
	}
	if GetValue(cards[0]) != GetValue(cards[1]) {
		return false, 0
	}
	//joker必须一样颜色
	if IsJoker(cards[0]) {
		if cards[0] != cards[1] {
			return false, 0
		}
		return true, GetValue(cards[0])
	}
	//红5
	if IsRedFive(cards[0]) && IsRedFive(cards[1]) {
		return true, GetWeight(cards[0])
	}
	return true, GetWeightWithOutRedFive(cards[1])
}

func Isthree(cards []int) (bool, int) {
	if len(cards) != 3 {
		return false, 0
	}
	//点数必须一样
	if GetValue(cards[0]) != GetValue(cards[1]) || GetValue(cards[0]) != GetValue(cards[2]) {
		return false, 0
	}
	//花色必须不一样
	if GetColor(cards[0]) == GetColor(cards[1]) && GetColor(cards[0]) == GetColor(cards[2]) {
		return false, 0
	}
	return true, GetWeightWithOutRedFive(cards[2])
}
