from qtpy import QtCore
from qtpy.QtCore import Qt

from IPython.lib import guisupport

from qtconsole.rich_jupyter_widget import RichJupyterWidget
from qtconsole.inprocess import QtInProcessKernelManager


class InProcessJupyterWidget(RichJupyterWidget):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.kernel_manager = QtInProcessKernelManager()
        self.kernel_manager.start_kernel()
        # kernel_manager.kernel.gui = 'qt4' # Ignored
        self.kernel_client = self._kernel_manager.client()
        self.kernel_client.start_channels()

        def stop():
            self.kernel_client.stop_channels()
            self.kernel_manager.shutdown_kernel()
            # guisupport.get_app_qt4().exit()

        self.exit_requested.connect(stop)
        self.setAttribute(Qt.WA_StyledBackground, True)

