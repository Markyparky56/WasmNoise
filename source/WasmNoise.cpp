#include "WasmNoise.hpp"
#include "xoroshiro128plus.hpp"
#include "uniform_int_distribution.hpp"

static WasmNoise wasmNoise;

const WM_DECIMAL GRAD_X[] =
{
  1, -1, 1, -1,
  1, -1, 1, -1,
  0,  0, 0,  0
};

const WM_DECIMAL GRAD_Y[] =
{
  1,  1, -1, -1,
  0,  0,  0,  0,
  1, -1,  1, -1
};

const WM_DECIMAL GRAD_Z[] =
{
  0, 0,  0,  0,
  1, 1, -1, -1,
  1, 1, -1, -1
};

static int32 FastFloor(WM_DECIMAL f) { return (f >= 0 ? static_cast<int32>(f) : static_cast<int32>(f) - 1); }
static int32 FastRound(WM_DECIMAL f) { return (f >= 0 ? static_cast<int32>(f + WM_DECIMAL(0.5)) : static_cast<int32>(f - WM_DECIMAL(0.5))); }
//static int32 FastAbs(int32 i) { return abs(i); }
static WM_INLINE int32 FastAbs(int32 i) { return __builtin_labs(i); }
//static WM_DECIMAL FastAbs(WM_DECIMAL f) { return fabs(f); }
static WM_INLINE WM_DECIMAL FastAbs(WM_DECIMAL f){ return __builtin_fabs(f); }
static WM_DECIMAL Lerp(WM_DECIMAL a, WM_DECIMAL b, WM_DECIMAL t) { return a + t * (b - a); }
static WM_DECIMAL InterpHermiteFunc(WM_DECIMAL t) { return t*t*(3 - 2*t); }
static WM_DECIMAL InterpQuinticFunc(WM_DECIMAL t) { return t*t*t*(t*(t*6 -15) + 10); }
static WM_DECIMAL CubicLerp(WM_DECIMAL a, WM_DECIMAL b, WM_DECIMAL c, WM_DECIMAL d, WM_DECIMAL t)
{
  WM_DECIMAL p = (d-c)-(a-b);
  return t*t*t*p + t*t*((a-b)-p) + t*(c-a) + b;
}

void WasmNoise::SetSeed(int32 _seed)
{
  seed = _seed;

  xoroshiro128plus gen(seed); 

  for(int32 i = 0; i < 256; i++)
  {
    perm[i] = i;
  }

  for(int32 j = 0; j < 256; j++)
  {
    std::uniform_int_distribution<> dist(0, 256-j);
    int k = dist(gen) + j;
    // int k = (gen.next() % 256-j) + j;
    int l = perm[j];
    perm[j] = perm[j + 256] = perm[k];
    perm[k] = l;
    perm12[j] = perm12[j+256] = perm[j] % 12;
  }
}

uint8 WasmNoise::Index2D_12(uint8 offset, int32 x, int32 y) const
{
  return perm12[(x & 0xff) + perm[(y & 0xff) + offset]];
}

WM_DECIMAL WasmNoise::GradCoord2D(uint8 offset, int32 x, int32 y, WM_DECIMAL xd, WM_DECIMAL yd) const
{
  uint8 lutPos = Index2D_12(offset, x, y);
  return xd*GRAD_X[lutPos] + yd*GRAD_Y[lutPos];
}

WM_DECIMAL WasmNoise::GetPerlin(WM_DECIMAL x, WM_DECIMAL y) const
{
  return SinglePerlin(0, x, y); // TODO: include frequency multipliers when adding fractals
}

WM_DECIMAL WasmNoise::SinglePerlin(uint8 offset, WM_DECIMAL x, WM_DECIMAL y) const
{
  int32 x0 = FastFloor(x);
  int32 y0 = FastFloor(y);
  int32 x1 = x0+1;
  int32 y1 = y0+1;

  WM_DECIMAL xs, ys;
  // TODO: switch for different interps
  xs = x - static_cast<WM_DECIMAL>(x0);
  ys = y - static_cast<WM_DECIMAL>(y0);

  WM_DECIMAL xd0 = x - static_cast<WM_DECIMAL>(x0);
  WM_DECIMAL yd0 = y - static_cast<WM_DECIMAL>(y0);
  WM_DECIMAL xd1 = xd0 - 1; // Cast?
  WM_DECIMAL yd1 = yd0 - 1;

  WM_DECIMAL xf0 = Lerp(GradCoord2D(offset, x0, y0, xd0, yd0), GradCoord2D(offset, x1, y0, xd1, yd0), xs);
  WM_DECIMAL xf1 = Lerp(GradCoord2D(offset, x0, y1, xd0, yd1), GradCoord2D(offset, x1, y1, xd1, yd1), xs);

  return Lerp(xf0, xf1, ys);
}
