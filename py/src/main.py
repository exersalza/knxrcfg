#!/bin/python3

import os
import sys
import re
import io

from typing import Any

FILE_REG_BARE = r"(\.knxrcfg$)|(\.kxcfg$)"
FILE_REG_NAME = r"(([a-zA-Z0-9_\-]*)\.knxrcfg$)|(([a-zA-Z0-9_\-]*)\.kxcfg$)"

LINE_CATEGORY_REG = r"(\(.*\))"

class NoConfigError(Exception):
    pass


class Config:
    def __init__(self, path: str = None) -> None:
        self._path: str = path
        self._configs = {"bare": [], "named": []}
        self._config = {}

        self.__load_config()

    def __load_config(self):
        if not self._path:
            if not self.__find_config():
                raise FileNotFoundError("Config cannot be found due to no config in root/sub directory or none is given.")


        with open(self._path, 'r', encoding="utf-8") as f:
            self.__lexer(f)
        
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
        
        if len(self._configs["bare"]):
            self._path = self._configs["bare"][0]
        
        if len(self._configs["named"]) and not self._path:
            self._path = self._configs["named"][0]
        

        return c

    def __lexer(self, file_handle: io.TextIOWrapper):
        category = ""

        for i in file_handle.readlines():
            # get newlines for unix and windows based systems and also comments and just yeet them, 
            # no one needs them, also later run a check for newlines bc of categorys
            if i in ("\n", "\r\n") or i.startswith("//"):
                if not i.startswith("//"):
                    category = ""

                continue
            
            if "//" in i:
                comment = i.index("//")
                i = i[:comment]
            
            if re.match(LINE_CATEGORY_REG, i):
                category = i.strip().strip("()")
                self._config[category] = {}
                continue

            key, value = self.__key_and_value(i)

            if not category:
                self._config[key] = value
                continue
            
            self._config[category][key] = value

    def __key_and_value(self, i: str) -> tuple:
        index = i.index("=")
        value = str(i[index + 1:].strip().strip("\n\r\t\"'"))  # strip of tab, string and newlines

        value = self.__convert_to_numeric(value)

        return i[:index], value
    

    def __convert_to_numeric(self, value: str) -> int|float:
        try:
            if '.' in value:
                value = float(value)
                return value
            
            value = int(value)

        except ValueError:
            pass
        
        return value

    def get(self, key) -> Any:
        if not self._config:  # edge case when somehow someone is trying to get the config while its not even loaded.
            raise NoConfigError("No config has been loaded yet.")

        return self._config.get(key, None)

    def __getitem__(self, key) -> Any:
        return self.get(key)

    def __repr__(self):
        return self._config

    def __str__(self):
        return str(self._config)


if __name__ == "__main__":
    config = Config()
    print(config)
    