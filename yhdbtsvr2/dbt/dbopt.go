package dbt

import (
	"github.com/syndtr/goleveldb/leveldb"
	"log"
	"strconv"
)

var GDBOpt *leveldb.DB = nil

func InitDB() error {
	var err error
	GDBOpt, err = leveldb.OpenFile("dbtdb", nil)
	if err != nil {
		log.Println("open db error", err)
		return err
	}
	return nil
}

func GetDBValue(key string) string {
	val, err := GDBOpt.Get([]byte(key), nil)
	if err == nil {
		return string(val)
	}
	return ""
}

func GetDBValueAsInt(key string) int {
	val, err := GDBOpt.Get([]byte(key), nil)
	if err == nil {
		t,_:= strconv.Atoi(string(val))
		return t
	}
	return 0
}

func SetDBValue(key,val string) error {
	if err := GDBOpt.Put([]byte(key), []byte(val), nil); err != nil {
		return err
	}
	return nil
}