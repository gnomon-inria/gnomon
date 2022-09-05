import os
import platform
import socket
import threading
from time import sleep

ADDR = ("localhost", 54600)
TIMEOUT = 10  # second


class StreamCapture:
    """
    Adapted from https://stackoverflow.com/a/66808947
    """
    def __init__(self, streams: list, echo=True, monkeypatch=None):
        self.active = True
        self.streams = streams
        self.echo = echo
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.settimeout(TIMEOUT)
        self.sock.connect(ADDR)
        self.fd = {}
        self.dup_fd = {}
        self.oldwrite = {}
        self.threads = {}
        self.pipe_write_fd = {}
        for stream in self.streams:
            self.fd[stream] = stream.fileno()
            (pipe_read_fd, pipe_write_fd) = os.pipe()
            self.pipe_write_fd[stream] = pipe_write_fd
            self.dup_fd[stream] = os.dup(self.fd[stream])
            os.dup2(pipe_write_fd, self.fd[stream])
            self.monkeypatch = monkeypatch if monkeypatch is not None else platform.system() == 'Windows'
            if self.monkeypatch:
                self.oldwrite[stream] = stream.write
                stream.write = lambda z: os.write(self.fd[stream], z.encode() if type(z) == str else z)
            t = threading.Thread(target=self.printer, args=(pipe_read_fd, self.dup_fd[stream]))
            self.threads[stream] = t
            t.start()

    def printer(self, input_fd, echo_fd):
        while True:
            sleep(0.001)
            data = os.read(input_fd, 100000)
            if len(data) == 0:
                os.close(echo_fd)
                os.close(input_fd)
                # print("====== Closing connection")
                self.sock.close()
                return
            try:
                self.sock.sendall(data)
            except OSError:
                os.close(echo_fd)
                os.close(input_fd)
                print("====== Closing connection due to error")
                print(data)
                self.sock.close()
                return
            if self.echo:
                os.write(echo_fd, data)

    def close(self):
        # print("====== Closing stream dup")
        if not self.active:
            return
        self.active = False
        for stream in self.streams:
            stream.flush()
            if self.monkeypatch:
                stream.write = self.oldwrite[stream]
            os.dup2(self.dup_fd[stream], self.fd[stream])
            os.close(self.pipe_write_fd[stream])

    def __enter__(self):
        return self

    def __exit__(self, a, b, c):
        self.close()

    def __del__(self):
        self.close()


# for testing
if __name__ == "__main__":
    import sys
    print("starttttt")
    logger = None
    try:
        logger = StreamCapture([sys.stdout, sys.stderr], echo=True)
    except Exception as e:
        print("Could not initialize logger.")
        print(e)
    # base run
    print("log connection established")
    print("I am running")
    print("I have finished")

    # cleanup
    if logger:
        logger.close()
        
    #server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    #server.bind(ADDR)
    #client = None
    #try:
    #    server.listen(1)
    #
    #    print("Waiting for client")
    #    client, client_address = server.accept()
    #    print("Connection accepted : ", client, client_address)
    #    while True:
    #        # rlist, wlist, xlist = select.select([client], [], [])
    #        data = client.recv(4096)
    #        if data:
    #            print(str(data, encoding="utf-8"), end="")
    #        sleep(0.01)
    #finally:
    #    server.close()
    #    if client:
    #        client.close()
