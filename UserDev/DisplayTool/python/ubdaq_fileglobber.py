try:
    from pyqtgraph.Qt import QtGui, QtCore
    import pyqtgraph as pg
    import glob, os, time
except ImportError:
    raise ImportError('You need to set up pyqtgraph before you can use this utility.')

class UBDaqFileGlobber(QtCore.QObject):
    # Class to constantly glob the ubdaq pnfs file directory and emit a signal if a new file is found
    # assuming that new files has nonzero size.

    new_file_ready = QtCore.pyqtSignal(str)

    def __init__(self):
        QtCore.QObject.__init__(self)

        self.file_base_dir = '/pnfs/uboone/scratch/uboonepro/dropbox/data/uboone/raw/'
        self.glob_pattern = '*.ubdaq'
        self.current_file = None
        self.file_checking_period = 10 #check for a new file every X seconds
    
        # self.timer = QtCore.QTimer()
        # self.timer.timeout.connect(self.FindFileAndEmitSignal)
        # self.timer.start(self.file_checking_period)

        while True:
            self.FindFileAndEmitSignal()
            time.sleep(self.file_checking_period)

    def FindFileAndEmitSignal(self):
        #This line checks for the newest file in the directory. It occasionally throws an OSError
        #in which case we just return (don't emit any signals)
        try:
            newest = max(glob.iglob(self.file_base_dir+self.glob_pattern), key=os.path.getctime)
        except OSError as e:
            return

        #Sanity check: does file exist? If not, return (don't emit any signals)
        if not os.path.isfile(newest):
            return

        #Sanity check: does the file have zero size? If so, return (don't emit any signals)
        if os.stat(newest).st_size == 0:
            return

        #Is this "newest" file one for which we've already emitted a signal?
        if self.current_file != newest:
            self.current_file = newest
            self.new_file_ready.emit(newest)
        else: 
            #No new files that we haven't already found. return (don't emit any signals)
            return
