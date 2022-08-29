from typing import Tuple, Dict
from flask import Flask, request
from threading import Thread, Lock
import time
import datetime
import json

# Do not modify this line or any function signatures.
app = Flask(__name__)

class snippet:
    def __init__(self, name, code, expires_in, url_base):
        self.name = name
        self.snippet = code
        self.expires_at = time.time() + int(expires_in)
        self.url = url_base + "/" + self.name
        self.likes = 0
        
        if expires_in < 0:
            raise Exception("expires_in cannot be negative")
        
    def touch(self):
        self.expires_at = self.expires_at + 5
        
    def like(self):
        self.touch()
        self.likes = self.likes + 1
    
    def expiresAtAsString(self):
        dt = datetime.datetime.fromtimestamp(self.expires_at)
        return dt.strftime("%Y-%m-%dT%H:%M:%SZ")
    
    def asJSON(self):
        class __snippet__:
            def __init__(self, snippet):
                self.name = snippet.name
                self.snippet = snippet.snippet
                self.expires_at = snippet.expiresAtAsString()
                self.url = snippet.url
                self.likes = snippet.likes
        s = __snippet__(self)
        return json.dumps(s.__dict__)
        
    def asBytes(self):
        return bytes(self.asJSON(), 'utf-8')

lock = Lock()  # Really want a read/write lock
snippets = {}  # A table of all the created snippets

@app.route("/snippets", methods=["POST"])
def make_snippet() -> Tuple[bytes, int]:
    """
    Process & validate a new snippet.

    It corresponds to `POST /snippets`.

    Return the response bytes (for example, marshaled JSON) and an appropriate
    HTTP status code.
    """
    json = request.get_json()
    url_base = "http://" + request.host + "/snippets"
    response_code = 200
    response_data = b""
    
    lock.acquire()
    try:
        s = snippet(json["name"], json["snippet"], int(json["expires_in"]), url_base)
        if s.name not in snippets:
            snippets[s.name] = s
            response_data = s.asBytes()
        else:
            response_code = 400
            response_data = b"Snippet already created"
    except BaseException as err:
        response_code = 400
        response_data = bytes(str(err), "utf-8")
    finally:
        lock.release()
    
    return response_data, response_code

def process_snippet(name, functor):
    """
    Searches for an existing snippet with the given name
    and runs the functor against it.
    """
    response_code = 200
    response_data = b""
    
    lock.acquire()
    try:
        if name in snippets:
            snippet = snippets[name]
            if snippet.expires_at > time.time():
                functor(snippet)
                response_data = snippet.asJSON()
            else:
                response_code = 404
                response_data = b"Snippet has expired"    
        else:
            response_code = 404
            response_data = b"Snippet does not exist" 
    except BaseException as err:
        response_code = 400
        response_data = bytes(str(err), "utf-8")        
    finally:
        lock.release()
    
    return response_data, response_code

@app.route("/snippets/<name>", methods=["GET"])
def get_snippet(name: str) -> Tuple[bytes, int]:
    """
    Process requests for a snippet by a name.

    It correponds to `GET /snippets/<name>`.

    Like `make_snippet()`, it should return the response bytes and an
    appropriate HTTP status code.
    """
    name = request.path.split("/")[-1] 
    return process_snippet(name, lambda x : x.touch())


@app.route("/snippets/<name>/like", methods=["POST"])
def like_snippet(name: str) -> Tuple[bytes, int]:
    """
    Process a like request to a snippet by name.

    It correponds to `POST /snippets/<name>/like`.
    """
    name = request.path.split("/")[-2]
    return process_snippet(name, lambda x : x.like())


# If running from the custom script, run the web server in a background
# process for 10 seconds (so that it's easily, cleanly killable).
if __name__ == "__main__":
    # this lets us import the runner
    import os, sys, inspect

    currentdir = os.path.dirname(
        os.path.abspath(inspect.getfile(inspect.currentframe()))
    )
    sys.path.insert(0, os.path.join(currentdir, "tests"))

    import runner
    import time

    bg = runner.ServerManager.create(app)
    bg.start()
    time.sleep(10)
    bg.stop()
