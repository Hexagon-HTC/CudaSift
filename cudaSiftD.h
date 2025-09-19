//********************************************************//
// CUDA SIFT extractor by Marten Bjorkman aka Celebrandil //
//********************************************************//  

#ifndef CUDASIFTD_H
#define CUDASIFTD_H

#include <cuda_runtime.h>
#include "cudaSift.h"

#define NUM_SCALES      5

// Scale down thread block width
#define SCALEDOWN_W    64 // 60 

// Scale down thread block height
#define SCALEDOWN_H    16 // 8

// Scale up thread block width
#define SCALEUP_W      64

// Scale up thread block height
#define SCALEUP_H       8

// Find point thread block width
#define MINMAX_W       30 //32 

// Find point thread block height
#define MINMAX_H        8 //16 
 
// Laplace thread block width
#define LAPLACE_W     128 // 56

// Laplace rows per thread
#define LAPLACE_H       4

// Number of laplace scales
#define LAPLACE_S   (NUM_SCALES+3)

// Laplace filter kernel radius
#define LAPLACE_R       4

#define LOWPASS_W      24 //56
#define LOWPASS_H      32 //16
#define LOWPASS_R       4

//====================== Number of threads ====================//
// ScaleDown:               SCALEDOWN_W + 4
// LaplaceMulti:            (LAPLACE_W+2*LAPLACE_R)*LAPLACE_S
// FindPointsMulti:         MINMAX_W + 2
// ComputeOrientations:     128
// ExtractSiftDescriptors:  256

//====================== Number of blocks ====================//
// ScaleDown:               (width/SCALEDOWN_W) * (height/SCALEDOWN_H)
// LaplceMulti:             (width+2*LAPLACE_R)/LAPLACE_W * height
// FindPointsMulti:         (width/MINMAX_W)*NUM_SCALES * (height/MINMAX_H)
// ComputeOrientations:     numpts
// ExtractSiftDescriptors:  numpts

//====================== Function declarations =================//

// Constant memory symbols
extern __constant__ int d_MaxNumPoints;
extern __device__ unsigned int d_PointCounter[8*2+1];
extern __constant__ float d_ScaleDownKernel[5];
extern __constant__ float d_LowPassKernel[2*LOWPASS_R+1];
extern __constant__ float d_LaplaceKernel[8*12*16];

// Kernel function declarations
__global__ void ScaleDownDenseShift(float *d_Result, float *d_Data, int width, int pitch, int height, int newpitch);
__global__ void ScaleDown(float *d_Result, float *d_Data, int width, int pitch, int height, int newpitch);
__global__ void ScaleUp(float *d_Result, float *d_Data, int width, int pitch, int height, int newpitch);
__global__ void ComputeOrientationsCONST(cudaTextureObject_t texObj, SiftPoint *d_Sift, int octave);
__global__ void ComputeOrientationsCONSTNew(float *image, int w, int p, int h, SiftPoint *d_Sift, int octave);
__global__ void ExtractSiftDescriptorsCONST(cudaTextureObject_t texObj, SiftPoint *d_sift, float subsampling, int octave);
__global__ void ExtractSiftDescriptorsCONSTNew(cudaTextureObject_t texObj, SiftPoint *d_sift, float subsampling, int octave);
__global__ void OrientAndExtractCONST(cudaTextureObject_t texObj, SiftPoint *d_Sift, float subsampling, int octave);
__global__ void RescalePositions(SiftPoint *d_sift, int numPts, float scale);
__global__ void LowPassBlock(float *d_Image, float *d_Result, int width, int pitch, int height);
__global__ void LowPass(float *d_Image, float *d_Result, int width, int pitch, int height);
__global__ void LaplaceMultiMem(float *d_Image, float *d_Result, int width, int pitch, int height, int octave);
__global__ void LaplaceMultiMemTest(float *d_Image, float *d_Result, int width, int pitch, int height, int octave);
__global__ void LaplaceMultiMemOld(float *d_Image, float *d_Result, int width, int pitch, int height, int octave);
__global__ void LaplaceMultiTex(cudaTextureObject_t texObj, float *d_Result, int width, int pitch, int height, int octave);
__global__ void FindPointsMultiTest(float *d_Data0, SiftPoint *d_Sift, int width, int pitch, int height, float subsampling, float lowestScale, float thresh, float factor, float edgeLimit, int octave);
__global__ void FindPointsMulti(float *d_Data0, SiftPoint *d_Sift, int width, int pitch, int height, float subsampling, float lowestScale, float thresh, float factor, float edgeLimit, int octave);
__global__ void FindPointsMultiNew(float *d_Data0, SiftPoint *d_Sift, int width, int pitch, int height, float subsampling, float lowestScale, float thresh, float factor, float edgeLimit, int octave);

#endif
