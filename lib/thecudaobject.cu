#include <cuda_runtime.h>
#include "thrust/device_vector.h"
#include <vector>
#include "cudares_ff_impl.h"

__global__ void FIR_simpleFilterKernel(int nsamples,float* c_samples,
    float* cf_samples,float *c_taps,int ntaps)
{
    // Get our global thread ID
    int id = blockIdx.x*blockDim.x+threadIdx.x;

    if(id < nsamples)
    {
        for(int t_index = 0;(t_index<ntaps)&&(id>=t_index);t_index++)
        {
            cf_samples[id] = cf_samples[id]+c_taps[t_index] * c_samples[id-t_index];
        }
    }
};


//void cu_FIR_Filter(int nsamples, int ntaps, float samples_h[], float f_samples_h[], float taps_h[])
void cu_FIR_Filter(std::vector<float> samples,int ntaps,std::vector<float> &f_samples, std::vector<float> taps)
{
    int deviceId,numberOfSMs;
    int threads_per_block,number_of_blocks;
    thrust::device_vector<float> c_samples(samples.begin(),samples.end());
    thrust::device_vector<float> cf_samples(f_samples.begin(),f_samples.end());
    thrust::device_vector<float> c_taps(taps.begin(),taps.end());
    float* cd_samples = thrust::raw_pointer_cast(&c_samples[0]);
    float* cd_taps = thrust::raw_pointer_cast(&c_taps[0]);
    float* cdf_samples = thrust::raw_pointer_cast(&cf_samples[0]);
    cudaGetDevice(&deviceId);
    cudaDeviceGetAttribute(&numberOfSMs,cudaDevAttrMultiProcessorCount,deviceId);
    threads_per_block = 256;
    number_of_blocks = numberOfSMs*32;

    FIR_simpleFilterKernel<<<number_of_blocks,threads_per_block>>>(c_samples.size(),cd_samples,cdf_samples,cd_taps,ntaps);
    cudaDeviceSynchronize();
    f_samples.clear();
    thrust::copy(cf_samples.begin(),cf_samples.end(),f_samples.begin());
    
};