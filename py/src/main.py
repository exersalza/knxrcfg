#!/bin/python3

import os
import sys
import re

from typing import Any

FILE_REG_BARE = r"(\.knxrcfg$)|(\.kxcfg$)"
FILE_REG_NAME = r"(([a-zA-Z0-9_\-]*)\.knxrcfg$)|(([a-zA-Z0-9_\-]*)\.kxcfg$)"

class NoConfigError(Exception):
    pass


class Config:
    def __init__(self, path: str = None) -> None:
        self._PATH: str = path # persistent, but maybe not for long
        self._file: str = None # changed later to the file path, maybe dropable when you dont want a persistent _path var.
        self._configs = {"bare": [], "named": []}
        self._config = {}

        self.__load_config()

    def __load_config(self):
        if not self._PATH:
            if not self.__find_config():
                raise FileNotFoundError("Config cannot be found due to no config in root/sub directory or none is given.")

        
    def __find_config(self) -> int:
        c = 0
        for root, _, files in os.walk('.'):
            for file in files:
                if re.match(FILE_REG_BARE, file):
                    self._configs["bare"].append(f"{root}/{file}")
                    c += 1
                    continue

                if re.match(FILE_REG_NAME, file):
                    c += 1
                    self._configs["named"].append(f"{root}/{file}")

        return c

    def __lexer(self):
        pass
    

    def get(self, key) -> Any:
        if not self._config:
            raise NoConfigError("No config has been loaded yet.") 

        return self._config.get(key, None)

    def __getitem__(self, key) -> Any:
        return self.get(key)


if __name__ == "__main__":
    config = Config()
    