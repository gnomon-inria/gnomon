import sys
import pickle
import time
import signal
import threading
import zmq

import numpy as np

import morphonet
from morphonet.plot import MorphoInfo

from .morphonetHelper import _dict_from_info

class MorphoPlot():
    """helper class to launch a morpho plot server and communicate with it.
    """

    def __init__(self, *args, **kwargs) -> None:
        self.context = zmq.Context()
        self.m_socket = self.context.socket(zmq.REP)
        self.m_socket.bind("tcp://127.0.0.1:5555")
        self.mc = morphonet.Plot(clear_temp=True)
        self.mc.set_dataset(begin=0, end=0, background=1)
        self.timestamps = {}
        self.tissue_args = {}
        self.readyToCurate = False
        
    #@staticmethod
    ##def parent_death_handler(self,sig, frame):
    #    print("in death method")
    #    if self.mc is not None:
    #        self.mc.quit_and_exit()
    #        exit(0) 

    def _set_morpho_data(self, t: int, data: np.ndarray, voxelsize: tuple[float, float, float]):
        if t > self.mc.dataset.end:
            self.mc.dataset.end = t

        self.mc.dataset.seg_from_disk[t] = False
        self.mc.dataset.voxel_size_by_t[t] = voxelsize
        self.mc.dataset.set_seg(t, data)
        self.config = True

    
    def data_handler(self):
        """ test server
        """
        print("launching server, listening on 5555 for data")
        while True:
            data_json = self.m_socket.recv_json()
            request = data_json["request"]
            if data_json["request"] == "set":
                print("set data!")
                data_received = np.asarray(data_json["data"], dtype=np.uint16)
                self.tissue_args[data_json["index"]] = data_json["tissue_args"]
                voxelsize = self.tissue_args[data_json["index"]]["voxelsize"]
                self.timestamps[data_json["index"]] = data_json["time"]
                self._set_morpho_data(data_json["index"], data_received, voxelsize)
                print("sending response")
                self.m_socket.send_json({"response": "data received"})
                print("response ok")

            elif request == "set_infos":
                infos = data_json["infos"]
                for info_name, info_string in infos.items():
                    info_type, _ = _dict_from_info(info_string)
                    info = MorphoInfo(self.mc.dataset, info_name, info_type)
                    info.add_data(info_string)
                    self.mc.dataset.infos[info_name] = info
                self.m_socket.send_json({"response": "Dataset information set"})

            elif data_json["request"] == "launch":
                print("launch curate!") # if necessary
                self.readyToCurate = True
                self.m_socket.send_json({"response": "Curation started"})

            elif data_json["request"] == "collect":
                print("collect data!")
                datas = {i_t: data.tolist() for i_t, data in self.mc.dataset.seg_datas.items()}
                infos = {info_name: info.get_txt() for info_name, infos in self.mc.dataset.infos.items()}
                self.m_socket.send_json({"response": "ok", "data": datas, "infos": infos, "timestamps": self.timestamps, "tissue_args": self.tissue_args})

            elif data_json["request"] == "kill":
                print("kill!") # if necessary
                exit(0)

    def run_morphoplot(self):
        while True:
            if self.readyToCurate:
                self.mc.curate()
    #            self.mc.wait_for_servers()
                self.readyToCurate = False
            time.sleep(1)



def main():
    print("startint morphoplt server")
    mplot = MorphoPlot()

    #def collect_data(sig, frame):
    #    print("signal: ", sig, "frame:", frame)
    #    print(mplot)

    #signal.signal(signal.SIGTERM, collect_data)
    #signal.signal(signal.SIGHUP, collect_data)
    #signal.signal(signal.SIGHINT, collect_data)

    #signal.signal(signal.SIGHUP, MorphoPlot.parent_death_handler)

    dataHandler = threading.Thread(target=mplot.data_handler)
    dataHandler.start()
    mplot.run_morphoplot()
    dataHandler.join()


if __name__ == "__main__":
    main()