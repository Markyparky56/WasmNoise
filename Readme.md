# WasmNoise

_I like to think it's pronounced Wah-zum-Noise_

WasmNoise is an open source noise generation library based on the popular 
[FastNoise Library](https://github.com/Auburns/FastNoise). It is written in 
C++ and compiled to WebAssembly in an effort to provide faster and more 
diverse noise generation utilities in the browser than are possible with 
just JavaScript. 

It is still early in development, but I hope to get WasmNoise to a state of 
parity with FastNoise in terms of features offered. WebAssembly offers _near_ 
native speeds but there will always be a slight difference in terms of speed. 

My intention for this library is that it can be used to accelerate generation of 
noise in existing and new web sites, games and applications which consist 
predominately of JavaScript. **If you are compiling an exisiting C/C++ application 
which uses FastNoise you should be using Emscripten, as it will compile it into 
your application.** 

### Current Features
- Perlin Noise 2D, 3D

### Planned Features
- Feature parity with FastNoise
- Custom builds to remove different noise types to reduce binary size

## Using WasmNoise
WebAssembly does not have the functionality to return arrays like JavaScript does, 
it is only able to return single values from exported functions. Currently there is 
also a noticeable overhead for calling an exported WebAssembly, this makes the 
single value return functions slower than their JavaScript equivalents. 

To bypass this limitation I have implemented functions which generate noise values 
in bulk and return a pointer (offset) to the array these values are stored in, in 
linear memory. Variations of these functions allow you to produce _strips_, 
_squares_ and _cubes_ of noise values, cubes are not available for 2D noise 
functions due to the obvious limitations. These functions are comparable in speed 
to regular FastNoise, and _much_ faster than their JavaScript equivalents.

There are a couple of examples in the `html` folder which demonstrate different 
usages and outputs, however these are still just the pages I use for testing build 
iterations and not actual polished use cases.

WebAssembly binary files (.wasm) currently need to be loaded manually, as there 
is no way to load them with `<script>` tags as of yet. For this purpose I have 
written a simple autoloader script which can be found in the `html` folder, while 
not fully featured it does load the WasmNoise module and provides an optional 
`onLoaded` function which it calls once it has set up the WasmNoise object as well 
as some extra functionality such as wrappers for the functions which return arrays.
Take a look at `autoloadertest.html` for how to use the `onLoaded` function to tell 
when the module is loaded, and play around with the different functions in the 
developer console through the `WasmNoise` global variable. 

## Building
WasmNoise is developed on a Windows 7 PC, and so the build instructions below 
are geared towards a similar build environment, however the tools used are all 
(except Visual Studio) available and buildable on Mac and Linux so adjusting the 
instructions to suit your environment should be straightforward.

### If you want to compile WasmNoise for yourself you will need:
- LLVM + Clang
- [Binaryen](https://github.com/WebAssembly/binaryen)
- [Wabt](https://github.com/WebAssembly/wabt)
- [Python 3](https://www.python.org/downloads/) to run the buildwasnnoise.py script
- The latest Visual Studio (e.g. 2017 Community)
- _Optional_ ninja for quicker building 
- _Optional_ MingW64 for building LLVM + Clang

To build the above required tools you will need [CMake](https://cmake.org/download/)

If you want to build with ninja on windows (like I do) you can get it [here](https://github.com/ninja-build/ninja/releases), 
the prebuilt binary is sufficient, just make sure it resides somewhere on your Path 
so CMake can find it.

Building with ninja requires MinGW (or theoretically Clang/Clang++) on windows, so 
you can grab it from [here](https://mingw-w64.org/doku.php/download/mingw-builds), 
make sure to install the latest version and set the architecture to `x86_64`, 
leave the rest of the options the same.

### Building LLVM for WebAssembly
LLVM can be checked out with SVN from `http://llvm.org/svn/llvm-project/llvm/trunk` 

Clang can be checked out with SVN from `http://llvm.org/svn/llvm-project/cfe/trunk`

Clang should be checked out into the tools folder within the llvm svn repository. 
There are instructions on the llvm site for how to build various parts of llvm _out 
of tree_, if you prefer to checkout LLVM and Clang side-by-side, but it involves 
additional cmake flags. I recommend the process listed below.

If you don't want to use SVN you can download the source for LLVM and Clang from 
[here](http://releases.llvm.org/download.html#5.0.0). Again, clang should be 
extracted to the tools folder of the llvm source. There are also git mirrors out 
there, though these are not official and may be behind the official svn repository.

Once you have the LLVM and Clang sources, build it with CMake (I advise creating a 
new folder called _build_ next to the llvm source folder to keep things neat)

I use the following in a batch file:
```
cmake -G "Ninja" ^
-DCMAKE_BUILD_TYPE=Release ^
-DCMAKE_INSTALL_PREFIX="C:/llvm" ^
-DLLVM_TARGETS_TO_BUILD=X86 ^
-DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD=WebAssembly ^
-DLLVM_INCLUDE_TESTS=OFF ^
-DLLVM_INCLUDE_EXAMPLES=OFF ^
-DLLVM_BUILD_TESTS=OFF ^
-DLLVM_BUILD_TOOLS=ON ^
-DCMAKE_CXX_FLAGS=-std=c++11 ^
-DBUILD_SHARED_LIBS=OFF ^
C:/llvm/source
```
The `C:/llvm/source` path should point to the folder containing CmakeLists.txt for 
LLVM

To build LLVM and Clang with Visual Studio 2017 you can use this cmake command:
```
cmake -G "Visual Studio 15 2017 Win64" ^
-DCMAKE_BUILD_TYPE=Release ^
-DCMAKE_INSTALL_PREFIX="C:/llvm" ^
-DLLVM_TARGETS_TO_BUILD=X86 ^
-DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD=WebAssembly ^
-DLLVM_INCLUDE_TESTS=OFF ^
-DLLVM_INCLUDE_EXAMPLES=OFF ^
-DLLVM_BUILD_TESTS=OFF ^
-DLLVM_BUILD_TOOLS=ON ^
-DBUILD_SHARED_LIBS=OFF ^
C:/llvm/source
```

This also includes the X86 target which will let clang & clang++ produce 
executables, this is not a requirement and can be removed, although clang isn't the 
most reliable on Windows it is handy to be able to check if the code you're writing 
for WebAssembly is valid for native executables and it's quicker than creating a VS 
project.

If it compiles correctly make sure to add the *bin* folder in your build directory 
to your Path variable.

### Getting & Building Binaryen
The best way to build Binaryen is with Visual Studio, and is relatively painless.

The following command should handle using git to checkout binaryen and building it 
with MSBuild
```
git clone http://github.com/WebAssembly/binaryen.git && cd binaryen && mkdir build && cd build && cmake -G "Visual Studio 15 2017 Win64" .. && cmake --build .
```

You can also use Visual Studio to build with the solution file produced by cmake if 
you prefer, just omit the `&& cmake --build .` from the above command.

Once it's finished compiling, remember to add the *bin* folder to your Path 
variable like before.

### Getting & Building Wabt
Again, Wabt is best built with Visual Studio.

The following command should handle checking out wabt and compiling it
```
git clone http://github.com/WebAssembly/wabt.git && cd wabt && mkdir build && cd build && cmake -G "Visual Studio 15 2017 win64" .. && cmake --build .
```

Add the *bin* folder to your Path and you should have all the necessary tools.

### Building WasmNoise 
If you have all the correct bin directories added to your path, cd into the root 
wasmnoise directory and run `buildwasmnoise.py`. This requires Python 3, and if you 
have both python 2 and python 3 installed your system may try running the build 
script with python 2, which will of course fail. As a work around on my system I 
have renamed the python 3 executable (located at 
`C:\Users\<Me>\AppData\Local\Programs\Python\Python36-32`) to python3, and so 
execute the build script with the command `python3 buildwasmnoise.py`.

This will, on a first build, create a bin directory and within it a folder with the name in the format of `wasmnoise-major-minor-patch-build` (e.g. `wasmnoise-0.0.0.b158`). Within this folder will be a number of build files:

```
.bc Files:
WasmNoise.bc - bitcode produced by clang++ when compiling WasmNoise.cpp
WasmNoiseInterface.bc - bitcode produced by clang++ when compiling WasmNoiseInterface.cpp
wasmnoise-0.0.0.bc - bitcode produced by llvm-link when linking WasmNoise.bc, 
                     WasmNoiceInterface.bc and memory.bc (located at 
                     source/memory-bitcode)

.s file:
wasmnoise-0.0.0.s - produced by llc from wasmnoise-0.0.0.bc

.wat Files:
wasmnoise-0.0.0.wat - produced by s2wasm from wasmnoise-0.0.0.s
wasmnoise-0.0.0.cleanexports.wat - processed version of wasmnoise-0.0.0.wat 
                                   produced as part of a build step using 
                                   functions in removeextraexports.py

.wasm files:
wasmnoise-0.0.0.wasm - compiled wasm file produced by wat2wasm from 
                       wasmnoise-0.0.0.cleanexports.wat
wasmnoise-0.0.0.opt.wasm - optimised wasm file produced by wasm-opt from 
                           wasmnoise-0.0.0.wasm, removing unused functions
```


