import time
import logging
import threading
import zmq

import numpy as np

import morphonet
from morphonet.plot import MorphoInfo

from timagetk import LabelledImage
from timagetk.algorithms.resample import resample, isometric_resampling


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
    #    logging.debug("in death method")
    #    if self.mc is not None:
    #        self.mc.quit_and_exit()
    #        exit(0) 

    def _set_morpho_data(self, t: int, data: np.ndarray, voxelsize: tuple[float, float, float], meshing_voxelsize=0.5):
        if t > self.mc.dataset.end:
            self.mc.dataset.end = t

        self.mc.dataset.seg_from_disk[t] = False

        image_voxelsize = voxelsize[0]
        if not all([v == image_voxelsize for v in voxelsize]):
            seg_img = LabelledImage(data, voxelsize=voxelsize, not_a_label=0)
            resampled_seg_img = isometric_resampling(seg_img, method='min', interpolation='nearest')
            seg_data = resampled_seg_img.get_array()
            image_voxelsize = resampled_seg_img.voxelsize[0]
        else:
            seg_data = data
        logging.debug(f"--> Resampling: {voxelsize} -> {(image_voxelsize, image_voxelsize, image_voxelsize)}  ({data.shape} -> {seg_data.shape})")

        factor = int(np.round(meshing_voxelsize / image_voxelsize))
        logging.debug(f"--> Setting factor {factor} for meshing")
        self.mc.factor = factor
        self.mc.dataset.voxel_size_by_t[t] = [image_voxelsize, image_voxelsize, image_voxelsize]
        self.mc.dataset.set_seg(t, np.transpose(seg_data, (2,1,0)))
        self.config = True

    
    def data_handler(self):
        """ test server
        """
        logging.debug("launching server, listening on 5555 for data")
        while True:
            data_json = self.m_socket.recv_json()
            request = data_json["request"]
            if data_json["request"] == "set":
                logging.debug("set data!")
                data_received = np.asarray(data_json["data"], dtype=np.uint16)
                self.tissue_args[data_json["index"]] = data_json["tissue_args"]
                voxelsize = self.tissue_args[data_json["index"]]["voxelsize"]
                self.timestamps[data_json["index"]] = data_json["time"]
                meshing_voxelsize = data_json["meshing_voxelsize"]
                self._set_morpho_data(data_json["index"], data_received, voxelsize, meshing_voxelsize)
                logging.debug("sending response")
                self.m_socket.send_json({"response": "data received"})
                logging.debug("response ok")

            elif request == "set_infos":
                infos = data_json["infos"]
                for info_name, info_string in infos.items():
                    info_type, _ = _dict_from_info(info_string)
                    info = MorphoInfo(self.mc.dataset, info_name, info_type)
                    info.add_data(info_string)
                    self.mc.dataset.infos[info_name] = info
                self.m_socket.send_json({"response": "Dataset information set"})

            elif data_json["request"] == "launch":
                logging.debug("launch curate!") # if necessary
                self.readyToCurate = True
                self.m_socket.send_json({"response": "Curation started"})

            elif data_json["request"] == "collect":
                logging.debug("collect data!")
                datas = {}
                for i_t, data in self.mc.dataset.seg_datas.items():
                    voxelsize = self.mc.dataset.voxel_size_by_t[i_t]
                    resampled_seg_img = LabelledImage(np.transpose(data, (2,1,0)), voxelsize=voxelsize, not_a_label=0)
                    seg_img = resample(resampled_seg_img, voxelsize=self.tissue_args[i_t]["voxelsize"], interpolation='nearest')
                    logging.debug(f"--> De-resampling: {voxelsize} -> {seg_img.voxelsize} ({data.shape} -> {seg_img.shape})")
                    datas[i_t] = seg_img.get_array().tolist()
                infos = {info_name: infos.get_txt() for info_name, infos in self.mc.dataset.infos.items()}
                self.m_socket.send_json({"response": "ok", "data": datas, "infos": infos, "timestamps": self.timestamps, "tissue_args": self.tissue_args})

            elif data_json["request"] == "kill":
                logging.debug("kill!") # if necessary
                exit(0)

    def run_morphoplot(self):
        while True:
            if self.readyToCurate:
                self.mc.curate()
    #            self.mc.wait_for_servers()
                self.readyToCurate = False
            time.sleep(1)



def main():
    logging.info("Starting MorphoPlot server")
    mplot = MorphoPlot()

    #def collect_data(sig, frame):
    #    logging.debug("signal: ", sig, "frame:", frame)
    #    logging.debug(mplot)

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