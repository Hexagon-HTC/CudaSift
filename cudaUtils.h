#ifndef CUDAUTILS_H
#define CUDAUTILS_H
#pragma once

#include <cstdio>
#include <iostream>

#define safeCall(err)       __safeCall(err, __FILE__, __LINE__)
#define safeThreadSync()    __safeThreadSync(__FILE__, __LINE__)
#define checkMsg(msg)       __checkMsg(msg, __FILE__, __LINE__)

inline void __safeCall(cudaError err, const char *file, const int line)
{
  if (cudaSuccess != err) {
    fprintf(stderr, "safeCall() Runtime API error in file <%s>, line %i : %s.\n", file, line, cudaGetErrorString(err));
    exit(-1);
  }
}

inline bool deviceInit(int dev)
{
  int deviceCount;
  safeCall(cudaGetDeviceCount(&deviceCount));
  if (deviceCount == 0) {
    fprintf(stderr, "CUDA error: no devices supporting CUDA.\n");
    return false;
  }
  if (dev < 0) dev = 0;						
  if (dev > deviceCount-1) dev = deviceCount - 1;
  cudaDeviceProp deviceProp;
  safeCall(cudaGetDeviceProperties(&deviceProp, dev));
  if (deviceProp.major < 1) {
    fprintf(stderr, "error: device does not support CUDA.\n");
    return false;					
  }
  safeCall(cudaSetDevice(dev));
  return true;
}

#endif // CUDAUTILS_H