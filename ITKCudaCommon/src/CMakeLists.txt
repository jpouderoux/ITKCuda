if (ITK_USE_CUDA)

  set(ITKCudaCommon_SRC
    itkCudaContextManager.cxx
    itkCudaDataManager.cxx
    itkCudaKernelManager.cxx
    itkCudaUtil.cxx
  )

  set(ITKCudaCommon_Kernels 
    CudaImageOps.cu
    CudaReduction.cu
  )

  generate_cuda_ptx_wrappers(ITKCudaCommon_KernelsPTX ITKCudaCommon_KernelsWrappers ${ITKCudaCommon_Kernels})

  include_directories(${CUDA_INCLUDE_DIRS}) 
  add_library(ITKCudaCommon ${ITKCudaCommon_SRC} ${ITKCudaCommon_KernelsWrappers} ${ITKCudaCommon_Kernels})
  target_link_libraries(ITKCudaCommon ${ITKCommon_LIBRARIES} ${CUDA_LIBRARIES})
  itk_module_target(ITKCudaCommon)

endif()
