package dbt

import (
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

func (this *GameMgr) OnConnect(remote string) *Player {
	this.muxPlayer.Lock()
	defer this.muxPlayer.Unlock()
	play := &Player{}
	play.InitPlayer(remote)
	this.mapPlayers[remote] = play
	return play
}

func (this *GameMgr) OnLeave(remote string) {
	this.muxPlayer.Lock()
	defer this.muxPlayer.Unlock()
	delete(this.mapPlayers, remote)
}

func (this *GameMgr) GetPlayCounts() int {
	this.muxPlayer.Lock()
	defer this.muxPlayer.Unlock()
	return len(this.mapPlayers)
}

func (this *GameMgr) AddDesk(p *Player) (int, int) {
	this.muxDesk.Lock()
	defer this.muxDesk.Unlock()
	deskNum := 0
	//找一个有空位的
	for k, desk := range this.mapDesks {
		site := desk.AddDesk(p)
		if site != -1 {
			return k, site
		}
		deskNum = k
	}
	//创建新桌子
	deskNum++
	newDesk := CreateDesk(deskNum)
	this.mapDesks[deskNum] = newDesk
	return deskNum, newDesk.AddDesk(p)
}

type DeskInfo struct {
	name  string
	ready string
}

func (this *GameMgr) GetMessage(deskNum int) [4]*DeskInfo {
	this.muxDesk.Lock()
	defer this.muxDesk.Unlock()

	arrDeskInfo := [4]*DeskInfo{}
	desk := this.mapDesks[deskNum]
	for i, v := range desk.ArrPlayer {
		info := &DeskInfo{}
		info.name = v.Remote
		info.ready = "0"
		if v.Ready {
			info.ready = "1"
		}
		arrDeskInfo[i] = info
	}
	return arrDeskInfo
}
