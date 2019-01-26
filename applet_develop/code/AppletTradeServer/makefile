#-----------------------------------------------------------------------

APP       := HardwareApplet
TARGET    := AppletTradeServer
CONFIG    := 
STRIP_FLAG:= N

INCLUDE   += -I../AppletCommon
INCLUDE   += -I./Commands/
LIB       += -L/usr/local/mysql/lib/ -lmysqlclient -lssl -lcrypto -L../AppletCommon/BaseUtils/ -lbase_utils

LOCAL_SRC   += $(sort $(wildcard ./Commands/*.cpp))

#-----------------------------------------------------------------------

include /usr/local/tars/cpp/makefile/makefile.tars

#-----------------------------------------------------------------------
