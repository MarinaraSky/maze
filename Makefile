CFLAGS += -Wall -Wextra -Wpedantic

CFLAGS += -std=c11


maze: LDLIBS += -lcrypto 
maze: maze.o Dijkstra.o graph/Graph.o graph/GraphSerializer.o map/map.o map/vmap.o dpqueue/pqueue.o

debug: CFLAGS += -g
debug: LDLIBS += -lcrypto 
debug: maze.o Dijkstra.o graph/Graph.o graph/GraphSerializer.o map/map.o map/vmap.o dpqueue/pqueue.o
	$(CC) $(CFLAGS) $^ -o maze $(LDLIBS)

profile: CFLAGS += -pg
profile: LDLIBS += -lcrypto 
profile: maze.o Dijkstra.o graph/Graph.o graph/GraphSerializer.o map/map.o map/vmap.o dpqueue/pqueue.o
	$(CC) $(CFLAGS) $^ -o maze $(LDLIBS)

.PHONY: clean

clean:
	rm -f *.o *.out ./graph/*.o ./dpqueue/*.o ./map/*.o maze

