"""
Module to process the wat file output by s2wasm, and remove unneeded exported functions
"""
def canRemoveExport(export, exports, enabledFlags):
  """
  Cycles through each exported function, comparing its name to those in the enabled
  set of functions, if it isn't present it returns true
  """
  # Get the function name within the export declaration
  funcName = export.split('"')[1]
  if funcName in exports["exports"]["getset"]["funcs"]:
    return False # Is listed in exports.getset, so we can't remove it
  else:
    for enabled in enabledFlags:
      if funcName in exports["exports"][enabled]["funcs"]:
        return False
    return True

def removeExtraExports(fileName, enabledFlags, exports):
  """
  Writes out a modified version of the input file, removing functions which aren't enabled
  """
  # Construct the new file name, inserting cleanexports before the file ending
  fileParts = fileName.split('.')
  length = len(fileParts)
  fileParts.insert(length-1, "cleanexports")
  newfile = '.'.join(fileParts)

  with open(fileName, "r") as pre:
    with open(newfile, "w") as post:
      # Check every line
      for line in pre:
        # If it's an export, check if it's one we want
        if "export" in line:
          # Check if the exported function is in the exportsfile
          if canRemoveExport(line, exports, enabledFlags):
            continue
          else:
            post.write(line)
        # If it's not an export leave it alone
        else:
          post.write(line)
