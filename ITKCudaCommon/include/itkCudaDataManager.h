/*=========================================================================
*
*  Copyright Insight Software Consortium
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*         http://www.apache.org/licenses/LICENSE-2.0.txt
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
*=========================================================================*/
#ifndef __itkCudaDataManager_h
#define __itkCudaDataManager_h

#include "itkObject.h"
#include "itkDataObject.h"
#include "itkObjectFactory.h"
#include "itkCudaUtil.h"
#include "itkCudaContextManager.h"
#include "itkSimpleFastMutexLock.h"
#include "itkMutexLockHolder.h"

#include <memory>

namespace itk
{
class GPUMemPointer;

/** \class CudaDataManager
 * \brief GPU memory manager implemented using Cuda. Required by CudaImage class.
 *
 * This class serves as a base class for Cuda data container for CudaImage class,
 * which is similar to ImageBase class for Image class. However, all the image-related
 * meta data will be already stored in image class (parent of CudaImage), therefore
 * we did not name it CudaImageBase. Rather, this class is a Cuda-specific data manager
 * that provides functionalities for RAM-GRAM data synchronization and grafting Cuda data.
 *
 * \ingroup ITKCudaCommon
 */
class ITK_EXPORT CudaDataManager : public Object   //DataObject//
{
  /** allow CudaKernelManager to access Cuda buffer pointer */
  friend class CudaKernelManager;

public:

  typedef CudaDataManager          Self;
  typedef Object                   Superclass;
  typedef SmartPointer<Self>       Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);
  itkTypeMacro(CudaDataManager, Object);

  typedef MutexLockHolder<SimpleFastMutexLock> MutexHolderType;

  /** total buffer size in bytes */
  void SetBufferSize(unsigned int num);

  unsigned int GetBufferSize() {
    return m_BufferSize;
  }

  void SetBufferFlag(int flags);

  void SetCPUBufferPointer(void* ptr);

  void SetCPUDirtyFlag(bool isDirty);

  void SetGPUDirtyFlag(bool isDirty);

  /** Make GPU up-to-date and mark CPU as dirty.
   * Call this function when you want to modify CPU data */
  void SetCPUBufferDirty();

  /** Make CPU up-to-date and mark GPU as dirty.
   * Call this function when you want to modify Cuda data */
  void SetGPUBufferDirty();

  bool IsCPUBufferDirty() {
    return m_IsCPUBufferDirty;
  }

  bool IsGPUBufferDirty() {
    return m_IsGPUBufferDirty;
  }

  /** actual Cuda->CPU memory copy takes place here */
  virtual void UpdateCPUBuffer();

  /** actual CPU->Cuda memory copy takes place here */
  virtual void UpdateGPUBuffer();

  void Allocate();
  
  /** Synchronize CPU and Cuda buffers (using dirty flags) */
  bool Update();

  /** Method for grafting the content of one CudaDataManager into another one */
  virtual void Graft(const CudaDataManager* data);

  /** Initialize CudaDataManager */
  virtual void Initialize();

  /** Get Cuda buffer pointer */
  void* GetGPUBufferPointer();

  /** Get Cuda buffer pointer */
  void* GetCPUBufferPointer();

protected:

  CudaDataManager();
  virtual ~CudaDataManager();
  virtual void PrintSelf(std::ostream & os, Indent indent) const;

private:

  CudaDataManager(const Self&);   //purposely not implemented
  void operator=(const Self&);

protected:

  unsigned int m_BufferSize;   // # of bytes

  CudaContextManager *m_ContextManager;

  /** buffer type */
  int m_MemFlags;

  /** buffer pointers */
  std::tr1::shared_ptr<GPUMemPointer> m_GPUBuffer;
  //void* m_GPUBuffer;
  void* m_CPUBuffer;

  /** checks if buffer needs to be updated */
  bool m_IsGPUBufferDirty;
  bool m_IsCPUBufferDirty;

  /** Mutex lock to prevent r/w hazard for multithreaded code */
  SimpleFastMutexLock m_Mutex;
};

class GPUMemPointer
{
public:
  GPUMemPointer() : m_GPUBuffer(0), m_bufferSize(0) 
  {
  }
    
  void Allocate(size_t bufferSize)
  {
    m_bufferSize = bufferSize;
    CUDA_CHECK(cudaMalloc(&m_GPUBuffer, bufferSize));
  }

  ~GPUMemPointer()
  {
    if(m_GPUBuffer) 
      {
      CUDA_CHECK(cudaFree(m_GPUBuffer));
      }
  }

  void* GetPointer()
  {
    return m_GPUBuffer;
  }

  void* GetPointerPtr()
  {
    return &m_GPUBuffer;
  }

protected:
  void* m_GPUBuffer;
  size_t m_bufferSize;
};

} // namespace itk

#endif
