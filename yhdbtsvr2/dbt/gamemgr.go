package dbt

import (
	"fmt"
	"net"
	"sync"
	"time"
)

var GGameMgr *GameMgr = &GameMgr{}

type GameMgr struct {
	mapDesks   map[int32]*DeskMgr //共有多少桌子
	muxDesk    sync.Mutex
	mapPlayers map[string]*Player
	mapPlayersTimes map[string]int64
	muxPlayer  sync.Mutex
}

func (this *GameMgr) Start() {
	this.mapDesks = make(map[int32]*DeskMgr)
	this.mapPlayers = make(map[string]*Player)
	this.mapPlayersTimes = make(map[string]int64)
	go this.CheckBreak()
}

func (this *GameMgr) OnConnect(remote string, conn net.Conn) *Player {
	this.muxPlayer.Lock()
	defer this.muxPlayer.Unlock()
	play := &Player{}
	play.InitPlayer(remote, conn)
	this.mapPlayers[remote] = play
	this.mapPlayersTimes[remote] = 1
	return play
}

func (this *GameMgr) OnLeave(remote string, p *Player) {
	this.muxDesk.Lock()
	p.gone = true
	for _, desk := range this.mapDesks {
		if !desk.IsStart {
			for j, p := range desk.ArrPlayer {
				if p != nil && p.Remote == remote {
					desk.ArrPlayer[j] = nil
				}
			}
		}
	}
	this.muxDesk.Unlock()

	this.muxPlayer.Lock()
	defer this.muxPlayer.Unlock()
	delete(this.mapPlayers, remote)
}

func (this *GameMgr) GetPlayCounts(p *Player) {
	this.muxPlayer.Lock()
	counts := len(this.mapPlayers)
	this.muxPlayer.Unlock()
	p.AddMessage(fmt.Sprintf(fmt_query, counts))
}

func (this *GameMgr) AddDesk(p *Player) {
	this.muxDesk.Lock()
	defer this.muxDesk.Unlock()
	var deskNum int32 = (int32)(len(this.mapDesks))
	var siteNum int32 = -1
	var pDesk *DeskMgr
	//找一个有空位的
	for k, desk := range this.mapDesks {
		site := desk.AddDesk(p)
		if site != -1 {
			deskNum = k
			siteNum = site
			pDesk = desk
			break
		}
		deskNum = k
	}
	//或创建新桌子
	if siteNum == -1 {
		deskNum++
		pDesk = CreateDesk(deskNum)
		this.mapDesks[deskNum] = pDesk
		siteNum = pDesk.AddDesk(p)
	}
	p.Ready = false
	p.gone = false
	p.AddMessage(fmt.Sprintf(fmt_add, deskNum, siteNum, p.Remote))

	//广播信息
	pDesk.BroadDeskInfo()
}

func (this *GameMgr) ChangeState(m *Message) {
	this.muxDesk.Lock()
	defer this.muxDesk.Unlock()
	pDesk := this.mapDesks[m.DeskNum]
	pDesk.PostMsg(m)
}

func (this *GameMgr) CheckBreak() {
	ticker := time.NewTicker(time.Second * 180)
	for {
		select {
		case <-ticker.C:
			this.muxPlayer.Lock()
			for k,v := range this.mapPlayersTimes {
				if v == 0 {
					delete(this.mapPlayers, k)
				}else{
					this.mapPlayersTimes[k] = 0
				}
			}
			this.muxPlayer.Unlock()
		}
	}
}

func (this *GameMgr) DoHeart(p *Player) {
	this.muxPlayer.Lock()
	defer this.muxPlayer.Unlock()
	this.mapPlayersTimes[p.Remote] = 1
}