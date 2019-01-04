#-----------------------------------------------------------------------

APP       := HardwareApplet
TARGET    := CommonTemplateServer
CONFIG    := 
STRIP_FLAG:= N

INCLUDE   += -I../AppletCommon
INCLUDE   += -I./Commands/
LIB       += -L/usr/local/mysql/lib/ -lmysqlclient -lssl -lcrypto

LOCAL_SRC   += $(sort $(wildcard ./Commands/*.cpp))

#-----------------------------------------------------------------------

include /usr/local/tars/cpp/makefile/makefile.tars

#-----------------------------------------------------------------------
