package dbt

import (
	"fmt"
	"sync"
	"time"
)

type DeskMgr struct {
	DeskNum   int
	ArrPlayer [4]*Player //座位号
	muxPlay   sync.Mutex
	IsStart   bool
	ForceExit int
	TimeTick  int
	Chmsg     chan *Message
}

func CreateDesk(id int) *DeskMgr {
	desk := &DeskMgr{}
	desk.DeskNum = id
	desk.InitDesk()
	return desk
}

func (this *DeskMgr) InitDesk() {
	this.IsStart = false
	this.ForceExit = -1
	this.ArrPlayer = [4]*Player{nil, nil, nil, nil}
	this.Chmsg = make(chan *Message, 100)
	go this.GameSchedule()
}

func (this *DeskMgr) GameSchedule() {
	ticker := time.NewTicker(time.Second * 2)
	for {
		select {
		case <-ticker.C:
			//超时
			if this.IsStart && this.TimeTick >= 60 {

			}
		case m := <-this.Chmsg:
		}
	}
}

func (this *DeskMgr) PostMsg(m *Message) {
	this.Chmsg <- m
}

//游戏开始
func (this *DeskMgr) GameStart() {
	this.muxPlay.Lock()
	this.muxPlay.Unlock()
	if this.ArrPlayer[0].Ready && this.ArrPlayer[1].Ready &&
		this.ArrPlayer[2].Ready && this.ArrPlayer[3].Ready {
		this.IsStart = true
		this.ForceExit = -1
		this.TimeTick = 0
		arrCards := Create4Cards()
		for i, p := range this.ArrPlayer {
			p.AddMessage(fmt.Sprintf(fmt_start, arrCards[i]))
		}
	}
}

func (this *DeskMgr) AddDesk(p *Player) int {
	this.muxPlay.Lock()
	this.muxPlay.Unlock()
	for i := 0; i < 4; i++ {
		if this.ArrPlayer[i] == nil {
			this.ArrPlayer[i] = p
			return i
		}
	}
	return -1
}
