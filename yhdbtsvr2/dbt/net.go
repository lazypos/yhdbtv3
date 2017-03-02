package dbt

import (
	"fmt"
	"net"
	"unsafe"
)

func RecvMessage(conn net.Conn) ([]byte, error) {
	buf := make([]byte, 4)
	n, e := conn.Read(buf)
	if n != 4 || e != nil {
		return []byte{}, e
	}
	var msglen int32 = *(*int32)(unsafe.Pointer(&buf[0]))
	buf = make([]byte, msglen)
	recvLen := 0
	for {
		if int32(recvLen) >= msglen {
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

func SendMessage(conn net.Conn, msg []byte) error {
	msglen := len(msg)
	head := *(*[4]byte)(unsafe.Pointer(&msglen))
	n, e := conn.Write(head[:])
	if n != 4 || e != nil {
		return fmt.Errorf("send error %s", e.Error())
	}
	sendLen := 0
	for {
		if sendLen >= msglen {
			break
		}
		n, e = conn.Write(msg[sendLen:])
		if e != nil || n == 0 {
			break
		}
		sendLen += n
	}
	return e
}
