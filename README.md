# DNS-client

A DNS resolver written from scratch in C. Builds raw DNS query packets, sends them over UDP to `8.8.8.8`, and parses the response. No libraries — just sockets and RFC 1035.

## Usage

1.  **Create Domain List:**
    * Make a file named `domain_name_list.txt`.
    * Add domains, one per line (e.g., `www.google.com`).

2.  **Set Query Type (IPv4/IPv6):**
    * Open `main.c` and find the `make_dns_query` call.
    * To find **IPv4** addresses, use `A`:
        ```c
        make_dns_query(..., A);
        ```
    * To find **IPv6** addresses, use `AAAA`:
        ```c
        make_dns_query(..., AAAA);
        ```

## Building

```bash
make
```

Or manually:

```bash
gcc dns_client/main.c dns_client/dns_protocol.c dns_client/network.c dns_client/utils.c -o dns_client_bin
```

## Example Output

```bash
./dns_client
DNS Query Packet for www.google.com (length: 32 bytes):
Received 48 bytes in response
IP Address: 142.250.75.100
---
DNS Query Packet for www.yahoo.com (length: 31 bytes):
Received 108 bytes in response
IP Address: 87.248.119.251
IP Address: 87.248.119.252
---
DNS Query Packet for www.youtube.com (length: 33 bytes):
Received 163 bytes in response
IP Address: 142.250.75.46
IP Address: 142.250.75.110
IP Address: 142.250.75.78
IP Address: 142.250.75.206
IP Address: 142.250.75.174
IP Address: 142.250.75.142
---