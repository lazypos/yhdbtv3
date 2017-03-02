package dbt

import (
	"encoding/json"
	"fmt"
	"log"
)

const (
	fmt_query  = `{"opt":"query","online":"%d"}`
	fmt_add    = `{"opt":"add","desk":"%d","site":"%d","name":"%s"}`
	fmt_change = `{"opt":"change","info":[
					{"site":"0","name":"%s","ready":"%d"},
					{"site":"1","name":"%s","ready":"%d"},
					{"site":"2","name":"%s","ready":"%d"},
					{"site":"3","name":"%s","ready":"%d"}]}`
	fmt_start = `{"opt":"start","cards":"%s"}`
	fmt_run   = `{"opt":"run","site":"%d","name":"%s"}`
	fmt_over  = `{"opt":"over","info":[
                    {"site":"0","name":"%s","result":"%d"},
                    {"site":"1","name":"%s","result":"%d"},
                    {"site":"2","name":"%s","result":"%d"},
                    {"site":"3","name":"%s","result":"%d"}]}`
	fmt_game_put = `{"opt":"game","per":"%d","cards":"%s","surplus":"%d","score":"%d","now":"%d","must":"%d"}`
	fmt_score    = `{"opt":"score","p0":"%d","p1":"%d"}`
	fmt_error    = `{"opt":"error"}`
	fmt_timeout  = `{"opt":"timeout"}`
)

type Message struct {
	Opt     string `json:"opt"`   //操作
	DeskNum int32  `json:"desk"`  //桌号
	Site    int32  `json:"site"`  //位号
	Type    string `json:"type"`  //类型
	Cards   string `json:"cards"` //出牌
}

func Dispatch_opt(content []byte, p *Player) error {
	//解析收到的消息
	msg := &Message{}
	log.Println("recv:", string(content))
	e := json.Unmarshal(content, msg)
	if e != nil {
		log.Println("json error", e)
		return e
	}

	//处理消息
	switch msg.Opt {
	case "query":
		opt_query(p)
	case "add":
		opt_add(p)
	case "change":
		opt_change(msg)
	case "game":
		opt_game(msg)
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

func opt_game(m *Message) {
	GGameMgr.ChangeState(m)
}
