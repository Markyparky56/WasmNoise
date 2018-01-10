"""
Build script to compile WasmNoise from C++ to a WebAssembly Binary file
Requires Python 3
"""
import configparser
import sys
import subprocess
import os
import enum
import json
from removeextraexports import removeExtraExports
from constructautoloader import outputAutloaderFile

class BuildType(enum.IntEnum):
  """Enum for build types"""
  build = 0
  patch = 1
  minor = 2
  major = 3

buildTypeLookup = ["build", "patch", "minor", "major"]

class FunctionEnableType(enum.IntEnum):
  """Enum for function sets"""
  EnableAll = 0
  EnablePerlin = 1
  EnablePerlinFractal = 2
  EnableAllPerlin = 3
  EnableSimplex = 4
  EnableSimplexFractal = 5
  EnableAllSimplex = 6
  EnableCellular = 7

# Lookup maps to exportNames array
enableTypeLookup = [
  [0], # EnableAll has special case
  [0, 2], # Enable Perlin
  [0, 1, 3], # Enable Perlin Fractal
  [0, 1, 2, 3], # Enable Perlin and Perlin Fractal
  [0, 4], # Enable Simplex
  [0, 1, 5], # Enable Simplex Fractal
  [0, 1, 4, 5], # Enable Simplex and Simplex Fractal
  [0, 6, 7] # Enable Cellular
]
exportNames = [
  "getset",         #0
  "fractalGetSet",  #1
  "perlin",         #2
  "perlinFractal",  #3
  "simplex",        #4
  "simplexFractal", #5
  "cellularGetSet", #6  
  "cellular",       #7
]

class TextColours:
  """Class to store Text Colours"""
  Blue = '\033[94m'
  Green = '\033[92m'
  Red = '\033[91m'  
  StopColour = '\033[0m'

def getEnabledExportsSet(enableType):
  """
  Constructs a set of strings by mapping the indexes in
  the corresponding enableTypeLookup array to the function sets in exportNames
  """
  enabledExports = set()
  lookupArr = enableTypeLookup[int(enableType)]
  for i in lookupArr:
    enabledExports.add(exportNames[i])
  return enabledExports

def getEnabledFunctions(enables):
  """
  Constructs a set of enabled function groups by adding (ORing) together all the
  enabled sets. Bypasses this loop if EnableAll is present.
  """
  enabledFunctions = set()
  if FunctionEnableType.EnableAll in enables:
    return exportNames
  for enable in enables:
    enabledFunctions |= getEnabledExportsSet(enable)
  return enabledFunctions        

def runCommand(cmd):
  """
  Helper to run subprocesses and catch errors they may return
  """
  try:
    subprocess.run(cmd, shell=True, check=True)
  except subprocess.CalledProcessError as err:
    print(TextColours.Red + "Error: " + ' '.join(err.cmd) + " returned code: " + str(err.returncode) + "\nCheck output above for more information, stopping build." + TextColours.StopColour)
    exit()

# Args:
# --build to increment the build counter in version.ini
# --patch to increment the patch counter in version.ini and zero build
# --minor to increment the minor counter in version.ini and zero build and patch
# --major to increment the major counter in version.ini and zero the rest
def main(args):
  """
  Main Function
  Processes arguments passed to the script and then configures the build process
  """
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
    "-EnableAllPerlin": FunctionEnableType.EnableAllPerlin,
    "-EnableSimplex": FunctionEnableType.EnableSimplex,
    "-EnableSimplexFractal": FunctionEnableType.EnableSimplexFractal,
    "-EnableAllSimplex": FunctionEnableType.EnableAllSimplex,
    "-EnableCellular": FunctionEnableType.EnableCellular
  }
  helpArgs = ["-h", "-help", "--h", "--help", "-H", "--H"]
  allowAbortArg = "-AllowAbort"

  buildType = BuildType(0)
  optimisationLevel = "-O3"
  verboseMode = False  
  allowAbort = False
  enableFlags = []

  if(len(args) > 1):
    # Check for a help arg
    if any(arg in args for arg in helpArgs):
      print(
        "Allowed Configuration Arguments As Follows:\n",
        "Build Types:\n",
        "\t-build\tIncrement Build Number\n",
        "\t-patch\tIncrement Patch Number\n",
        "\t-minor\tIncrement Minor Number\n",
        "\t-major\tIncrement Major Number\n",
        "Optimisation Args:\n",
        "\t -O0, -O1, -O2, -O3\n",
        "Verbose Arg:\n",
        "\t-v\n",
        "Enable Function Sets With:\n",
        "\tCombine as necessary\n",
        "\t-EnableAll\t\tEnable All Function Sets\n",
        "\t-EnablePerlin\t\tEnable Non-Fractal Perlin Functions Only\n",
        "\t-EnablePerlinFractal\tEnable Fractal Perlin Functions Only\n",
        "\t-EnableAllPerlin\tEnable All Perlin Functions (Fractal and Non-Fractal)\n",
        "\t-EnableSimplex\t\tEnable Non-Fractal Simplex Functions Only\n",
        "\t-EnableSimplexFractal\tEnable Fractal Simplex Functions Only\n",
        "\t-EnableAllSimplex\t\tEnable All Simplex Functions (Fractal and Non-Fractal)\n",
        "\t-EnableCellular\t\tEnable Cellular Functions\n",
        "Allow Abort Alerts:\n",
        "(Only recommended for testing and development, not for production)\n",
        "\t-AllowAbort\n",
        "This Help Message -\n",
        "\t-h --h -H --H -help --help"
      )
      return

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

      # Check if it is an allowAbort arg
      if arg.strip() == allowAbortArg:
        allowAbort = True
        continue

      # Else, unrecognised arg
      print("Ignoring Unrecongised Option '", arg, "'")

  # Assume enable all if no flags provided
  if not enableFlags:
    enableFlags.append(FunctionEnableType.EnableAll)

  print("Building WasmNoise, incrementing", buildTypeLookup[int(buildType)])
  build(buildType, optimisationLevel, verboseMode, allowAbort, enableFlags)

def build(buildType, optLevel, verbose, allowAbort, enabledFlags):
  #TODO: Break version increment off into own function for neatness
  """
  Build process, multi-step
  """
  iniLoc = "version.ini"

  # Create a configparser and load version.ini
  config = configparser.ConfigParser()
  config.read(iniLoc)

  # Increment the build version
  config["VERSION"][buildTypeLookup[int(buildType)]] = str(
    int(config["VERSION"][buildTypeLookup[buildType]])+1
  )

  # Reset lower values
  if buildType == BuildType.major: 
    config["VERSION"]["minor"] = str(0)
    config["VERSION"]["patch"] = str(0)
    config["VERSION"]["build"] = str(0)
  elif buildType == BuildType.minor: 
    config["VERSION"]["patch"] = str(0)
    config["VERSION"]["build"] = str(0)
  elif buildType == BuildType.patch:
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
  enabledFuncMacros = []
  for enable in enabledFunctions:
    print(TextColours.Blue + "Enabling function set: " + enable + TextColours.StopColour)

  # Load the exports json file and collect macros for enabled function sets
  exportsFile = open("wasmnoiseexports.json", "r")
  exports = json.load(exportsFile)

  for enabledFuncSet in enabledFunctions:
    try:
      enabledFuncMacros.append(exports["exports"][enabledFuncSet]["macro"])
    except KeyError:
      continue
  
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

  clangCmd = [
    "clang++",
    "--target=wasm32",
    "-emit-llvm",
    "-std=c++14",
    optimisationLevel,
    "-c",
    "-I..\\..\\..\\wasm-stdlib-hack\\include\\libc",
    "../../source/*.cpp",
    "-pedantic",
    "-Wall",
    "-Wextra"
  ]
  for macro in enabledFuncMacros:
    clangCmd.append(macro)
  if allowAbort:
    clangCmd.append("-DWN_ALLOW_ABORT")
  
  if verbose:
    clangCmd.append("-v")

  llcCmd = [
    "llc",
    "-asm-verbose=false",
    optimisationLevel,
    "-o", llcOut,
    llvmlinkOut
  ]

  s2wasmCmd = [
    "s2wasm",
     "-s", "524288",
     "--import-memory",
     llcOut, ">", s2wasmOut
  ]

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
    if file.endswith(".bc"):
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
  removeExtraExports(s2wasmOut, enabledFunctions, exports)

  print(TextColours.Blue + "Compiling wat to wasm..." + TextColours.StopColour)
  runCommand(wat2wasmCmd)

  print(TextColours.Blue + "Passing compiled wasm through wasm-opt to try to achieve faster and smaller binary..." + TextColours.StopColour)
  runCommand(wasmoptCmd)

  print(TextColours.Green + "Wasm compiled successfully! " + wat2wasmOut + " file now located at " + binLoc + TextColours.StopColour)
  
  print(TextColours.Blue + "Writing Autoloader Script..." + TextColours.StopColour)
  outputAutloaderFile(wasmoptOut, enabledFunctions, exports)  
  print(TextColours.Green + "wasmnoise.autoloader.js written successfully!" + TextColours.StopColour)

if __name__ == "__main__":
  main(sys.argv)
