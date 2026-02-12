# EECE446-Program-1-Group-10
Program 1 HTML Tag and Byte Counter Prof. Kredo Due: By 23:59 Sunday, February 15

## h1-counter TCP Client

A TCP client that connects to www.ecst.csuchico.edu:80, retrieves an HTML file, and counts `<h1>` tags.

### Building

```bash
make
```

### Usage

```bash
./h1-counter <chunk_size>
```

Where `chunk_size` is a number between 5 and 1000 that specifies the receive buffer size.

### Example

```bash
./h1-counter 100
```

### Output

The program outputs two numbers:
- Total count of `<h1>` tags found
- Total bytes received

Format: `<tag_count> <byte_count>`

### Implementation Details

- Uses `getaddrinfo()` with `AF_UNSPEC` for address resolution
- Connects to www.ecst.csuchico.edu on port 80
- Sends HTTP/1.0 GET request for `/~kkredo/file.html`
- Implements send-loop for reliable sending
- Implements recv-loop for reliable receiving
- Uses `memmem()` library function for efficient tag searching
- Only counts tags that appear fully within each received chunk
- Compiles with `-Wall` with no warnings

### Cleaning

```bash
make clean
```
