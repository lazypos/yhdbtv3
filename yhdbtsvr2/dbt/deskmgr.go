package dbt

import (
	"sync"
)

type DeskMgr struct {
	DeskNum   int
	ArrPlayer [4]*Player //座位号
	muxPlay   sync.Mutex
}

func CreateDesk(id int) *DeskMgr {
	desk := &DeskMgr{}
	desk.DeskNum = id
	desk.arrPlayer = [4]*Player{nil, nil, nil, nil}
}

func (this *DeskMgr) AddDesk(p *Player) int {
	this.muxPlay.Lock()
	this.muxPlay.Unlock()
	for i := 0; i < 4; i++ {
		if this.arrPlayer[i] == nil {
			this.arrPlayer[i] = p
			return i
		}
	}
	return -1
}
