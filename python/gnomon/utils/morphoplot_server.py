import sys
import pickle
import time
import morphonet
import signal
import threading
import zmq

import numpy as np

class MorphoPlot():
    """helper class to launch a morpho plot server and communicate with it.
    """

    def __init__(self, *args, **kwargs) -> None:
        self.context = zmq.Context()
        self.m_socket = self.context.socket(zmq.REP)
        self.m_socket.bind("tcp://127.0.0.1:5555")
        self.mc = morphonet.Plot(clear_temp=True)
        self.mc.set_dataset(begin=0,end=0, background=1)
        self.config = False
        self.launch_ready = True
        
    @staticmethod
    def parent_death_handler(self,sig, frame):
        print("in death method")
        if self.mc is not None:
            self.mc.quit_and_exit()
            exit(0) 

    def _set_morpho_data(self, t: int, data): #data np.Array
        if t > self.mc.dataset.end:
            self.mc.dataset.end = t

        self.mc.dataset.seg_from_disk[t] = False
        self.mc.dataset.set_seg(t, data)
        self.config = True
        self.mc.curate()
    
    def local_server(self):
        """ test server
        """
        test_time = 0
        print("launching server, listening on 5555 for data")
        while True:
            data_received = pickle.loads(self.m_socket.recv())
            self._set_morpho_data(test_time, data_received)
            break

    #def run_morphoplot(self):
    ##    while self.launch_ready:
    #        if self.config:
    #            self.mc.curate()
    #            self.mc.wait_for_servers()
    ##            self.config = False
    #        time.sleep(1)

    

def main():
    print("startint morphoplt server")
    signal.signal(signal.SIGHUP, MorphoPlot.parent_death_handler)

    mplot = MorphoPlot()
    localServer = threading.Thread(target=mplot.local_server)
    localServer.start()
    #mplot.run_morphoplot()

if __name__ == "__main__":
    main()