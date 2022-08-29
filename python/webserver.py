import argparse
from http.server import BaseHTTPRequestHandler, HTTPServer


class Server(BaseHTTPRequestHandler):
  def do_GET(self):
    print("client_address", self.client_address)
    print("server", self.server)
    print("requestline", self.requestline)
    print("command", self.command)
    print("path", self.path)
    print("request_version", self.request_version)
    print("server_version", self.server_version)
    print("sys_version", self.sys_version)
    print("error_content_type", self.error_content_type)
    print("protocol_version", self.protocol_version)
    print("headers", self.headers)
    print("do_GET invoked")

    self.send_response(200)
    self.send_header("Content-type", "application/json")
    self.end_headers()
    self.wfile.write(bytes("{\"name\": \"Updated item\", \"year\": \"2010\"}", "utf-8"))


  def do_PUT(self):
    print("client_address", self.client_address)
    print("server", self.server)
    print("requestline", self.requestline)
    print("command", self.command)
    print("path", self.path)
    print("request_version", self.request_version)
    print("server_version", self.server_version)
    print("sys_version", self.sys_version)
    print("error_content_type", self.error_content_type)
    print("protocol_version", self.protocol_version)
    print("headers", self.headers)
    print("do_PUT invoked")

    self.send_response(200)
    self.send_header("Content-type", "application/json")
    self.end_headers()
    self.wfile.write(bytes("{\"name\": \"Updated item\", \"year\": \"2010\"}", "utf-8"))


if __name__ == "__main__":    
    parser = argparse.ArgumentParser(description="Run web server")
    parser.add_argument("-n", "--hostName", help = "Hostname", required=False, default="localhost")
    parser.add_argument("-p", "--serverPort", help = "ServerPort", required=False, default=8080)
    args = parser.parse_args()

    webServer = HTTPServer((args.hostName, args.serverPort), Server)
    print("Server started http://%s:%s" % (args.hostName, args.serverPort))

    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server stopped.")


# curl -i -X GET http://localhost:8080
# curl -i -X PUT -H 'Content-Type: application/json' -d '{"name": "Updated item", "year": "2010"}' http://localhost:8080

# Application	application/EDI-X12
# application/EDIFACT
# application/javascript
# application/octet-stream
# application/ogg
# application/pdf
# application/xhtml+xml
# application/x-shockwave-flash
# application/json
# application/ld+json
# application/xml
# application/zip
# application/x-www-form-urlencoded
# Audio	audio/mpeg
# audio/x-ms-wma
# audio/vnd.rn-realaudio
# audio/x-wav
# Image	image/gif
# image/jpeg
# image/png
# image/tiff
# image/vnd.microsoft.icon
# image/x-icon
# image/vnd.djvu
# image/svg+xml
# Multipart	multipart/mixed
# multipart/alternative
# multipart/related (using by MHTML (HTML mail).)
# multipart/form-data
# Text	text/css
# text/csv
# text/html
# text/javascript (obsolete)
# text/plain
# text/xml
# Video	video/mpeg
# video/mp4
# video/quicktime
# video/x-ms-wmv
# video/x-msvideo
# video/x-flv
# video/webm
# VND	application/vnd.oasis.opendocument.text
# application/vnd.oasis.opendocument.spreadsheet
# application/vnd.oasis.opendocument.presentation
# application/vnd.oasis.opendocument.graphics
# application/vnd.ms-excel
# application/vnd.openxmlformats-officedocument.spreadsheetml.sheet
# application/vnd.ms-powerpoint
# application/vnd.openxmlformats-officedocument.presentationml.presentation
# application/msword
# application/vnd.openxmlformats-officedocument.wordprocessingml.document
# application/vnd.mozilla.xul+xml