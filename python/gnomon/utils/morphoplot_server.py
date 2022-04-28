import sys
import zmq
import pickle
import time
import numpy as np
import morphonet
import signal
import threading

class MorphoPlot():
    def __init__(self, *args, **kwargs) -> None:
        self.context = zmq.Context()
        self.m_socket = self.context.socket(zmq.REP)
        self.m_socket.bind("tcp://*:5050")
        self.mc = morphonet.Plot(start_browser=True, clear_temp=True, only_compute_mesh=False)
        self.config = False
        self.launch_ready = True
        # sys.argv[1] to be remplaced by data received through socket
        # self.mc.set_dataset(begin=0, end=0, background=1, segment=sys.argv[1], factor=5, memory=20)

    @staticmethod
    def parent_death_handler(self,sig, frame):
        if self.mc is not None:
            self.mc.quit_and_exit()
            exit(0)

    def _set_morpho_data(self, file_path):
        self.mc.set_dataset(begin=0, end=0, background=1, segment=file_path, factor=5, memory=20)
        # self.mc.set_seg(1,data)
    
    def local_server(self):
        test_data = np.zeros((3,3))
        i_i = 0
        while True:
            message = self.m_socket.recv()
            print(f"Received request : {message}")
            time.sleep(1)
            self.m_socket.send(pickle.dumps(test_data))
            data_received = pickle.loads(self.m_socket.recv())
            if i_i == 0:
                self._set_morpho_data(data_received)
                self.config = True
                i_i += 1
            print(f"Received data : {data_received}")
            time.sleep(1)
            mn_data = self.mc.get_info("Cell Name")
            self.m_socket.send(pickle.dumps(mn_data))


    def run_morphoplot(self):
        while self.launch_ready:
            if self.config:
                self.mc.curate()
                self.mc.wait_for_servers()
                self.config = False
            time.sleep(1)

    

def main():
    signal.signal(signal.SIGHUP, MorphoPlot.parent_death_handler)
    mplot = MorphoPlot()
    localServer = threading.Thread(target=mplot.local_server)
    localServer.start()
    mplot.run_morphoplot()



if __name__ == "__main__":
    main()