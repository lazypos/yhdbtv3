package dbt

import (
	"fmt."
	"log"
)

const (
	fmt_query  = `{"opt":"query","num":"%d"}`
	fmt_add    = `{"opt":"add","desk":"%d","site":"%d"}`
	fmt_getmsg = `{"opt":"getmsg","info":[
					{"site":"0","name":"%s","ready":"%d"},
					{"site":"1","name":"%s","ready":"%d"},
					{"site":"2","name":"%s","ready":"%d"},
					{"site":"3","name":"%s","ready":"%d"}
				 ]}`
)

type Message struct {
	Opt     string `json:"opt"`
	DeskNum int    `json:"desk"`
}

func Dispatch_opt(msg *Message, p *Player) string {
	rpy := ""
	switch msg.Opt {
	case "query":
		rpy = opt_query()
		break
	case "add":
		rpy = opt_add(p)
		break
	case "getmsg":
		rpy = opt_getmsg(msg.DeskNum)
		break
	default:
		log.Println("unknow opt.")
	}
	return rpy
}

func opt_query() string {
	player := GGameMgr.GetPlayCounts()
	return fmt.Sprintf(fmt_query, player)
}

func opt_add(p *Player) string {
	desknum, site := GGameMgr.AddDesk(p)
	return fmt.Sprintf(fmt_add, desknum, site)
}

func opt_getmsg(desk int) string {
	arrInfo := GGameMgr.GetMessage(desk)
	return fmt.Sprintf(fmt_getmsg, arrInfo[0].name, arrInfo[0].ready,
		arrInfo[1].name, arrInfo[1].ready, arrInfo[2].name, arrInfo[2].ready,
		arrInfo[3].name, arrInfo[3].ready)
}
