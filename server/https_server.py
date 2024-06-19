#!/usr/bin/env python3

from http.server import HTTPServer, BaseHTTPRequestHandler
import ssl
import logging
logger = logging.getLogger(__name__)

#Set logging config
logging.basicConfig(filename='https_server.log', level=logging.INFO)

from io import BytesIO

getData = "GET response body!"
postData = [0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38]

class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):
    def do_HEAD(self):
        logger.info("Received HEAD")
        self.send_response(200)
        self.send_header("Content-Type", "application/octet-stream") # this is sent
        self.end_headers()
        self.wfile.write(b'HEAD response!') # does not get sent
        print(self.path)

    def do_GET(self):
        logger.info("Received GET")
        self.send_response(200)
        self.send_header("Content-Type", "application/octet-stream")
        self.send_header("Content-Length", str(len(getData.encode('utf-8'))))
        self.end_headers()
        self.wfile.write(bytearray(getData.encode('utf-8')))
        print(self.path)

    def do_POST(self):
        logger.info("Received POST")
        content_length = int(self.headers['Content-Length'])
        body = self.rfile.read(content_length)
        self.send_response(200)
        self.send_header("Content-Type", "application/octet-stream")  # for binary data
        self.send_header("Content-Length", str(len(postData)))
        self.end_headers()

        # Print body content of POST
        print(body)

        # Send binary response to client
        response = BytesIO()
        response.write(bytearray(postData))
        self.wfile.write(response.getvalue())

def run(server_class=HTTPServer, handler_class=SimpleHTTPRequestHandler, port=8080):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    
    logger.info("Starting HTTP server on port %i", port)
    print('running HTTP server on port ' + str(port))
    httpd.serve_forever()


def run_https(server_class=HTTPServer, handler_class=SimpleHTTPRequestHandler, port=8585, https=False, key=None, cert=None):
    context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    context.load_cert_chain(certfile=cert, keyfile=key, password='')
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    httpd.socket = context.wrap_socket(httpd.socket, server_side=True)

    logger.info("Starting HTTPS server on port %i", port)
    print('running HTTPS server on port ' + str(port))
    httpd.serve_forever()


# Create argparse object
def getArgs():
    import argparse
    import pathlib

    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--port", help="Port number to listen for connections", type=int, default=8585)
    parser.add_argument("-s", "--ssl", help="Set true for HTTPS", type=bool, default=False)
    parser.add_argument("-k", "--key", help="Path to key file. For HTTPS", type=pathlib.Path)
    parser.add_argument("-c", "--cert", help="Path to cert file. For HTTPS", type=pathlib.Path)
    args = parser.parse_args()

    return args


if __name__ == '__main__':
    Args=getArgs()
    if Args.ssl is True:
        if Args.key is None or Args.cert is None:
            print("Key and Certificate required")
        else:
            run_https(port=Args.port, https=Args.ssl, key=Args.key, cert=Args.cert)
    else:
        run(port=Args.port)
