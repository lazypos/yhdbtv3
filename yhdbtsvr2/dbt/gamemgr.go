package dbt

import (
	"fmt"
	"net"
	"sync"
)

var GGameMgr *GameMgr = &GameMgr{}

type GameMgr struct {
	mapDesks   map[int]*DeskMgr //共有多少桌子
	muxDesk    sync.Mutex
	mapPlayers map[string]*Player
	muxPlayer  sync.Mutex
}

func (this *GameMgr) Start() {
	this.mapDesks = make(map[int]*DeskMgr)
	this.mapPlayers = make(map[string]*Player)
}

func (this *GameMgr) OnConnect(remote string, conn net.Conn) *Player {
	this.muxPlayer.Lock()
	defer this.muxPlayer.Unlock()
	play := &Player{}
	play.InitPlayer(remote, conn)
	this.mapPlayers[remote] = play
	return play
}

func (this *GameMgr) OnLeave(remote string) {
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
	deskNum := -1
	siteNum := -1
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
	p.AddMessage(fmt.Sprintf(fmt_add, deskNum, siteNum))

	//广播信息
	this.BroadDeskInfo(pDesk)
}

func (this *GameMgr) BroadDeskInfo(desk *DeskMgr) {
	type DeskInfo struct {
		name  string
		ready string
	}
	//集中消息
	arrDeskInfo := [4]*DeskInfo{}
	for i, v := range desk.ArrPlayer {
		info := &DeskInfo{}
		info.name = v.Remote
		info.ready = "0"
		if v.Ready {
			info.ready = "1"
		}
		arrDeskInfo[i] = info
	}
	//广播
	for _, p := range desk.ArrPlayer {
		p.AddMessage(fmt.Sprintf(fmt_change, arrDeskInfo[0].name, arrDeskInfo[0].ready,
			arrDeskInfo[1].name, arrDeskInfo[1].ready, arrDeskInfo[2].name, arrDeskInfo[2].ready,
			arrDeskInfo[3].name, arrDeskInfo[3].ready))
	}
}

func (this *GameMgr) ChangeState(m *Message) {
	this.muxDesk.Lock()
	defer this.muxDesk.Unlock()
	pDesk := this.mapDesks[m.DeskNum]
	pDesk.PostMsg(m)
}
