package dbt

import (
	"log"
	"net"
	"strconv"
	"strings"
	"time"
)

type Player struct {
	ArrCards []int
	Remote   string
	Ready    bool
	Chmsg    chan string
	conn     net.Conn
}

func (this *Player) InitPlayer(remote string, conn net.Conn) {
	this.Chmsg = make(chan string, 100)
	this.Remote = remote
	this.conn = conn
	go this.SendQueue()
}

func (this *Player) AddMessage(msg string) {
	this.Chmsg <- msg
}

func (this *Player) SendQueue() {
	for {
		select {
		case <-time.After(time.Second * 5):
			break
		case m := <-this.Chmsg:
			log.Println("发送数据:", this.Remote, m)
			SendMessage(this.conn, []byte(m))
		}
	}
}

func (this *Player) PutCards(cards string) (bool, int) {
	arr := strings.Split(cards, ",")
	score := 0
	for _, c := range arr {
		n, _ := strconv.Atoi(c)
		score += GetCardScore(n)
		for i, _ := range this.ArrCards {
			if this.ArrCards[i] == n {
				this.ArrCards[i] = -1
				break
			}
		}
	}
	tmp := []int{}
	for _, v := range this.ArrCards {
		if v != -1 {
			tmp = append(tmp, v)
		}
	}
	log.Println(this.Remote, "出牌前", this.ArrCards)
	this.ArrCards = tmp
	log.Println(this.Remote, "出牌后", this.ArrCards)
	return true, score
}
