#!/usr/bin/env python3

from sys import argv
from http.server import BaseHTTPRequestHandler, HTTPServer

__version__ = "0.1.0"
__usage__ = """\
Usage: %s DIR [PORT]

        DIR             book directory to view
        PORT            port to listen, default is 8080


Lettrine demo %s - Open Library Hackathon 2017"""

class testHTTPServer_RequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header('Content-type','text/html')
        self.end_headers()

        message = "Hello world!"
        self.wfile.write(bytes(message, "utf8"))

def usage(name):
    print(__usage__ % (name, __version__))
    return 1

def main(argv):
    if not(1 < len(argv) < 4):
        return usage(argv[0])
    folder = argv[1]
    port = int(argv[2]) if len(argv) == 3 else 8080
    print("Serving %s directory on port %s..." % (argv[1], port))
    httpd = HTTPServer(("localhost", port), testHTTPServer_RequestHandler)
    httpd.serve_forever()
    print("bye!")
    return 0

if __name__ == '__main__':
    exit(main(argv))
