import json
import os
import os.path
import subprocess
import time

class CVServer:
    def __init__(self):
        pass

    def Start(self, GPU = True):
        command = "humdroid_cpu"
        if GPU and self._IsCommand("humdroid_gpu"):
            command = "humdroid_gpu"

        if not self._IsCommand("humdroid_cpu"):
            raise Exception("humdroid_cpu is not installed or on PATH.")

        self.server = subprocess.Popen([command,], stdout=subprocess.PIPE, shell=False)

    def Close(self):
        self.server.kill()

    def _IsCommand(self, command) -> bool:
        rc = subprocess.call(['which', command], stdout=subprocess.DEVNULL)

        return rc == 0

