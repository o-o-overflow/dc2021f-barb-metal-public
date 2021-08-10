from websocket import create_connection
from threading import Thread
from time import sleep
import socket, sys
import base64

class myClassA(Thread):
    def __init__(self):
        Thread.__init__(self)
        self.daemon = True
        self.start()
    def run(self):
        while True:
            x = ""
            x = ws.recv()
            x = base64.b64decode(x)
            print(x)
            connection.sendall(x + b"\n")

host = sys.argv[1]
port = sys.argv[2]
ws = create_connection("ws://" + host + ":" + port + "/")

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# Bind the socket to the port
server_address = ('localhost', 1338)
print('starting up on {} port {}'.format(*server_address))
sock.bind(server_address)

# Listen for incoming connections
sock.listen(1)


# Wait for a connection
print('waiting for a connection')
connection, client_address = sock.accept()
myClassA()
try:
    print('connection from', client_address)

    # Receive the data in small chunks and retransmit it
    while True:
        data = connection.recv(1)
        if data:
            ws.send(base64.b64encode(data))
        else:
            print('no data from', client_address)
            break

finally:
    # Clean up the connection
    connection.close()

