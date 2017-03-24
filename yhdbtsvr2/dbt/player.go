package dbt

import (
	"log"
	"net"
	"time"
)

type Player struct {
	ArrCards []int32
	Remote   string
	Ready    bool
	Chmsg    chan string
	conn     net.Conn
	RunNum   int32
	times    int32
	gone     bool
}

func (this *Player) InitPlayer(remote string, conn net.Conn) {
	this.Chmsg = make(chan string, 100)
	this.Remote = remote
	this.conn = conn
	this.times = 0
	this.gone = false
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
			if this.gone {
				return
			}
		case m := <-this.Chmsg:
			log.Println("发送数据:", this.Remote, m)
			err := SendMessage(this.conn, []byte(m))
			if err != nil {
				log.Println("发送数据错误:", this.Remote, m)
				this.conn.Close()
			}
		}
	}
}

func (this *Player) PutCards(cards []int32) (bool, int32) {
	var score int32 = 0
	for _, n := range cards {
		score += GetCardScore(n)
		for i, _ := range this.ArrCards {
			if this.ArrCards[i] == n {
				this.ArrCards[i] = -1
				break
			}
		}
	}

	tmp := []int32{}
	for _, v := range this.ArrCards {
		if v != -1 {
			tmp = append(tmp, v)
		}
	}
	//log.Println(this.Remote, "出牌前", this.ArrCards)
	this.ArrCards = tmp
	log.Println(this.Remote, "出牌后", this.ArrCards)
	return true, score
}
