package dbt

import (
	"encoding/json"
	"fmt"
	"log"
)

const (
	fmt_query  = `{"opt":"query","online":"%d"}`
	fmt_add    = `{"opt":"add","desk":"%d","site":"%d"}`
	fmt_change = `{"opt":"change","info":[
					{"site":"0","name":"%s","ready":"%d"},
					{"site":"1","name":"%s","ready":"%d"},
					{"site":"2","name":"%s","ready":"%d"},
					{"site":"3","name":"%s","ready":"%d"}]}`
	fmt_start = `{"opt":"start","cards":"%s"}`
)

type Message struct {
	Opt     string `json:"opt"`  //操作
	DeskNum int    `json:"desk"` //桌号
	Site    int    `json:"site"` //位号
	Type    string `json:"type"` //类型
}

func Dispatch_opt(content []byte, p *Player) error {
	//解析收到的消息
	msg := &Message{}
	fmt.Println("recv:", string(content))
	e := json.Unmarshal(content, msg)
	if e != nil {
		log.Println("json error", e)
		return e
	}

	//处理消息
	switch msg.Opt {
	case "query":
		opt_query(p)
		break
	case "add":
		opt_add(p)
		break
	case "change":
		opt_change(msg)
		break
	default:
		log.Println("unknow opt.")
		return fmt.Errorf("unknow opt")
	}
	return nil
}

func opt_query(p *Player) {
	GGameMgr.GetPlayCounts(p)
}

func opt_add(p *Player) {
	GGameMgr.AddDesk(p)
}

func opt_change(m *Message) {
	GGameMgr.ChangeState(m)
}
