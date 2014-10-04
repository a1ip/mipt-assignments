# make build
# make run
# make clean

OUTDIR=build/
SRCDIR=src/
CXX=g++
LINK=g++
OBJ=obj

CPP_LIST=\
	main.cpp \
	ipc_interface.cpp \
	ipc_file_based.cpp \
	ipc_pipe_based.cpp \
	ipc_message_queue_based.cpp \
	ipc_socket_based.cpp \
	semaphore.cpp \
	utility.cpp \
	error.cpp

HPP_LIST=\
	ipc_interface.hpp \
	ipc_file_based.hpp \
	ipc_pipe_based.hpp \
	ipc_message_queue_based.hpp \
	ipc_socket_based.hpp \
	semaphore.hpp \
	utility.hpp \
	error.hpp

OBJ_LIST=$(patsubst %.cpp, $(OUTDIR)%.$(OBJ), $(CPP_LIST))
INC_LIST=$(patsubst %, $(SRCDIR)%, $(HPP_LIST))

$(OUTDIR)dishwashing: $(OBJ_LIST)
	$(LINK) -o $@ $^

$(OUTDIR)%.$(OBJ): $(SRCDIR)%.cpp $(INC_LIST)
	$(CXX) -o $@ -c $<

outdir:
	mkdir -p $(OUTDIR)

build: outdir $(OUTDIR)dishwashing
.PHONY: build

run: build
	$(OUTDIR)dishwashing
.PHONY: run

clean:
	rm -r $(OUTDIR)
.PHONY: clean

