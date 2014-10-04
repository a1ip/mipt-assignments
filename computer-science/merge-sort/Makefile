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
  mutex.cpp \
  condition_variable.cpp \
  synchronized_queue.cpp \
  thread.cpp \
  thread_pool.cpp

HPP_LIST=\
  mutex.hpp \
  condition_variable.hpp \
  synchronized_queue.hpp \
  thread.hpp \
  thread_pool.hpp

LIB_LIST=\
  pthread\
  rt

OBJ_LIST=$(patsubst %.cpp, $(OUTDIR)%.$(OBJ), $(CPP_LIST))
INC_LIST=$(patsubst %, $(SRCDIR)%, $(HPP_LIST))
LINK_LIST=$(patsubst %, -l%, $(LIB_LIST))

build: outdir $(OUTDIR)merge-sort
.PHONY: build

outdir:
	mkdir -p $(OUTDIR)

$(OUTDIR)merge-sort: $(OBJ_LIST)
	$(LINK) -o $@ $^ $(LINK_LIST) -O2 -g

$(OUTDIR)%.$(OBJ): $(SRCDIR)%.cpp $(INC_LIST)
	$(CXX) -o $@ -c $< -O2 -g

run: build
	$(OUTDIR)merge-sort ${ARGS}
.PHONY: run

clean:
	rm -r $(OUTDIR)
.PHONY: clean

