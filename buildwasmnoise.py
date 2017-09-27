import configparser
import sys
import subprocess

def main(numArgs, args):
    if(numArgs >= 2): 
        if(numArgs == 2):
            if("version.ini" in args[1]): # We have a specific version.ini to work with but no build type
                build("build", args[1])
            elif(args[1] == "build" or args[1] == "patch" or args[1] == "minor" or args[1] == "major"):
                build(args[1])
        elif(numArgs == 3): # We have a specific file and build type
            if("version.ini" in args[1]):
                if(args[2] == "build" or args[2] == "patch" or args[2] == "minor" or args[2] == "major"):
                    build(args[2], args[1])
            elif("version.ini" in args[2]):
                if(args[1] == "build" or args[1] == "patch" or args[1] == "minor" or args[1] == "major"):
                    build(args[1], args[2])
    else:  
        print("No build version type specified (major, minor, patch, or build), assuming build")
        build("build")

def build(buildType, loc=None):
    # If we don't have a specific version.ini file to work with assume its in the same folder
    iniLoc = "version.ini"
    if loc != None:
        iniLoc = loc

    # Create a ConfigParser and load the supplied version.ini file
    config = configparser.ConfigParser()
    config.read(iniLoc)

    major = config["VERSION"]["major"]
    majorOpt = "-DWASMNOISE_LIB_MAJOR=" + str(major)
    minor = config["VERSION"]["minor"]
    minorOpt = "-DWASMNOISE_LIB_MINOR=" + str(minor)
    patch = config["VERSION"]["patch"]
    patchOpt = "-DWASMNOISE_LIB_PATCH=" + str(patch)
    build = config["VERSION"]["build"]
    buildOpt = "-DWASMNOISE_LIB_BUILD=" + str(build)

    # Increment the specified version part
    print("Incrementing " + buildType)
    config["VERSION"][buildType] =  str(int(config["VERSION"][buildType]) + 1)

    # Reset lower values
    if(buildType == "major"): 
        config["VERSION"]["minor"] = str(0)
        config["VERSION"]["patch"] = str(0)
        config["VERSION"]["build"] = str(0)
    elif(buildType == "minor"): 
        config["VERSION"]["patch"] = str(0)
        config["VERSION"]["build"] = str(0)
    elif(buildType == "patch"):
        config["VERSION"]["build"] = str(0)

    # Output the new value
    print("New Version: " + config["VERSION"]["major"] + '.' + config["VERSION"]["minor"] + '.' + config["VERSION"]["patch"] + '.' + config["VERSION"]["build"])
    
    # Save the modified file
    with open(iniLoc, 'w') as configfile:
        config.write(configfile)

    # Run cmake to generate an up to date make file
    cmakeCmd = ["cmake.exe", "-DCMAKE_TOOLCHAIN_FILE=C:/local/emsdk/emscripten/incoming/cmake/Modules/Platform/Emscripten.cmake", "-DCMAKE_MAKE_PROGRAM:PATH=C:/gnuwin32/bin/make.exe", majorOpt, minorOpt, patchOpt, buildOpt, "-G", "MinGW Makefiles", "."]
    subprocess.run(cmakeCmd, shell=True)
    subprocess.run(["make"], shell=True)

if __name__ == "__main__":
    main(len(sys.argv), sys.argv)
