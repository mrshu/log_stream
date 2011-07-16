C=$(CROSS_COMPILE)gcc -Wall -g
CPP=$(CROSS_COMPILE)g++ -Wall -g

GFLAGS=`pkg-config --libs --cflags gtk+-2.0 gmodule-2.0`
OFLAGS=`pkg-config --libs --cflags opencv`

TARGET=log_stream_server

log_stream_server: log_stream_server.cpp
	$(CPP) log_stream_server.cpp -o log_stream_server $(OFLAGS)



all: $(TARGET)
	@echo "All done"



