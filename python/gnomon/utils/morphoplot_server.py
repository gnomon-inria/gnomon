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
        self.mc.set_dataset()
        self.config = False
        self.launch_ready = True
        
    @staticmethod
    def parent_death_handler(self,sig, frame):
        if self.mc is not None:
            self.mc.quit_and_exit()
            exit(0)

    def _set_morpho_data(self, data):
        self.mc.dataset.set_seg(1,data)
    
    def local_server(self):
        test_data = np.zeros((3,3))
        while True:
            message = self.m_socket.recv()
            print(f"Received request : {message}")
            time.sleep(1)
            self.m_socket.send(pickle.dumps(test_data))
            data_received = pickle.loads(self.m_socket.recv())
            self._set_morpho_data(data_received)
            self.config = True
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