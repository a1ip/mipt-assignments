# make build
# make client
# make server
# make clean

OUTDIR=build/
SRCDIR=src/
CXX=g++
LINK=g++
OBJ=obj

CLIENT_CPP_LIST=\
	client_main.cpp \
	error.cpp \
  tcp_socket.cpp \
  tcp_listener.cpp \
  udp_socket.cpp \
  utility.cpp \
  chunk.cpp

SERVER_CPP_LIST=\
	server_main.cpp \
	error.cpp \
  tcp_socket.cpp \
  tcp_listener.cpp \
  udp_socket.cpp \
  utility.cpp \
  chunk.cpp

CLIENT_HPP_LIST=\
	error.hpp \
  tcp_socket.hpp \
  tcp_listener.hpp \
  udp_socket.hpp \
  utility.hpp \
  chunk.hpp

SERVER_HPP_LIST=\
	error.hpp \
  tcp_socket.hpp \
  tcp_listener.hpp \
  udp_socket.hpp \
  utility.hpp \
  chunk.hpp

CLIENT_OBJ_LIST=$(patsubst %.cpp, $(OUTDIR)%.$(OBJ), $(CLIENT_CPP_LIST))
SERVER_OBJ_LIST=$(patsubst %.cpp, $(OUTDIR)%.$(OBJ), $(SERVER_CPP_LIST))

CLIENT_INC_LIST=$(patsubst %, $(SRCDIR)%, $(CLIENT_HPP_LIST))
SERVER_INC_LIST=$(patsubst %, $(SRCDIR)%, $(SERVER_HPP_LIST))

$(OUTDIR)client: $(CLIENT_OBJ_LIST)
	$(LINK) -o $@ $^ -lssl

$(OUTDIR)server: $(SERVER_OBJ_LIST)
	$(LINK) -o $@ $^ -lssl

$(OUTDIR)%.$(OBJ): $(SRCDIR)%.cpp $(CLIENT_INC_LIST) $(SERVER_INC_LIST)
	$(CXX) -o $@ -c $<

outdir:
	mkdir -p $(OUTDIR)

build: outdir $(OUTDIR)client $(OUTDIR)server
.PHONY: build

client: build
	$(OUTDIR)client
.PHONY: client

server: build
	$(OUTDIR)server
.PHONY: server

clean:
	rm -r $(OUTDIR)
.PHONY: clean

