package main

import (
	"./dbt"
	"fmt"
	"log"
	"net"
	"os"
	"strings"
)

func initlog() {
	h, e := os.OpenFile("dbt.log", os.O_CREATE|os.O_APPEND|os.O_RDWR, 0x666)
	if e != nil {
		log.Panicln("log file error", e)
		return
	}
	log.SetFlags(log.LstdFlags | log.Lshortfile)
	log.Println(h)
	log.SetOutput(h)
}

func start() {
	listener, e := net.Listen("tcp", ":9999")
	if e != nil {
		log.Println("listen error", e)
		return
	}
	defer listener.Close()

	for {
		conn, e := listener.Accept()
		if e != nil {
			log.Println("accept error", e)
			continue
		}
		go process_connect(conn)
	}
}

//94.102.   204.93.
func process_connect(conn net.Conn) {
	remote := conn.RemoteAddr().String() 
	if strings.Contains(remote, "204.93.154."){
		return
	}
	log.Println(remote, "玩家上线")
	play := dbt.GGameMgr.OnConnect(remote, conn)
	defer func() {
		dbt.GGameMgr.OnLeave(remote, play)
		log.Println(remote, "玩家下线")
	}()
	defer conn.Close()
	conn.(*net.TCPConn).SetLinger(0)
	for {
		contect, e := dbt.RecvMessage(conn)
		if e != nil {
			log.Println("recv error", e)
			break
		}
		e = dbt.Dispatch_opt(contect, play)
		if e != nil {
			log.Println("process error", e)
			break
		}
	}
}

func main() {
	fmt.Println("start server !")
	initlog()
	dbt.GGameMgr.Start()
	start()
}
