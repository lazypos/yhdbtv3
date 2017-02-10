package main

import (
	"./dbt"
	"fmt"
	"log"
	"net"
	"os"
	"unsafe"
)

func initlog() {
	h, e := os.OpenFile("dbt.log", os.O_CREATE|os.O_APPEND|os.O_RDWR, 0x666)
	if e != nil {
		log.Panicln("log file error", e)
		return
	}
	log.SetFlags(log.LstdFlags | log.Lshortfile)
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

func recvMessage(conn net.Conn) ([]byte, error) {
	buf := make([]byte, 4)
	n, e := conn.Read(buf)
	if n != 4 || e != nil {
		return []byte{}, e
	}
	var msglen int = *(*int)(unsafe.Pointer(&buf[0]))
	buf = make([]byte, msglen)
	recvLen := 0
	for {
		if recvLen >= msglen {
			break
		}
		n, e = conn.Read(buf[recvLen:])
		if e != nil || n == 0 {
			break
		}
		recvLen += n
	}
	return buf, e
}

func process_connect(conn net.Conn) {
	remote := conn.RemoteAddr().String()
	log.Println(remote, "玩家上线")
	play := dbt.GGameMgr.OnConnect(remote, conn)
	defer func() {
		dbt.GGameMgr.OnLeave(remote)
		log.Println(remote, "玩家下线")
	}()
	defer conn.Close()
	conn.(*net.TCPConn).SetLinger(0)
	for {
		contect, e := recvMessage(conn)
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
	initlog()
	dbt.GGameMgr.Start()
	start()
}
