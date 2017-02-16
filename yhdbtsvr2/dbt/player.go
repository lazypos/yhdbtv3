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
