"""
Returns a string with all the --export=<value> switches for wasm-ld
"""
def getExports(exports, enabledFlags):
  exportsArr = []
  for enabled in enabledFlags:
    for func in exports["exports"][enabled]["funcs"]:
      export = "--export=" + func
      exportsArr.append(export)
  return exportsArr
