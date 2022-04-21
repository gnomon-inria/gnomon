import sys
import signal
import morphonet

mc = None

def parent_death_handler(sig, frame):
    global mc
    if mc is not None:
        mc.quit_and_exit()
        exit(0)


signal.signal(signal.SIGHUP, parent_death_handler)

def run_morphoplot():
    global mc
    print(sys.argv)
    mc = morphonet.Plot(start_browser=True, clear_temp=True, only_compute_mesh=False)
    mc.set_dataset(begin=0, end=0, background=1, segment=sys.argv[1], factor=5, memory=20)
    mc.curate()
