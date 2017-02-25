package dbt

import (
	"log"
	"net"
	"time"
)

type Player struct {
	ArrCards []int
	Remote   string
	Ready    bool
	Chmsg    chan string
	conn     net.Conn
	RunNum   int
	times    int
}

func (this *Player) InitPlayer(remote string, conn net.Conn) {
	this.Chmsg = make(chan string, 100)
	this.Remote = remote
	this.conn = conn
	this.times = 0
	go this.SendQueue()
}

func (this *Player) AddMessage(msg string) {
	this.Chmsg <- msg
}

func (this *Player) SendQueue() {
	ticker := time.NewTicker(time.Second * 5)
	for {
		select {
		case <-ticker.C:
			if !this.Ready {
				this.times += 5
			}
		case m := <-this.Chmsg:
			log.Println("发送数据:", this.Remote, m)
			SendMessage(this.conn, []byte(m))
		}
	}
}

func (this *Player) PutCards(cards []int) (bool, int) {
	score := 0
	for _, n := range cards {
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
