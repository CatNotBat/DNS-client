CC = gcc
CFLAGS = -Wall -Wextra
SRC = dns_client/main.c dns_client/dns_protocol.c dns_client/network.c dns_client/utils.c
OUT = dns_client_bin

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OUT)

.PHONY: all clean
