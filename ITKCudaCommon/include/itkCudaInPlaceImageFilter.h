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
#ifndef __itkCudaInPlaceImageFilter_h
#define __itkCudaInPlaceImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "itkCudaImageToImageFilter.h"

namespace itk
{
/** \class CudaInPlaceImageFilter
 * \brief Base class for Cuda filters that take an image as input and overwrite that image as the output
 *
 * This class is the base class for Cuda inplace filter. The template parameter for parent class type
 * must be InPlaceImageFilter type so that the Cuda superclass of this class can be correctly defined
 * (NOTE: TParentImageFilter::Superclass is used to define CudaImageToImageFilter class).
 *
 * \ingroup ITKCudaCommon
 */
template< class TInputImage, class TOutputImage = TInputImage, class TParentImageFilter =
            InPlaceImageFilter< TInputImage, TOutputImage > >
class ITK_EXPORT CudaInPlaceImageFilter : public CudaImageToImageFilter< TInputImage, TOutputImage, TParentImageFilter >
{
public:
  /** Standard class typedefs. */
  typedef CudaInPlaceImageFilter                                                  Self;
  typedef CudaImageToImageFilter< TInputImage, TOutputImage, TParentImageFilter > GPUSuperclass;
  typedef TParentImageFilter                                                      CPUSuperclass;
  typedef SmartPointer< Self >                                                    Pointer;
  typedef SmartPointer< const Self >                                              ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(CudaInPlaceImageFilter, CudaImageToImageFilter);

  /** Superclass typedefs. */
  typedef typename CudaSuperclass::OutputImageType       OutputImageType;
  typedef typename CudaSuperclass::OutputImagePointer    OutputImagePointer;
  typedef typename CudaSuperclass::OutputImageRegionType OutputImageRegionType;
  typedef typename CudaSuperclass::OutputImagePixelType  OutputImagePixelType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** Some convenient typedefs. */
  typedef TInputImage                           InputImageType;
  typedef typename InputImageType::Pointer      InputImagePointer;
  typedef typename InputImageType::ConstPointer InputImageConstPointer;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;

protected:
  CudaInPlaceImageFilter();
  ~CudaInPlaceImageFilter();

  virtual void PrintSelf(std::ostream & os, Indent indent) const;

  /** The GenerateData method normally allocates the buffers for all
   * of the outputs of a filter. Since InPlaceImageFilter's can use an
   * overwritten version of the input for its output, the output
   * buffer should not be allocated. When possible, we graft the input
   * to the filter to the output.  If an InPlaceFilter has multiple
   * outputs, then it would need to override this method to graft one
   * of its outputs and allocate the remaining. If a filter is
   * threaded (i.e. it provides an implementation of
   * ThreadedGenerateData()), this method is called automatically. If
   * an InPlaceFilter is not threaded (i.e. it provides an
   * implementation of GenerateData()), then this method (or
   * equivalent) must be called in GenerateData(). */
  virtual void AllocateOutputs();

  /** InPlaceImageFilter may transfer ownership of the input bulk data
   * to the output object.  Once the output object owns the bulk data
   * (done in AllocateOutputs()), the input object must release its
   * hold on the bulk data.  ProcessObject::ReleaseInputs() only
   * releases the input bulk data when the user has set the
   * ReleaseDataFlag.  InPlaceImageFilter::ReleaseInputs() also
   * releases the input that it has overwritten.
   *
   * \sa ProcessObject::ReleaseInputs() */
  virtual void ReleaseInputs();

private:
  CudaInPlaceImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);        //purposely not implemented

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkCudaInPlaceImageFilter.hxx"
#endif

#endif
