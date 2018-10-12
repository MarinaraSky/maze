CFLAGS += -Wall -Wextra -Wpedantic

CFLAGS += -std=c11


shortest: LDLIBS += -lcrypto 
shortest: CFLAGS+= -g
shortest: shortest.o Dijkstra.o graph/Graph.o graph/GraphSerializer.o map/map.o map/vmap.o dpqueue/pqueue.o
