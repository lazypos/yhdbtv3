package dbt

import ()

type Player struct {
	ArrCards []int
	Remote   string
	Ready    bool
}

func (this *Player) InitPlayer(remote string) {
	this.Remote = remote
}
