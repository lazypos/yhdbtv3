package dbt

import (
	"fmt"
	"log"
	"sync"
	"time"
)

type DeskMgr struct {
	DeskNum   int32
	ArrPlayer [4]*Player //座位号
	muxPlay   sync.Mutex
	Chmsg     chan *Message //消息通道
	IsStart   bool          //游戏是否开始
	ForceExit int32         //强制退出
	TimeTick  int32         //计时器
	NowSite   int32         //当前谁出牌
	LastSite  int32         //上一轮谁出牌
	PerCards  string        //上一次出的牌
	Score     int32         //当前桌面分数
	P0Score   int32         //0,2号玩家得分
	P1Score   int32         //1,3号玩家得分
	RunCounts int32         //逃跑数量
	LastCards []int32       //最后出的牌
}

func CreateDesk(id int32) *DeskMgr {
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
	tickCount := 0
	ticker := time.NewTicker(time.Second * 2)
	for {
		select {
		case <-ticker.C:
			//玩家超时
			tickCount += 2
			this.TimeTick += 2
			if this.IsStart && this.TimeTick >= 35 {
				//断线
				log.Println("玩家断线", this.NowSite)
				this.PlayerLeave(this.NowSite)
				this.IsStart = false
			}
			if !this.IsStart {
				for i, p := range this.ArrPlayer {
					if (p != nil && p.times > 60) || (p != nil && p.gone){
						p.AddMessage(fmt_timeout)
						this.PlayerLeave(int32(i))
					}
				}
			}
			if tickCount >= 10 && !this.IsStart {
				tickCount = 0
				this.muxPlay.Lock()
				this.BroadDeskInfo()
				this.muxPlay.Unlock()
			}
		case m := <-this.Chmsg:
			this.TimeTick = 0
			if m.Opt == "change" {
				switch m.Type {
				case "leave":
					this.PlayerLeave(m.Site)
				case "ready":
					this.PlayerReady(m.Site)
				}
			} else if m.Opt == "game" {
				this.ProcessGame(m)
			}
		}
	}
}

func (this *DeskMgr) GetNextPut(site int32) int32 {
	for i := 1; i < 4; i++ {
		seq := (site + int32(i)) % 4
		return seq
	}
	return -1
}

func (this *DeskMgr) ProcessGame(m *Message) {
	this.muxPlay.Lock()
	defer this.muxPlay.Unlock()
	if !this.IsStart {
		return
	}
	next := this.GetNextPut(m.Site)
	//没出牌
	if len(m.Cards) == 0 {
		//又是同一个了，必须出
		must := 0
		if next == this.LastSite {
			this.LastCards = []int32{}
			must = 1
			//某家得分
			if this.LastSite%2 == 0 {
				this.P0Score += this.Score
			} else {
				this.P1Score += this.Score
			}
			this.Score = 0
			//如果出完了，对家出
			if this.ArrPlayer[next].RunNum != -1 {
				next = (next + 2) % 4
			}
		} else {
			for i := 0; i < 4; i++ {
				if this.ArrPlayer[next].RunNum != -1 {
					next = this.GetNextPut(next)
					if next == this.LastSite {
						this.LastCards = []int32{}
						must = 1
						//某家得分
						if this.LastSite%2 == 0 {
							this.P0Score += this.Score
						} else {
							this.P1Score += this.Score
						}
						this.Score = 0
						//如果出完了，对家出
						if this.ArrPlayer[next].RunNum != -1 {
							next = (next + 2) % 4
						}
					}
				} else {
					break
				}
			}
		}
		this.NowSite = next
		for _, p := range this.ArrPlayer {
			p.AddMessage(fmt.Sprintf(fmt_game_put, m.Site, "", len(this.ArrPlayer[m.Site].ArrCards), this.Score, next, must))
			if must == 1 {
				p.AddMessage(fmt.Sprintf(fmt_score, this.P0Score, this.P1Score))
			}
		}
	} else { //出牌
		nowCards := StringToint32Arr(m.Cards)
		if !IsBigger(this.LastCards, nowCards) {
			this.ArrPlayer[m.Site].AddMessage(fmt_error)
			return
		}
		this.LastSite = m.Site
		log.Println("last put:", this.LastSite)
		ok, score := this.ArrPlayer[m.Site].PutCards(nowCards)
		if ok {
			//跑一个
			this.LastCards = nowCards
			if len(this.ArrPlayer[m.Site].ArrCards) == 0 {
				this.ArrPlayer[m.Site].RunNum = this.RunCounts
				log.Println(m.Site, "over")
				this.RunCounts++
			}
			this.Score += score
			for i := 0; i < 4; i++ {
				if this.ArrPlayer[next].RunNum != -1 {
					next = this.GetNextPut(next)
				} else {
					break
				}
			}
			for _, p := range this.ArrPlayer {
				p.AddMessage(fmt.Sprintf(fmt_game_put, m.Site, m.Cards, len(this.ArrPlayer[m.Site].ArrCards), this.Score, next, 0))
			}
			this.NowSite = next
		}
	}
	run := []int32{-1, -1, -1, -1}
	for i := 0; i < 4; i++ {
		run[i] = this.ArrPlayer[i].RunNum
	}
	if over, arrRst := IsOver(this.P0Score, this.P1Score, run); over {
		this.GameOver(false, arrRst)
	}
}

func (this *DeskMgr) PostMsg(m *Message) {
	this.Chmsg <- m
}

//游戏开始
func (this *DeskMgr) PlayerReady(site int32) {
	this.muxPlay.Lock()
	defer this.muxPlay.Unlock()
	if this.IsStart {
		return
	}
	this.ArrPlayer[site].Ready = true
	if this.IsAllReady() {
		this.IsStart = true
		this.ForceExit = -1
		this.TimeTick = 0
		this.NowSite = -1
		this.Score = 0
		this.P0Score = 0
		this.P1Score = 0
		this.RunCounts = 0
		this.LastCards = []int32{}
		for _, p := range this.ArrPlayer {
			p.RunNum = -1
			p.Ready = false
		}
		arrCards, arrCardsint32 := Create4Cards()
		for i, p := range this.ArrPlayer {
			this.ArrPlayer[i].ArrCards = arrCardsint32[i]
			p.AddMessage(fmt.Sprintf(fmt_start, arrCards[i]))
		}
		put := GRand.Intn(3)
		this.NowSite = int32(put)
		this.LastSite = int32(put)
		for _, p := range this.ArrPlayer {
			p.AddMessage(fmt.Sprintf(fmt_game_put, -1, "", 54, 0, put, 1))
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

func (this *DeskMgr) PlayerRun(site int32, name string) {
	this.ArrPlayer[site].RunCounts ++ ;
	SetDBValue(fmt.Sprint(this.ArrPlayer[site].OnlyId,"run"), fmt.Sprint(this.ArrPlayer[site].RunCounts))
	this.ArrPlayer[site] = nil
	for _, p := range this.ArrPlayer {
		if p != nil {
			p.AddMessage(fmt.Sprintf(fmt_run, site, name))
		}
	}
	this.GameOver(true, []int32{})
}

func (this *DeskMgr) GameOver(run bool, arrRst []int32) {
	if !this.IsStart {
		return
	}
	this.IsStart = false
	for _, p := range this.ArrPlayer {
		if p != nil {
			p.Ready = false
			p.RunNum = -1
			p.times = 0
		}
	}
	if !run {
		for i, p := range this.ArrPlayer {
			if p != nil {
				if arrRst[i] > 0 {
					this.ArrPlayer[i].WinCounts += int(arrRst[i])
					SetDBValue(fmt.Sprint(this.ArrPlayer[i].OnlyId,"win"), fmt.Sprint(this.ArrPlayer[i].WinCounts))
				}
				if arrRst[i] < 0 {
					this.ArrPlayer[i].LoseCounts += int(-arrRst[i])
					SetDBValue(fmt.Sprint(this.ArrPlayer[i].OnlyId,"lose"), fmt.Sprint(this.ArrPlayer[i].LoseCounts))
				}

				p.AddMessage(fmt.Sprintf(fmt_over, "", arrRst[0], "", arrRst[1], "", arrRst[2], "", arrRst[3]))
			}
		}
	}
}

func (this *DeskMgr) AddDesk(p *Player) int32 {
	this.muxPlay.Lock()
	defer this.muxPlay.Unlock()
	for i := 0; i < 4; i++ {
		if this.ArrPlayer[i] == nil {
			this.ArrPlayer[i] = p
			p.times = 0
			return int32(i)
		}
	}
	return -1
}

func (this *DeskMgr) PlayerLeave(site int32) {
	this.muxPlay.Lock()
	defer this.muxPlay.Unlock()
	if this.ArrPlayer[site] == nil {
		return
	}
	name := this.ArrPlayer[site].Remote
	//还没开始游戏
	if !this.IsStart {
		this.ArrPlayer[site] = nil
		this.BroadDeskInfo()
		return
	}
	//游戏结束
	this.PlayerRun(site, name)
}

func (this *DeskMgr) BroadDeskInfo() {
	type DeskInfo struct {
		name  string
		ready int32
		WinCounts	int
		LoseCounts	int
		RunCounts	int
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
			info.WinCounts = v.WinCounts
			info.LoseCounts = v.LoseCounts
			info.RunCounts = v.RunCounts
		}
		arrDeskInfo[i] = info
	}
	//广播
	for _, p := range this.ArrPlayer {
		if p != nil {
			p.AddMessage(fmt.Sprintf(fmt_change, arrDeskInfo[0].name, arrDeskInfo[0].ready, 
				arrDeskInfo[0].WinCounts, arrDeskInfo[0].LoseCounts, arrDeskInfo[0].RunCounts,
				arrDeskInfo[1].name, arrDeskInfo[1].ready, 
				arrDeskInfo[1].WinCounts, arrDeskInfo[1].LoseCounts, arrDeskInfo[1].RunCounts,
				arrDeskInfo[2].name, arrDeskInfo[2].ready,
				arrDeskInfo[2].WinCounts, arrDeskInfo[2].LoseCounts, arrDeskInfo[2].RunCounts,
				arrDeskInfo[3].name, arrDeskInfo[3].ready,
				arrDeskInfo[3].WinCounts, arrDeskInfo[3].LoseCounts, arrDeskInfo[3].RunCounts))
		}
	}
}
