from qtpy import QtCore
from qtpy.QtCore import Qt

from qtconsole.rich_jupyter_widget import RichJupyterWidget
from qtconsole.inprocess import QtInProcessKernelManager


class InProcessJupyterWidget(RichJupyterWidget):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.kernel_manager = QtInProcessKernelManager()
        self.kernel_manager.start_kernel()
        self.kernel_client = self._kernel_manager.client()
        self.kernel_client.start_channels()

        self.enable_calltips = False
        self._display_banner = False

        def stop():
            self.kernel_client.stop_channels()
            self.kernel_manager.shutdown_kernel()

        self.exit_requested.connect(stop)


