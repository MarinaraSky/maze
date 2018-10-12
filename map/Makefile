include ../Makefile

driver: LDLIBS += -lcrypto
driver: driver.o map.o

debug: CFLAGS += -g
debug: driver

profile: CFLAGS += -pg
profile: LDFLAGS += -pg
profile: driver

clean:
	$(RM) *.o driver

.PHONY: debug profile clean
