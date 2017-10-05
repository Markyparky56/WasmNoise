import configparser
import sys
import subprocess
import os
import enum

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

# Args:
# --build to increment the build counter in version.ini
# --patch to increment the patch counter in version.ini and zero build
# --minor to increment the minor counter in version.ini and zero build and patch
# --major to increment the major counter in version.ini and zero the rest
# TODO: Consider optimisation flags and other compiler options
def main(args):
  recognisedArgs = {
    "--build": BuildType.build, 
    "--patch": BuildType.patch,
    "--minor": BuildType.minor,
    "--major": BuildType.major
  }

  buildType = BuildType(0)

  if(len(args) > 1):
    # For each arg
    for arg in args:
      # Check if it's recognised
      for recongisedArg, value in recognisedArgs.items():
        if arg == key:
          buildType = value
          break
      else:
        print("Ignoring Unrecongised Option '", arg, "'")

  print("Building WasmNoise, incrementing ", referenceLookup[int(buildType)])
  build(buildType)

def build(buildType):
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
  wat2wasmOut = outName+".wasm"
  optimisationLevel = "-O3" # -O3 for final?

  clangCmd = ["clang"
  , "--target=wasm32"
  , "-emit-llvm"
  , "-std=c++14", optimisationLevel
  , "-v", "-c"
  , "../../source/*.cpp"]

  llcCmd = ["llc", "-asm-verbose=false", optimisationLevel, "-o", llcOut, llvmlinkOut]

  s2wasmCmd = ["s2wasm", llcOut, ">", s2wasmOut]

  wat2wasmCmd = ["wat2wasm", s2wasmOut, "-o", wat2wasmOut]

  # Make sure the build directory exists
  os.makedirs(binLoc, exist_ok=True)

  # Change the working directory to the build directory
  os.chdir(binLoc)

  # Run build commands
  print(TextColours.Blue + "Compiling With clang..." + TextColours.StopColour)
  try:
    subprocess.run(clangCmd, shell=True, check=True)
  except subprocess.CalledProcessError as e:
    print(TextColors.Red + "Error: " + ' '.join(e.cmd) + " returned code: " + str(e.returncode) + "\nCheck output above for more information, stopping build." + TextColours.StopColour)
    return
  
  # The llvm-link command requires knowledge of the output from the clang command
  # so we assemble it here
  llvmlinkCmd = ["llvm-link", "-v", "-o", llvmlinkOut]
  for file in os.listdir('.'):
    if(file.endswith(".bc")):
      llvmlinkCmd.append(file)

  print(TextColours.Blue + "Linking with llvm-link..." + TextColours.StopColour)
  try:
    subprocess.run(llvmlinkCmd, shell=True, check=True)
  except subprocess.CalledProcessError as e:
    print(TextColours.Red + "Error: " + ' '.join(e.cmd) + " returned code: " + str(e.returncode) + "\nCheck output above for more information, stopping build." + TextColours.StopColour)
    return
  
  print(TextColours.Blue + "Converting to S-expressions with llc..." + TextColours.StopColour)
  try:
    subprocess.run(llcCmd, shell=True, check=True)
  except subprocess.CalledProcessError as e:
    print(TextColours.Red + "Error: " + ' '.join(e.cmd) + " returned code: " + str(e.returncode) + "\nCheck output above for more information, stopping build." + TextColours.StopColour)
    return
  
  print(TextColours.Blue + "Converting S-expressions to wat..." + TextColours.StopColour)
  try:
    subprocess.run(s2wasmCmd, shell=True, check=True)
  except subprocess.CalledProcessError as e:
    print(TextColours.Red + "Error: " + ' '.join(e.cmd) + " returned code: " + str(e.returncode) + "\nCheck output above for more information, stopping build." + TextColours.StopColour)
    return
  
  print(TextColours.Blue + "Compiling wat to wasm..." + TextColours.StopColour)
  try:
    subprocess.run(wat2wasmCmd, shell=True, check=True)
  except subprocess.CalledProcessError as e:
    print(TextColours.Red + "Error: " + ' '.join(e.cmd) + " returned code: " + str(e.returncode) + "\nCheck output above for more information, stopping build." + TextColours.StopColour)
    return

  print(TextColours.Green + "Wasm compiled successfully! " + wat2wasmOut + " file now located at " + binLoc + TextColours.StopColour)

if __name__ == "__main__":
  main(sys.argv)
