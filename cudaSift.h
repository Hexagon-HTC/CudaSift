#ifndef CUDASIFT_H
#define CUDASIFT_H

#include "cudaImage.h"

// Define export macros for different platforms
#ifdef _WIN32
    #ifdef CUDASIFT_BUILD
        #define CUDASIFT_API __declspec(dllexport)  // Export symbols when building the library
    #else
        #define CUDASIFT_API __declspec(dllimport)  // Import symbols when using the library
    #endif
#else
    #define CUDASIFT_API  // On Linux/macOS, this can be empty or use __attribute__((visibility("default")))
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
CUDASIFT_API float *AllocSiftTempMemory(int width, int height, int numOctaves, bool scaleUp = false);
CUDASIFT_API void FreeSiftTempMemory(float *memoryTmp);
CUDASIFT_API void ExtractSift(SiftData &siftData, CudaImage &img, int numOctaves, double initBlur, float thresh, float lowestScale = 0.0f, bool scaleUp = false, float *tempMemory = 0);
CUDASIFT_API void InitSiftData(SiftData &data, int num = 1024, bool host = false, bool dev = true);
CUDASIFT_API void FreeSiftData(SiftData &data);
CUDASIFT_API void PrintSiftData(SiftData &data);
CUDASIFT_API double MatchSiftData(SiftData &data1, SiftData &data2);
CUDASIFT_API double FindHomography(SiftData &data,  float *homography, int *numMatches, int numLoops = 1000, float minScore = 0.85f, float maxAmbiguity = 0.95f, float thresh = 5.0f);

#endif
