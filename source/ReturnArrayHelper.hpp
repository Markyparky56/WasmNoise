#pragma once
#include "defineconfig.hpp"

// A helper class
class ReturnArrayHelper
{
  WN_DECIMAL *array;
public:
  ReturnArrayHelper()
    : array(nullptr)
  {}
  WN_DECIMAL *NewArray(int num)
  {
    if(array != nullptr)
    {
      delete[] array;
    }
    array = new WN_DECIMAL[num];
    return array;
  }
};
