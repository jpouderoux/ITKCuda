project(ITKCudaCommon)

option(ITK_USE_CUDA "GPU acceleration via CUDA" OFF)

if (ITK_USE_CUDA)

  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

  set(ITK_CUDA_PATH ${CMAKE_CURRENT_SOURCE_DIR})

  include(itkCUDA)

  set(ITKCudaCommon_LIBRARIES ITKCudaCommon)
  set(ITKCudaCommon_SYSTEM_INCLUDE_DIRS ${CUDA_INCLUDE_DIRS})
  set(ITKCudaCommon_SYSTEM_LIBRARY_DIRS ${CUDA_LIBRARIES})

  itk_module_impl()

endif ()
