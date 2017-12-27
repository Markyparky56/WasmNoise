if(WasmNoise != undefined) {let WasmNoise = {};}
WasmNoise.memory = WasmNoise.memory || WebAssembly.Memory({initial: 9});
// TODO: Rest of the enums
WasmNoise.Interp = WasmNoise.Interp || Object.freeze({
  Linear: 0,
  Hermite: 1,
  Quintic: 2,
});
WasmNoise.FractalType = WasmNoise.FractalType || Object.freeze({
  FBM: 0,
  Billow: 1,
  RigidMulti: 2
});
WasmNoise.StripDirection = WasmNoise.StripDirection || Object.freeze({
  XAxis: 0,
  YAxis: 1,
  ZAxis: 2,
  WAxis: 3
});
WasmNoise.SquarePlane = WasmNoise.SquarePlane || Object.freeze({
  XYPlane: 0,
  XZPlane: 1,
  ZYPlane: 2
});
WasmNoise.fetchCompileAndInstantiate = WasmNoise.fetchCompileAndInstantiate || function()
{
  // TODO: Find a way to keep the version number here accurate, I'm thinking a template 
  // which is generated/filled out for each patch/minor/major build? Or just every build
  return fetch("./wasmnoise-0.1.0.opt.wasm")
  .then(res => {
    if(res.ok)
    {
      return res.arrayBuffer();
    }
    else
    {
      throw new Error("Unable to fetch WasmNoise!");
    }
  })
  .then(bytes => WebAssembly.compile(bytes))
  .then(wasmnoiseModule => {    
    return WebAssembly.instantiate(wasmnoiseModule, {
      env: {
        __errno_location: function() { return 8; },
        abort: function() { throw new Error("Abort called!"); },
        sbrk: function(len) { return (WasmNoise.memory.grow(len >> 16) << 16); },
        memory: this.memory
      }
    })
  })
  .then(instance => {
    this.instance = instance;
    // Call startup function
    this.instance.exports["_GLOBAL__sub_I_WasmNoiseInterface.cpp"]();
    // TODO: These will need to be kept up to date as I add new functions
    // Elevate exported functions from instance to top-level WasmNoise object
    this.SetSeed = this.instance.exports.SetSeed;
    this.GetSeed = this.instance.exports.GetSeed;
    this.SetFrequency = this.instance.exports.SetFrequency;
    this.GetFrequency = this.instance.exports.GetFrequency;
    this.SetInterp = this.instance.exports.SetInterp;
    this.GetInterp = this.instance.exports.GetInterp;
    this.SetFractalOctaves = this.instance.exports.SetFractalOctaves;
    this.GetFractalOctaves = this.instance.exports.GetFractalOctaves;
    this.SetFractalLacunarity = this.instance.exports.SetFractalLacunarity;
    this.GetFractalLacunarity = this.instance.exports.GetFractalLacunarity;
    this.SetFractalGain = this.instance.exports.SetFractalGain;
    this.GetFractalGain = this.instance.exports.GetFractalGain;
    this.SetFractalType = this.instance.exports.SetFractalType;
    this.GetFractalType = this.instance.exports.GetFractalType;
    this.GetPerlin2 = this.instance.exports.GetPerlin2;
    this.GetPerlin2_Strip = this.instance.exports.GetPerlin2_Strip;
    this.GetPerlin2_Square = this.instance.exports.GetPerlin2_Square;
    this.GetPerlinFractal2 = this.instance.exports.GetPerlinFractal2;
    this.GetPerlinFractal2_Strip = this.instance.exports.GetPerlinFractal2_Strip;
    this.GetPerlinFractal2_Square = this.instance.exports.GetPerlinFractal2_Square;
    this.GetPerlin3 = this.instance.exports.GetPerlin2;
    this.GetPerlin3_Strip = this.instance.exports.GetPerlin3_Strip;
    this.GetPerlin3_Square = this.instance.exports.GetPerlin3_Square;
    this.GetPerlin3_Cube = this.instance.exports.GetPerlin3_Cube;
    this.GetPerlinFractal3 = this.instance.exports.GetPerlinFractal3;
    this.GetPerlinFractal3_Strip = this.instance.exports.GetPerlinFractal3_Strip;
    this.GetPerlinFractal3_Square = this.instance.exports.GetPerlinFractal3_Square;
    this.GetPerlinFractal3_Cube = this.instance.exports.GetPerlinFractal3_Cube;

    // Auto fetchers for the array functions
    this.GetPerlin2_Strip_Values = function(startX, startY, length, direction)
    {
      let offset = this.GetPerlin2_Strip(startX, startY, length, direction);
      return new Float32Array(this.memory.buffer.slice(offset, offset+(length*4)));
    }

    this.GetPerlin2_Square_Values = function(startX, startY, width, height)
    {
      let offset = this.GetPerlin2_Square(startX, startY, width, height);
      return new Float32Array(this.memory.buffer.slice(offset, offset+(width*height*4)));
    }

    this.GetPerlinFractal2_Strip_Values = function(startX, startY, length, direction)
    {
      let offset = this.GetPerlinFractal2_Strip(startX, startY, length, direction);
      return new Float32Array(this.memory.buffer.slice(offset, offset+(length*4)));
    }

    this.GetPerlinFractal2_Square_Values = function(startX, startY, width, height)
    {
      let offset = this.GetPerlinFractal2_Square(startX, startY, width, height);
      return new Float32Array(this.memory.buffer.slice(offset, offset+(width*height*4)));
    }

    this.GetPerlin3_Strip_Values = function(startX, startY, startZ, length, direction)
    {
      let offset = this.GetPerlin3_Strip(startX, startY, startZ, length, direction);
      return new Float32Array(this.memory.buffer.slice(offset, offset+(length*4)));
    }

    this.GetPerlin3_Square_Values = function(startX, startY, startZ, width, height)
    {
      let offset = this.GetPerlin3_Square(startX, startY, startZ, width, height);
      return new Float32Array(this.memory.buffer.slice(offset, offset+(width*height*4)));
    }

    this.GetPerlin3_Cube_Values = function(startX, startY, startZ, width, height, depth)
    {
      let offset = this.GetPerlin3_Cube(startX, startY, startZ, width, height, depth);
      return new Float32Array(this.memory.buffer.slice(offset, offset+(width*height*depth*4)));
    }

    this.GetPerlinFractal3_Strip_Values = function(startX, startY, startZ, length, direction)
    {
      let offset = this.GetPerlinFractal3_Strip(startX, startY, startZ, length, direction);
      return new Float32Array(this.memory.buffer.slice(offset, offset+(length*4)));
    }

    this.GetPerlinFractal3_Square_Values = function(startX, startY, startZ, width, height, plane)
    {
      let offset = this.GetPerlinFractal3_Strip(startX, startY, startZ, width, height, plane);
      return new Float32Array(this.memory.buffer.slice(offset, offset+(width*height*4)));
    }

    this.GetPerlinFractal3_Cube_Values = function(startX, startY, startZ, width, height, depth)
    {
      let offset = this.GetPerlinFractal3_Strip(startX, startY, startZ, width, height, depth);
      return new Float32Array(this.memory.buffer.slice(offset, offset+(width*height*depth*4)));
    }

    // Generic
    this.GetValues = function(offset, elements)
    {
      return new Float32Array(this.memory.buffer.slice(offset, offset+(elements*4)));
    }

    // Call the onLoaded function if there is one
    if(this.onLoaded) this.onLoaded();
  });
}
WasmNoise.onLoaded = WasmNoise.onLoaded || null;
WasmNoise.fetchCompileAndInstantiate();