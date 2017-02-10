package dbt

import (
	"math/rand"
	"time"
)

var GRand *rand.Rand = rand.New(rand.NewSource(time.Now().UnixNano()))

//创建手牌
func Create4Cards() [4]string {
	arrCards := [4]string{}
	return arrCards
}
