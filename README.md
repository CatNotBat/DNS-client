# DNS-client

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

To compile the project, run the following `gcc` command from the root of the directory:

```bash
gcc main.c dns_protocol.c network.c utils.c -o dns_client
```

## Example Output

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