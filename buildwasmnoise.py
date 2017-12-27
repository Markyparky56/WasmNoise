import configparser
import sys
import subprocess
import os
import enum
from removeextraexports import removeExtraExports

class BuildType(enum.IntEnum):
  build = 0
  patch = 1
  minor = 2
  major = 3  

buildTypeLookup = ["build", "patch", "minor", "major"]

class FunctionEnableType(enum.IntEnum):
  EnableAll = 0
  EnablePerlin = 1
  EnablePerlinFractal = 2
  EnableAllPerlin = 3

# Lookup maps to exportNames array
enableTypeLookup = [
  [0], # EnableAll has special case
  [0, 2], # Enable Perlin
  [0, 1, 3], # Enable Perlin Fractal
  [0, 1, 2, 3] # Enable Perlin and Perlin Fractal
]
exportNames = ["getset", "fractalGetSet", "perlin", "perlinFractal"]


class TextColours:
  Blue = '\033[94m'
  Green = '\033[92m'
  Red = '\033[91m'  
  StopColour = '\033[0m'

def getEnabledExportsSet(enableType):
  enabledExports = set()
  lookupArr = enableTypeLookup[int(enableType)]
  for i in lookupArr:
    enabledExports.add(exportNames[i])
  return enabledExports

def getEnabledFunctions(enables):
  enabledFunctions = set()
  if FunctionEnableType.EnableAll in enables:
    return exportNames
  for enable in enables:
    enabledFunctions |= getEnabledExportsSet(enable)
  return enabledFunctions        

def runCommand(cmd):
  try:
    subprocess.run(cmd, shell=True, check=True)
  except subprocess.CalledProcessError as e:
    print(TextColours.Red + "Error: " + ' '.join(e.cmd) + " returned code: " + str(e.returncode) + "\nCheck output above for more information, stopping build." + TextColours.StopColour)
    exit()

# Args:
# --build to increment the build counter in version.ini
# --patch to increment the patch counter in version.ini and zero build
# --minor to increment the minor counter in version.ini and zero build and patch
# --major to increment the major counter in version.ini and zero the rest
# TODO: Consider optimisation flags and other compiler options
def main(args):
  recognisedBuildTypeArgs = {
    "-build": BuildType.build, 
    "-patch": BuildType.patch,
    "-minor": BuildType.minor,
    "-major": BuildType.major
  }
  recognisedOptimisationArgs = ["-O0", "-O1", "-O2", "-O3"]
  verboseArg = "-v"
  recognisedEnableArgs = {
    "-EnableAll": FunctionEnableType.EnableAll,
    "-EnablePerlin": FunctionEnableType.EnablePerlin,
    "-EnablePerlinFractal": FunctionEnableType.EnablePerlinFractal,
    "-EnableAllPerlin": FunctionEnableType.EnableAllPerlin
  }

  buildType = BuildType(0)
  optimisationLevel = "-O3"
  verboseMode = False  
  enableFlags = []

  if(len(args) > 1):
    # For each arg
    for arg in args:
      doneWithArg = False
      if "buildwasmnoise.py" in arg:
        continue
      # Check if it is a recognised Build Type
      for recongisedArg, value in recognisedBuildTypeArgs.items():
        if arg.strip() == recongisedArg:
          buildType = value
          doneWithArg = True
          break
      if doneWithArg:
        continue

      # Check if it is a recognised optimisation arg
      for recongisedArg in recognisedOptimisationArgs:
        if arg.strip() == recongisedArg:
          optimisationLevel = arg
          doneWithArg = True
          break
      if doneWithArg:
        continue

      # Check if it is a recognised enable arg
      for recognisedArg, value in recognisedEnableArgs.items():
        if arg.strip() == recognisedArg:
          enableFlags.append(value)
          doneWithArg = True
          break
      if doneWithArg:
        continue

      # Check if it is a verbose arg
      if arg.strip() == verboseArg:
        verboseMode = True
        continue

      # Else, unrecognised arg
      print("Ignoring Unrecongised Option '", arg, "'")

  # Assume enable all if no flags provided
  if len(enableFlags) == 0:
    enableFlags.append(FunctionEnableType.EnableAll)

  print("Building WasmNoise, incrementing", buildTypeLookup[int(buildType)])
  build(buildType, optimisationLevel, verboseMode, enableFlags)

def build(buildType, optLevel, verbose, enabledFlags):
  iniLoc = "version.ini"

  # Create a configparser and load version.ini
  config = configparser.ConfigParser()
  config.read(iniLoc)

  # Increment the build version
  config["VERSION"][buildTypeLookup[int(buildType)]] = str(int(config["VERSION"][buildTypeLookup[buildType]])+1)

  # Reset lower values
  if(buildType == BuildType.major): 
    config["VERSION"]["minor"] = str(0)
    config["VERSION"]["patch"] = str(0)
    config["VERSION"]["build"] = str(0)
  elif(buildType == BuildType.minor): 
    config["VERSION"]["patch"] = str(0)
    config["VERSION"]["build"] = str(0)
  elif(buildType == BuildType.patch):
    config["VERSION"]["build"] = str(0)

  # Output the new value
  print("New Version: " + config["VERSION"]["major"] + '.' + config["VERSION"]["minor"] + '.' + config["VERSION"]["patch"] + '.' + config["VERSION"]["build"])

  # File output name (minus file type)
  outName = "wasmnoise-" + config["VERSION"]["major"] + '.' + config["VERSION"]["minor"] + '.' + config["VERSION"]["patch"]

  # Save the modified file
  with open(iniLoc, 'w') as configfile:
    config.write(configfile)

  # Prepare the enable functions array
  enabledFunctions = getEnabledFunctions(enabledFlags)
  for enable in enabledFunctions:
    print(TextColours.Blue + "Enabling function set: " + enable + TextColours.StopColour)

  # Grab all code files from source directory
  codefiles = []
  for file in os.listdir("source"):
    if(file.endswith(".cpp") or file.endswith(".c")): # Is .c necessary?
      codefiles.append(file)

  # Assemble build commands
  binLoc = "bin/" + outName + '.b' + config["VERSION"]["build"]
  llvmlinkOut = outName+".bc"
  llcOut = outName+".s"
  s2wasmOut = outName+".wat"
  processedWat = outName+".cleanexports.wat"
  wat2wasmOut = outName+".wasm"
  wasmoptOut = outName+".opt.wasm"
  optimisationLevel = optLevel

  clangCmd = ["clang++"
  , "--target=wasm32"
  , "-emit-llvm"
  , "-std=c++14", optimisationLevel
  , "-c"
  , "-I..\..\..\wasm-stdlib-hack\include\libc"
  , "../../source/*.cpp"
  , "-pedantic", "-Wall", "-Wextra"]
  
  if verbose:
    clangCmd.append("-v")

  llcCmd = ["llc", "-asm-verbose=false", optimisationLevel, "-o", llcOut, llvmlinkOut]

  s2wasmCmd = ["s2wasm"
  , "-s", "524288"
  , "--import-memory"
  , llcOut, ">", s2wasmOut]

  wat2wasmCmd = ["wat2wasm", processedWat, "-o", wat2wasmOut]

  wasmoptCmd = ["wasm-opt", optimisationLevel, wat2wasmOut, "-o", wasmoptOut]

  # Make sure the build directory exists
  os.makedirs(binLoc, exist_ok=True)

  # Change the working directory to the build directory
  os.chdir(binLoc)

  # Run build commands
  print(TextColours.Blue + "Compiling With clang..." + TextColours.StopColour)
  runCommand(clangCmd)
  
  # The llvm-link command requires knowledge of the output from the clang command
  # so we assemble it here
  llvmlinkCmd = ["llvm-link", "-v", "-o", llvmlinkOut]
  for file in os.listdir('.'):
    if(file.endswith(".bc")):
      llvmlinkCmd.append(file)
  # Include memory.bc 
  llvmlinkCmd.append("../../source/memory-bitcode/memory.bc")

  print(TextColours.Blue + "Linking with llvm-link..." + TextColours.StopColour)
  runCommand(llvmlinkCmd)
  
  print(TextColours.Blue + "Converting to S-expressions with llc..." + TextColours.StopColour)
  runCommand(llcCmd)
  
  print(TextColours.Blue + "Converting S-expressions to wat..." + TextColours.StopColour)
  runCommand(s2wasmCmd)

  # Remove extra exports from the wat file before compiling
  print(TextColours.Blue + "Removing unwanted exports from wat file..." + TextColours.StopColour)
  removeExtraExports(s2wasmOut, enabledFunctions)
  
  print(TextColours.Blue + "Compiling wat to wasm..." + TextColours.StopColour)
  runCommand(wat2wasmCmd)

  print(TextColours.Blue + "Passing compiled wasm through wasm-opt to try to achieve faster and smaller binary..." + TextColours.StopColour)
  runCommand(wasmoptCmd)

  print(TextColours.Green + "Wasm compiled successfully! " + wat2wasmOut + " file now located at " + binLoc + TextColours.StopColour)

if __name__ == "__main__":
  main(sys.argv)
