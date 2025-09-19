#ifndef CUDASIFT_H
#define CUDASIFT_H
#pragma once

// Windows DLL export/import macros
#ifdef _WIN32
  #ifdef CUDASIFT_EXPORTS
    #define CUDASIFT_API __declspec(dllexport)
  #else
    #define CUDASIFT_API __declspec(dllimport)
  #endif
#else
  #define CUDASIFT_API
#endif

typedef struct {
  float xpos;
  float ypos;   
  float scale;
  float sharpness;
  float edgeness;
  float orientation;
  float score;
  float ambiguity;
  int match;
  float match_xpos;
  float match_ypos;
  float match_error;
  float subsampling;
  float empty[3];
  float data[128];
} SiftPoint;

typedef struct {
  int numPts;         // Number of available Sift points
  int maxPts;         // Number of allocated Sift points
#ifdef MANAGEDMEM
  SiftPoint *m_data;  // Managed data
#else
  SiftPoint *h_data;  // Host (CPU) data
  SiftPoint *d_data;  // Device (GPU) data
#endif
} SiftData;

CUDASIFT_API void InitCuda(int devNum = 0);
CUDASIFT_API void InitSiftData(SiftData &data, int num = 1024, bool host = false, bool dev = true);
CUDASIFT_API void FreeSiftData(SiftData &data);
CUDASIFT_API double MatchSiftData(SiftData &data1, SiftData &data2);

#endif
