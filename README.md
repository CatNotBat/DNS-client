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