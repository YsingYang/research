INCLUDE =  ieee80211.h ieee80211_radiotap.h 

INCLUDES = -I $(INCLUDE)

#CFLAGS += -Wall -std=gnu99 -g
LDFLAGS += -lpcap

router:router.o
	$(CC) router.o -o router -lpcap 

router.o:router.c routerForwardPacket.h $(INCLUDE)
	$(CC) -c  router.c

clean:
	rm *.o router
