import socket
import zmq
import pickle
import numpy as np
import sys

# import gnomon.utils.morphonetHelper as helper

# mn = helper.MorphonetHelper()
# mesh = mn.transform_to_mn_mesh(cell_img, 1)

def main():

    context = zmq.Context()

    print("Connecting to my local Server")
    m_socket = context.socket(zmq.REQ)
    m_socket.connect("tcp://localhost:9875")

    test_data_reply = np.eye(3)

    print(sys.argv)
    for request in range(5):
        print(f"sending request {request}")

        # m_socket.send(pickle.dumps("Hello"))
        m_socket.send_string("Hello!")

        message = pickle.loads(m_socket.recv())
        print(f"Received reply 0 {request}[ {message} ]")

        m_socket.send(pickle.dumps(sys.argv[1]))

        message = pickle.loads(m_socket.recv())
        print(f"Received reply 1 {request}[ {message} ]")

def handle_data():
    # uncode data to be sent
    # decode data received
    pass

# disconnect from server til a connection signal received
    
if __name__ == "__main__":
    main()
        