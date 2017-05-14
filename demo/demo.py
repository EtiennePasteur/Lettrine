#!/usr/bin/env python3

from sys import argv
from http.server import BaseHTTPRequestHandler, HTTPServer
from os.path import isdir, realpath, dirname
from os import listdir, chdir

__version__ = "0.1.0"
__usage__ = """\
Usage: %s DIR [PORT]

        DIR             book directory to view
        PORT            port to listen, default is 8080


Lettrine demo %s - Open Library Hackathon 2017"""
CONFIG = realpath(dirname(argv[0])) + "/"
DOM_IMAGE = """\
<a class="thumbnail" href="#thumb">
  <img class="mini" src="%s"
       border="0" />
  <span><img src="%s" /></span>
</a>"""
# body = "<style>%s</style>" % open("style.css", "r").read()
body = """\
<head>
        <link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">
</head>
<div class="background-image"></div>
"""

def content(path):
    try:
        return open(path, "rb").read()
    except:
        return open(CONFIG + path, "rb").read()

class Handler(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()
        message = bytes(body, "utf8") if self.path == "/" else content(self.path[1:])
        self.wfile.write(message)

def page(filename):
    new = "_".join(filename.split("_")[:-1]) + ".jpg"
    return ("pics/" + filename, "img/" + new)

def usage(name):
    print(__usage__ % (name, __version__))
    return 1

def main(argv):
    if not(1 < len(argv) < 4):
        return usage(argv[0])
    path = realpath(argv[1])
    if not isdir(path):
        return usage(argv[0])
    print("Entering %s directory..." % path)
    chdir(path)
    global body
    page.root = path
    body += "\n".join([ DOM_IMAGE % page(i) for i in listdir(path + "/pics") ])

    port = int(argv[2]) if len(argv) == 3 else 8080
    print("Serving %s directory on port %s..." % (argv[1], port))
    httpd = HTTPServer(("localhost", port), Handler)
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print(" bye!")
    return 0

if __name__ == '__main__':
    exit(main(argv))
