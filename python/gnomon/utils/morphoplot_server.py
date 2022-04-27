import socket
import zmq
import pickle
import time
import numpy as np


context = zmq.Context()
m_socket = context.socket(zmq.REP)
m_socket.bind("tcp://*:9876")


def handle_client():
    # set up client config
    # decode data received from client
    # encode data to be sent to client
    pass

def start():
    # launch morphoplot into a local Port
    # listen to client
    test_data = np.zeros((3,3))
    while True:
        message = m_socket.recv()
        # message = pickle.loads(m_socket.recv())
        print(f"Received request : {message}")
        time.sleep(1)
        m_socket.send(pickle.dumps(test_data))
        data_received = pickle.loads(m_socket.recv())
        print(f"Received data : {data_received}")
        time.sleep(1)
        m_socket.send(pickle.dumps("Final state"))


if __name__ == "__main__":
    start()


    