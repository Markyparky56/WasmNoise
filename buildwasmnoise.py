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
  
referenceLookup = ["build", "patch", "minor", "major"]

class TextColours:
  Blue = '\033[94m'
  Green = '\033[92m'
  Red = '\033[91m'  
  StopColour = '\033[0m'

def runCommand(cmd):
  try:
    subprocess.run(cmd, shell=True, check=True)
  except subprocess.CalledProcessError as e:
    print(TextColours.Red + "Error: " + ' '.join(e.cmd) + " returned code: " + str(e.returncode) + "\nCheck output above for more information, stopping build." + TextColours.StopColour)
    return

# Args:
# --build to increment the build counter in version.ini
# --patch to increment the patch counter in version.ini and zero build
# --minor to increment the minor counter in version.ini and zero build and patch
# --major to increment the major counter in version.ini and zero the rest
# TODO: Consider optimisation flags and other compiler options
def main(args):
  recognisedBuildTypeArgs = {
    "--build": BuildType.build, 
    "--patch": BuildType.patch,
    "--minor": BuildType.minor,
    "--major": BuildType.major
  }

  recognisedOptimisationArgs = ["-O0", "-O1", "-O2", "-O3"]

  verboseArg = "-v"

  buildType = BuildType(0)
  optimisationLevel = "-O3"
  verboseMode = False  

  if(len(args) > 1):
    # For each arg
    for arg in args:
      # Check if it's recognised
      for recongisedArg, value in recognisedBuildTypeArgs.items():
        if arg == recongisedArg:
          buildType = value
          break
      for recongisedArg in recognisedOptimisationArgs:
        if arg == recongisedArg:
          optimisationLevel = arg
      if arg == verboseArg:
        verboseMode = True
      else:
        print("Ignoring Unrecongised Option '", arg, "'")

  print("Building WasmNoise, incrementing ", referenceLookup[int(buildType)])
  build(buildType, optimisationLevel, verboseMode)

def build(buildType, optLevel, verbose):
  iniLoc = "version.ini"

  # Create a configparser and load version.ini
  config = configparser.ConfigParser()
  config.read(iniLoc)

  # Increment the build version
  config["VERSION"][referenceLookup[int(buildType)]] = str(int(config["VERSION"][referenceLookup[buildType]])+1)

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

  clangCmd = ["clang"
  , "--target=wasm32"
  , "-emit-llvm"
  , "-std=c++14", optimisationLevel
  , "-c"
  , "-I..\..\..\wasm-stdlib-hack\include\libc"
  , "../../source/*.cpp"]
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
  removeExtraExports(s2wasmOut)
  
  print(TextColours.Blue + "Compiling wat to wasm..." + TextColours.StopColour)
  runCommand(wat2wasmCmd)

  print(TextColours.Blue + "Passing compiled wasm through wasm-opt to try to achieve faster and smaller binary..." + TextColours.StopColour)
  runCommand(wasmoptCmd)

  print(TextColours.Green + "Wasm compiled successfully! " + wat2wasmOut + " file now located at " + binLoc + TextColours.StopColour)

if __name__ == "__main__":
  main(sys.argv)
