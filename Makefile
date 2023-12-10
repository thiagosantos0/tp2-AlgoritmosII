CPP := g++

SRCS := helper.cc tsp-branch-and-bound.cc tsp-twice-around-tree.cc tsp-christofides.cc
OBJS := $(SRCS:.cc=.o)

BRANCH_AND_BOUND := tsp-branch-and-bound
TWICE_AROUND_TREE := tsp-twice-around-tree
CHRISTOFIDES := tsp-christofides

all: $(BRANCH_AND_BOUND) $(TWICE_AROUND_TREE) $(CHRISTOFIDES)

$(BRANCH_AND_BOUND): tsp-branch-and-bound.o helper.o
	$(CPP) -o $@ $^ -lm

$(TWICE_AROUND_TREE): tsp-twice-around-tree.o helper.o
	$(CPP) -o $@ $^ -lm

$(CHRISTOFIDES): tsp-christofides.o helper.o
	$(CPP) -o $@ $^ -lm

%.o: %.cc
	$(CPP) $(CFLAGS) -c $<

run-branch-and-bound: $(BRANCH_AND_BOUND)
	./$(BRANCH_AND_BOUND) $(FILE_NAME)

run-twice-around-tree: $(TWICE_AROUND_TREE)
	./$(TWICE_AROUND_TREE) $(FILE_NAME) 

run-christofides: $(CHRISTOFIDES)
	./$(CHRISTOFIDES) $(FILE_NAME)

clean:
	rm -f $(BRANCH_AND_BOUND) $(TWICE_AROUND_TREE) $(CHRISTOFIDES) $(OBJS)
