"""
Moudle to output an autoloader js file tailored to the build
"""
from string import Template

WasmNoiseObjectDeclaration = "if(WasmNoise != undefined) {let WasmNoise = {};}"
WasmNoiseMemoryDeclaration = "WasmNoise.memory = WasmNoise.memory || WebAssembly.Memory({initial: 9});"
WasmNoiseEnums = {
  "Interp": "WasmNoise.Interp = WasmNoise.Interp || Object.freeze({Linear: 0, Hermite: 1, Quintic: 2});",
  "FractalType":"WasmNoise.FractalType = WasmNoise.FractalType || Object.freeze({FBM: 0, Billow: 1, RidgedMulti: 2});",
  "StripDirection":"WasmNoise.StripDirection = WasmNoise.StripDirection || Object.freeze({XAxis: 0, YAxis: 1, ZAxis: 2, WAxis: 3});",
  "SquarePlane":"WasmNoise.SquarePlane = WasmNoise.SquarePlane || Object.freeze({XYPlane: 0, XZPlane: 1, ZYPlane: 2});"
}

WasmNoiseOnLoadedDef = "WasmNoise.onLoaded = WasmNoise.onLoaded || null;"
WasmNoiseCallCompile = "WasmNoise.fetchCompileAndInstantiate();"

def constructValuesFunc(funcName, funcType):
  """Constructs an ease-of-use function for calling and returning values from WasmNoise"""
  funcTemplate = Template(
    "this.${func}_${funcType}_Values = function(${startParams}, ${otherParams})"+
    "{" +
    "let offset = this.${func}_${funcType}(${startParams}, ${otherParams});" +
    "return new Float32Array(this.memory.buffer.slice(offset, offset+${size}));" +
    "}")
  startParams = "startX, startY" if funcName[-1] is '2' else ("startX, startY, startZ" if (funcName[-1] is '3') else "startX, startY, startZ, startW")
  otherParams = str()
  size = str()
  if funcName[-1] is '2':
    otherParams = "length, direction" if funcType is "Strip" else "width, height"
    size = "length*4" if funcType is "Strip" else "width*height*4"
  else:
    if funcType is "Strip":
      otherParams = "length, direction"
      size = "length*4"
    elif funcType is "Square":
      otherParams = "width, height, plane"
      size = "width*height*4"
    elif funcType is "Cube":
      otherParams = "width, height, depth"
      size = "width*height*depth*4"
  return funcTemplate.substitute(func=funcName, funcType=funcType, startParams=startParams, otherParams=otherParams, size=size)

def constructFunctionElevation(funcName):
  """Constructs a string for elevating a function from exports to top-level"""
  template = Template(
    "this.${funcName} = this.instance.exports.${funcName};"
  )
  return template.substitute(funcName=funcName)

def constructFetchCompileAndInstantiateFunction(filename, enabledFunctions, exports):
  """
  Fills out a template for the functions which fetches,
  compiles and instantiates the WasmNoise Object
  """
  funcTemplate = Template(
    "WasmNoise.fetchCompileAndInstantiate = WasmNoise.fetchCompileAndInstantiate || function()" +
    "{" +
    "return fetch('./${filename}')" +
    ".then(res => {" +
    "if(res.ok)" +
    "{" +
    "return res.arrayBuffer();" +
    "}" +
    "else" +
    "{" +
    "throw new Error('Unable to fetch WasmNoise!');" +
    "}" +
    "})" +
    ".then(bytes => WebAssembly.compile(bytes))" +
    ".then(wasmnoiseModule => {" +
    "return WebAssembly.instantiate(wasmnoiseModule, {" +
    "env: {" +
    "__errno_location:function() { return 8; }," +
    "abort: function() { throw new Error('Abort called!'); }," +
    "sbrk: function(len) { return (WasmNoise.memory.grow(len >> 16) << 16); }," +
    "memory: this.memory" +
    "}" +
    "})" +
    "})" +
    ".then(instance => {\n" +
    "this.instance = instance;\n" +
    "this.instance.exports['_GLOBAL__sub_I_WasmNoiseInterface.cpp']();\n"
    "${functionElevations}" +
    "${valueFunctions}" +
    "this.GetValues = function(offset, elements)" +
    "{" +
    "return new Float32Array(this.memory.buffer.slice(offset, offset+(elements*4)));" +
    "}\n" +
    "if(this.onLoaded) this.onLoaded();" +
    "});"
    "}"
  )
  # Get a list of all the functions we need to elevate, remove the _GLOBAL__ function from the list
  # as we don't need to expose that
  elevateFunctionList = []
  for enabledFunctionSet in enabledFunctions:
    elevateFunctionList += exports["exports"][enabledFunctionSet]["funcs"]
  elevateFunctionList.remove("_GLOBAL__sub_I_WasmNoiseInterface.cpp")

  # Construct function elevations
  functionElevationsStr = str()
  for function in elevateFunctionList:
    functionElevationsStr += constructFunctionElevation(function) + "\n"

  # We only need a subset of the elevate function list for our _Values functions
  # We can picked them out by the prescense of their dimension in their function name
  valueFunctionList = []  
  for elevatedFunction in elevateFunctionList:
    if elevatedFunction[-1] is '2' or elevatedFunction[-1] is '3' or elevatedFunction[-1] is '4':
      valueFunctionList.append(elevatedFunction)

  # Construct _Values functions
  valueFunctionsStr = str()
  for function in valueFunctionList:
    if '2' in function:
      valueFunctionsStr += constructValuesFunc(function, "Strip") + "\n"
      valueFunctionsStr += constructValuesFunc(function, "Square") + "\n"
    else: # We can create a cube function
      valueFunctionsStr += constructValuesFunc(function, "Strip") + "\n"
      valueFunctionsStr += constructValuesFunc(function, "Square") + "\n"
      valueFunctionsStr += constructValuesFunc(function, "Cube") + "\n"

  params = {
    "filename": filename,
    "functionElevations": functionElevationsStr,
    "valueFunctions": valueFunctionsStr
  }
  return funcTemplate.substitute(params)

def outputAutloaderFile(fileName, enabledFunctions, exports):
  """
  Constructs and writes a js file based on the given filename and the enabled
  function sets
  """
  with open("wasmnoise.autoloader.js", "w") as file:
    file.write(WasmNoiseObjectDeclaration + "\n")
    file.write(WasmNoiseMemoryDeclaration + "\n")
    file.write(WasmNoiseEnums["Interp"] + "\n")
    if "fractalGetSet" in enabledFunctions:
      file.write(WasmNoiseEnums["FractalType"] + "\n")
    file.write(WasmNoiseEnums["StripDirection"] + "\n")
    file.write(WasmNoiseEnums["SquarePlane"] + "\n")
    file.write(constructFetchCompileAndInstantiateFunction(fileName, enabledFunctions, exports) + "\n")
    file.write(WasmNoiseOnLoadedDef + "\n")
    file.write(WasmNoiseCallCompile + "\n")
