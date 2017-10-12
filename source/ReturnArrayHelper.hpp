#pragma once
#include "defineconfig.hpp"

// A helper class
class ReturnArrayHelper
{
  WM_DECIMAL *array;
public:
  ReturnArrayHelper()
    : array(nullptr)
  {}
  WM_DECIMAL *NewArray(int num)
  {
    if(array != nullptr)
    {
      delete[] array;
    }
    array = new WM_DECIMAL[num];
    return array;
  }
};
