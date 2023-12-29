import os
import re

with open("build/cpp.js", "r+") as file:
    contents = file.read()

    # update to correct path
    newContents = re.sub("cpp.wasm", "/static/wasm/cpp.wasm", contents)

    # some more modifications
    newContents = newContents.replace("""if (typeof exports === 'object' && typeof module === 'object')
  module.exports = Module;
else if (typeof define === 'function' && define['amd'])
  define([], () => Module);""", "export default Module;")

    # delete contents
    file.seek(0)
    file.truncate()

    # add new contents
    file.write(newContents)

# move files to proper places
os.replace("build/cpp.js", "../cpp.js")
os.replace("build/cpp.wasm", "../../static/wasm/cpp.wasm")