# Pico HTTP Server in C to fuzz with Honggfuzz NetDriver
## How to use
1. export HONGGFUZZ_DIR=/path/to/honggfuzz
2. CC=$HONGGFUZZ_DIR/hfuzz_cc/hfuzz-clang make
3. _HF_TCP_PORT=8080 $HONGGFUZZ_DIR/honggfuzz -i ./corpus --dict $HONGGFUZZ_DIR/examples/apache-httpd/httpd.wordlist --workspace ./crashes -output ./new_corpus -- ./server

## Links
Based on <https://www.cnblogs.com/hac425/p/9416915.html>


### Original README of Pico HTTP Server in C

This is a very simple HTTP server for Unix, using `fork()`. It's very easy to use.

#### How to use

1. Include header `httpd.h`.
2. Write your route method, handling requests.
3. Call `serve_forever("8000")` to start serving on http://127.0.0.1:8000/.

See `main.c`, an interesting example.

To log stuff, use `fprintf(stderr, "message");`

View `httpd.h` for more information.

#### Quick start

1. Run `make`.
2. Run `./server` or `./server [port]` (port = 8000 by default).
3. Open http://localhost:8000/ or http://localhost:8000/test in browser to see request headers.

#### Testing and benchmarking

I suggest using [Siege](https://github.com/JoeDog/siege) utility for testing and benchmarking the Pico HTTP server.

```sh
> siege -i -f urls.txt
```

#### Links

Reworked and refactored from <https://gist.github.com/laobubu/d6d0e9beb934b60b2e552c2d03e1409e>.

Based on <http://blog.abhijeetr.com/2010/04/very-simple-http-server-writen-in-c.html>.
