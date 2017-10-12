import json

def canRemoveExport(export, exports):
  funcName = export.split('"')[1]
  if funcName in exports["exports"]:
    return False # Is listed in exports, so we can't remove it
  else:
    return True

def removeExtraExports(fileName):
  fileParts = fileName.split('.')
  length = len(fileParts)
  fileParts.insert(length-1, "cleanexports")
  newfile = '.'.join(fileParts)

  with open("../../wasmnoiseexports.json", "r") as exportsfile:
    exports = json.load(exportsfile)
    with open(fileName, "r") as pre:
      with open(newfile, "w") as post:
        # Check every line
        for line in pre:
          # If it's an export, check if it's one we want
          if "export" in line:
            # Check if the exported function is in the exportsfile
            if canRemoveExport(line, exports):
              continue
            else:
              post.write(line)
          # If it's not an export leave it alone
          else:
            post.write(line) 
