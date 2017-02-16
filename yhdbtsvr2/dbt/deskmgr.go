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
	NowSite   int //当前谁出牌
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
			//玩家超时
			if this.IsStart && this.TimeTick >= 65 {
				//断线
			}
		case m := <-this.Chmsg:
			switch m.Type {
			case "leave":
				this.PlayerLeave(m.Site)
			case "ready":
				this.PlayerReady(m.Site)
			}
		}
	}
}

func (this *DeskMgr) PostMsg(m *Message) {
	this.Chmsg <- m
}

//游戏开始
func (this *DeskMgr) PlayerReady(site int) {
	this.muxPlay.Lock()
	this.muxPlay.Unlock()
	this.ArrPlayer[site].Ready = true
	if this.IsAllReady() {
		this.IsStart = true
		this.ForceExit = -1
		this.TimeTick = 0
		this.NowSite = -1
		arrCards, arrCardsInt := Create4Cards()
		for i, p := range this.ArrPlayer {
			this.ArrPlayer[i].ArrCards = arrCardsInt[i]
			p.AddMessage(fmt.Sprintf(fmt_start, arrCards[i]))
		}
		put := GRand.Intn(3)
		for _, p := range this.ArrPlayer {
			p.AddMessage(fmt.Sprintf(fmt_game_put, 0, "", 54, put))
		}
	} else {
		this.BroadDeskInfo()
	}
}

func (this *DeskMgr) IsAllReady() bool {
	for _, p := range this.ArrPlayer {
		if p == nil {
			return false
		}
		if !p.Ready {
			return false
		}
	}
	return true
}

func (this *DeskMgr) PlayerRun(site int, name string) {
	for _, p := range this.ArrPlayer {
		if p != nil {
			p.AddMessage(fmt.Sprintf(fmt_run, site, name))
		}
	}
	this.GameOver(true)
}

type DeskResult struct {
	Name   string
	Result string
}

func (this *DeskMgr) GetResult() [4]*DeskResult {
	return [4]*DeskResult{}
}

func (this *DeskMgr) GameOver(run bool) {
	this.IsStart = false
	if !run {
		arrRst := this.GetResult()
		for _, p := range this.ArrPlayer {
			if p != nil {
				p.AddMessage(fmt.Sprintf(fmt_over, arrRst[0].Name, arrRst[0].Result,
					arrRst[1].Name, arrRst[1].Result, arrRst[2].Name, arrRst[2].Result,
					arrRst[3].Name, arrRst[3].Result))
			}
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

func (this *DeskMgr) PlayerLeave(site int) {
	this.muxPlay.Lock()
	this.muxPlay.Unlock()

	name := this.ArrPlayer[site].Remote
	this.ArrPlayer[site] = nil
	//还没开始游戏
	if !this.IsStart {
		this.BroadDeskInfo()
		return
	}
	//游戏结束
	this.PlayerRun(site, name)
}

func (this *DeskMgr) BroadDeskInfo() {
	type DeskInfo struct {
		name  string
		ready int
	}
	//集中消息
	arrDeskInfo := [4]*DeskInfo{}
	for i, v := range this.ArrPlayer {
		info := &DeskInfo{}
		info.name = ""
		info.ready = 0
		if v != nil {
			info.name = v.Remote
			if v.Ready {
				info.ready = 1
			}
		}
		arrDeskInfo[i] = info
	}
	//广播
	for _, p := range this.ArrPlayer {
		if p != nil {
			p.AddMessage(fmt.Sprintf(fmt_change, arrDeskInfo[0].name, arrDeskInfo[0].ready,
				arrDeskInfo[1].name, arrDeskInfo[1].ready, arrDeskInfo[2].name, arrDeskInfo[2].ready,
				arrDeskInfo[3].name, arrDeskInfo[3].ready))
		}
	}
}
