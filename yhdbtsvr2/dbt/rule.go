package dbt

import (
	"bytes"
	"fmt"
	"log"
	"math/rand"
	"sort"
	"strconv"
	"strings"
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
		arrCardsString[i] = buf.String()[:buf.Len()-1]
	}
	return arrCardsString, arrCardsInt
}

func StringToIntArr(cards string) []int {
	r := []int{}
	arr := strings.Split(cards, ",")
	for _, c := range arr {
		if len(c) > 0 {
			n, _ := strconv.Atoi(c)
			r = append(r, n)
		}
	}
	return r
}

func IsBigger(per, now []int) bool {
	if !CheckCardsVailds(now) {
		return false
	}
	sort.Ints(per)
	sort.Ints(now)
	log.Println(per, now)
	//当前牌是否合法
	tnow, wnow := GetType(now)
	if tnow == type_unknow {
		log.Println("当前类型不合法")
		return false
	}
	if len(per) == 0 {
		return true
	}
	tper, wper := GetType(per)
	if wper == 1000 {
		return false
	}
	//比较类型
	if IsNotBoomAndAtom(tnow) && !IsNotBoomAndAtom(tper) {
		log.Println("非炸")
		return false
	}
	if !IsNotBoomAndAtom(tnow) && IsNotBoomAndAtom(tper) {
		return true
	}
	//都是普通牌，必须数量类型一样
	if IsNotBoomAndAtom(tnow) && IsNotBoomAndAtom(tper) {
		if tnow == tper && len(now) == len(per) {
			return wnow > wper
		}
		log.Println("类型数量不一样")
		return false
	}
	if !IsNotBoomAndAtom(tnow) && !IsNotBoomAndAtom(tper) {
		if tper == type_atom && tnow == type_boom {
			return false
		}
		if tper == type_boom && tnow == type_atom {
			return true
		}
		//类型一样,数量一样比权重，数量多的大
		if len(now) == len(per) {
			return wnow > wper
		}
		return len(now) > len(per)
	}
	return false
}

func IsOver(site, s0, s1 int, info []int) (bool, []*DeskResult) {

	return false, []*DeskResult{}
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
	if IsRedFive(n) {
		return 1000
	}
	return GetWeightWithOutRedFive(n)
}

func GetWeightWithOutRedFive(n int) int {
	if IsJoker(n) {
		return n + 100
	}
	if GetValue(n) == 0 || GetValue(n) == 1 {
		return n + 54
	}
	return n
}

func GetAtomWeight(n int) int {
	if IsRedFive(n) {
		return 1000
	}
	if IsJoker(n) {
		return 500 + n
	}
	return GetColor(n)*20 + n
}

func IsNotBoomAndAtom(t int) bool {
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

func IsAtom(cards []int) (bool, int) {
	//必须是相同的牌
	if len(cards) == 3 {
		if cards[0] == cards[1] && cards[0] == cards[2] {
			return true, GetAtomWeight(cards[0])
		}
	}
	if len(cards) == 4 {
		if cards[0] == cards[1] && cards[0] == cards[2] && cards[0] == cards[3] {
			return true, GetAtomWeight(cards[0])
		}
	}
	return false, 0
}

func IsBoom(cards []int) (bool, int) {
	//不能带joker
	if len(cards) < 4 || len(cards) > 16 || IsJoker(cards[0]) {
		return false, 0
	}
	if b, _ := IsAtom(cards); b {
		return false, 0
	}
	//点数要一样
	val := GetValue(cards[0])
	for _, c := range cards {
		if GetValue(c) != val {
			return false, 0
		}
	}
	return true, GetWeightWithOutRedFive(cards[len(cards)-1])
}

func IsSister(cards []int) (bool, int) {
	l := len(cards)
	if len(cards) < 8 || len(cards)%2 != 0 || IsJoker(cards[l-1]) {
		return false, 0
	}
	//必须两两相等
	for i := 0; i < len(cards); i += 2 {
		if GetValue(cards[0]) != GetValue(cards[1]) {
			return false, 0
		}
	}
	//带A必须带K
	if GetValue(cards[0]) == 0 {
		if GetValue(cards[l-1]) != 12 {
			return false, 0
		}
		//递增
		for i := 2; i < len(cards)-2; i += 2 {
			if GetValue(cards[i])+1 != GetValue(cards[i+2]) {
				return false, 0
			}
		}
		return true, GetWeightWithOutRedFive(cards[1])
	}
	//不带A
	for i := 0; i < len(cards)-2; i += 2 {
		if GetValue(cards[i])+1 != GetValue(cards[i+2]) {
			return false, 0
		}
	}
	return true, GetWeightWithOutRedFive(cards[l-1])
}

func IsPlane(cards []int) (bool, int) {
	num := len(cards)
	if num < 9 || num%3 != 0 || IsJoker(cards[num-1]) {
		return false, 0
	}
	//必须三三相等
	for i := 0; i < len(cards); i += 3 {
		if GetValue(cards[0]) != GetValue(cards[1]) || GetValue(cards[0]) != GetValue(cards[2]) {
			return false, 0
		}
	}
	//带A必须带K
	if GetValue(cards[0]) == 0 {
		if GetValue(cards[num-1]) != 12 {
			return false, 0
		}
		//递增
		for i := 3; i < len(cards)-3; i += 3 {
			if GetValue(cards[i])+1 != GetValue(cards[i+3]) {
				return false, 0
			}
		}
		return true, GetWeightWithOutRedFive(cards[2])
	}
	//不带A
	for i := 0; i < len(cards)-3; i += 3 {
		if GetValue(cards[i])+1 != GetValue(cards[i+3]) {
			return false, 0
		}
	}
	return true, GetWeightWithOutRedFive(cards[num-1])
}

func IsThreetwo(cards []int) (bool, int) {
	num := len(cards)
	if num != 5 {
		return false, 0
	}
	if b, _ := IsBoom(cards); b {
		return false, 0
	}
	//AAABB
	if GetValue(cards[0]) == GetValue(cards[1]) && GetValue(cards[0]) == GetValue(cards[2]) {
		if GetValue(cards[3]) != GetValue(cards[4]) {
			return false, 0
		}
		if IsJoker(cards[3]) && cards[3] != cards[4] {
			return false, 0
		}
		if IsJoker(cards[0]) && (cards[0] != cards[1] || cards[0] != cards[2]) {
			return false, 0
		}
		return true, GetWeightWithOutRedFive(cards[2])
	}
	//AABBB
	if GetValue(cards[2]) == GetValue(cards[3]) && GetValue(cards[2]) == GetValue(cards[4]) {
		if GetValue(cards[0]) != GetValue(cards[1]) {
			return false, 0
		}
		if IsJoker(cards[0]) && cards[0] != cards[1] {
			return false, 0
		}
		if IsJoker(cards[2]) && (cards[2] != cards[3] || cards[2] != cards[4]) {
			return false, 0
		}
		return true, GetWeightWithOutRedFive(cards[4])
	}
	return false, 0
}

func GetType(cards []int) (int, int) {
	if b, w := IsSingle(cards); b {
		return type_singal, w
	}
	if b, w := IsPairs(cards); b {
		return type_pairs, w
	}
	if b, w := Isthree(cards); b {
		return type_three, w
	}
	if b, w := IsAtom(cards); b {
		return type_atom, w
	}
	if b, w := IsBoom(cards); b {
		return type_boom, w
	}
	if b, w := IsSister(cards); b {
		return type_sister, w
	}
	if b, w := IsPlane(cards); b {
		return type_plane, w
	}
	if b, w := IsThreetwo(cards); b {
		return type_threetwo, w
	}
	return type_unknow, 0
}
