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
  llvmlinkOut = binLoc+'/'+outName+".ll"
  llcOut = binLoc+'/'+outName+".s"
  s2wasmOut = binLoc+'/'+outName+".wat"
  wat2wasmOut = binLoc+'/'+outName+".wasm"

  clangCmd = ["clang"
  , "-IC:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.11.25503/include"
  , "-IC:/Program Files (x86)/Windows Kits/10/Include/10.0.15063.0/ucrt"
  , "-IC:/Program Files (x86)/Windows Kits/10/include/10.0.15063.0/shared"
  , "-IC:/Program Files (x86)/Windows Kits/10/include/10.0.15063.0/um"
  , "-IC:/Program Files (x86)/Windows Kits/10/include/10.0.15063.0/winrt"
  , "-fdeclspec"
  ,"-S", "--target=wasm32"
  , "-std=c++14", "-Oz"
  , "-v", "-c"
  , "source/*.cpp"]

  llcCmd = ["-asm-verbose=false", "-o", llcOut, llvmlinkOut]

  s2wasmCmd = [llcCmd, ">", s2wasmOut]

  wat2wasmCmd = [s2wasmOut, "-o", wat2wasmOut]

  # Make sure the build directory exists
  os.makedirs(binLoc, exist_ok=True)

  # Run build commands
  try:
    subprocess.run(clangCmd, shell=True, check=True)
  except subprocess.CalledProcessError as e:
    print("Error: " + ' '.join(e.cmd) + " returned code: " + str(e.returncode) + "\nCheck output above for more information, stopping build.")
    return
  
  # The llvm-link command requires knowledge of the output from the clang command
  # so we assemble it here
  llvmlinkCmd = ["llvm-link", "-S", "-v", "-o", llvmlinkOut]
  for file in os.listdir(binLoc):
    if(file.endswith(".ll")):
      llvmlinkCmd.append(file)

  try:
    subprocess.run(llvmlinkCmd, shell=True, check=True)
  except subprocess.CalledProcessError as e:
    print("Error: " + ' '.join(e.cmd) + " returned code: " + str(e.returncode) + "\nCheck output above for more information, stopping build.")
    return

  try:
    subprocess.run(llcCmd, shell=True, check=True)
  except subprocess.CalledProcessError as e:
    print("Error: " + ' '.join(e.cmd) + " returned code: " + str(e.returncode) + "\nCheck output above for more information, stopping build.")
    return

  try:
    subprocess.run(s2wasmCmd, shell=True, check=True)
  except subprocess.CalledProcessError as e:
    print("Error: " + ' '.join(e.cmd) + " returned code: " + str(e.returncode) + "\nCheck output above for more information, stopping build.")
    return

  try:
    subprocess.run(wat2wasmCmd, shell=True, check=True)
  except subprocess.CalledProcessError as e:
    print("Error: " + ' '.join(e.cmd) + " returned code: " + str(e.returncode) + "\nCheck output above for more information, stopping build.")
    return

if __name__ == "__main__":
  main(sys.argv)
