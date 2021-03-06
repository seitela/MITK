﻿/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

#include <itkImageFileWriter.h>
#include <itkMetaDataObject.h>
#include <itkVectorImage.h>
#include <mitkImageCast.h>

#include <mitkBaseData.h>
#include <mitkFiberBundle.h>
#include "mitkCommandLineParser.h"
#include <boost/lexical_cast.hpp>
#include <mitkCoreObjectFactory.h>
#include <mitkIOUtil.h>
#include <itkTractDensityImageFilter.h>
#include <itkTractsToFiberEndingsImageFilter.h>


mitk::FiberBundle::Pointer LoadFib(std::string filename)
{
  std::vector<mitk::BaseData::Pointer> fibInfile = mitk::IOUtil::Load(filename);
  if( fibInfile.empty() )
    std::cout << "File " << filename << " could not be read!";
  mitk::BaseData::Pointer baseData = fibInfile.at(0);
  return dynamic_cast<mitk::FiberBundle*>(baseData.GetPointer());
}

/*!
\brief Modify input tractogram: fiber resampling, compression, pruning and transformation.
*/
int main(int argc, char* argv[])
{
  mitkCommandLineParser parser;

  parser.setTitle("Tract Density");
  parser.setCategory("Fiber Tracking and Processing Methods");
  parser.setDescription("Generate tract density image, fiber envelope or fiber endpoints image.");
  parser.setContributor("MIC");

  parser.setArgumentPrefix("--", "-");
  parser.addArgument("input", "i", mitkCommandLineParser::String, "Input:", "input fiber bundle (.fib)", us::Any(), false);
  parser.addArgument("output", "o", mitkCommandLineParser::String, "Output:", "output image", us::Any(), false);
  parser.addArgument("binary", "", mitkCommandLineParser::Bool, "Binary output:", "calculate binary tract envelope", us::Any());
  parser.addArgument("normalize", "", mitkCommandLineParser::Bool, "Normalized output:", "normalize output to 0-1", us::Any());
  parser.addArgument("endpoints", "", mitkCommandLineParser::Bool, "Output endpoints image:", "calculate image of fiber endpoints instead of mask", us::Any());
  parser.addArgument("reference_image", "", mitkCommandLineParser::String, "Reference image:", "output image will have geometry of this reference image", us::Any());


  std::map<std::string, us::Any> parsedArgs = parser.parseArguments(argc, argv);
  if (parsedArgs.size()==0)
    return EXIT_FAILURE;

  bool binary = false;
  if (parsedArgs.count("binary"))
    binary = us::any_cast<bool>(parsedArgs["binary"]);

  bool endpoints = false;
  if (parsedArgs.count("endpoints"))
    endpoints = us::any_cast<bool>(parsedArgs["endpoints"]);

  bool normalize = false;
  if (parsedArgs.count("normalize"))
    normalize = us::any_cast<bool>(parsedArgs["normalize"]);

  std::string reference_image = "";
  if (parsedArgs.count("reference_image"))
    reference_image = us::any_cast<std::string>(parsedArgs["reference_image"]);

  std::string inFileName = us::any_cast<std::string>(parsedArgs["input"]);
  std::string outFileName = us::any_cast<std::string>(parsedArgs["output"]);

  try
  {
    mitk::FiberBundle::Pointer fib = LoadFib(inFileName);

    mitk::Image::Pointer ref_img;
    MITK_INFO << reference_image;
    if (!reference_image.empty())
      ref_img = dynamic_cast<mitk::Image*>(mitk::IOUtil::Load(reference_image)[0].GetPointer());

    if (endpoints)
    {
      typedef unsigned int OutPixType;
      typedef itk::Image<OutPixType, 3> OutImageType;

      typedef itk::TractsToFiberEndingsImageFilter< OutImageType > ImageGeneratorType;
      ImageGeneratorType::Pointer generator = ImageGeneratorType::New();
      generator->SetFiberBundle(fib);

      if (ref_img.IsNotNull())
      {
        OutImageType::Pointer itkImage = OutImageType::New();
        CastToItkImage(ref_img, itkImage);
        generator->SetInputImage(itkImage);
        generator->SetUseImageGeometry(true);

      }
      generator->Update();

      // get output image
      typedef itk::Image<OutPixType,3> OutType;
      OutType::Pointer outImg = generator->GetOutput();
      mitk::Image::Pointer img = mitk::Image::New();
      img->InitializeByItk(outImg.GetPointer());
      img->SetVolume(outImg->GetBufferPointer());

      mitk::IOUtil::Save(img, outFileName );
    }
    else if (binary)
    {
      typedef unsigned char OutPixType;
      typedef itk::Image<OutPixType, 3> OutImageType;

      itk::TractDensityImageFilter< OutImageType >::Pointer generator = itk::TractDensityImageFilter< OutImageType >::New();
      generator->SetFiberBundle(fib);
      generator->SetBinaryOutput(binary);
      generator->SetOutputAbsoluteValues(!normalize);
      generator->SetWorkOnFiberCopy(false);

      if (ref_img.IsNotNull())
      {
        OutImageType::Pointer itkImage = OutImageType::New();
        CastToItkImage(ref_img, itkImage);
        generator->SetInputImage(itkImage);
        generator->SetUseImageGeometry(true);

      }
      generator->Update();

      // get output image
      typedef itk::Image<OutPixType,3> OutType;
      OutType::Pointer outImg = generator->GetOutput();
      mitk::Image::Pointer img = mitk::Image::New();
      img->InitializeByItk(outImg.GetPointer());
      img->SetVolume(outImg->GetBufferPointer());

      mitk::IOUtil::Save(img, outFileName );
    }
    else
    {
      typedef float OutPixType;
      typedef itk::Image<OutPixType, 3> OutImageType;

      itk::TractDensityImageFilter< OutImageType >::Pointer generator = itk::TractDensityImageFilter< OutImageType >::New();
      generator->SetFiberBundle(fib);
      generator->SetBinaryOutput(binary);
      generator->SetOutputAbsoluteValues(!normalize);
      generator->SetWorkOnFiberCopy(false);

      if (ref_img.IsNotNull())
      {
        OutImageType::Pointer itkImage = OutImageType::New();
        CastToItkImage(ref_img, itkImage);
        generator->SetInputImage(itkImage);
        generator->SetUseImageGeometry(true);

      }
      generator->Update();

      // get output image
      typedef itk::Image<OutPixType,3> OutType;
      OutType::Pointer outImg = generator->GetOutput();
      mitk::Image::Pointer img = mitk::Image::New();
      img->InitializeByItk(outImg.GetPointer());
      img->SetVolume(outImg->GetBufferPointer());

      mitk::IOUtil::Save(img, outFileName );
    }

  }
  catch (itk::ExceptionObject e)
  {
    std::cout << e;
    return EXIT_FAILURE;
  }
  catch (std::exception e)
  {
    std::cout << e.what();
    return EXIT_FAILURE;
  }
  catch (...)
  {
    std::cout << "ERROR!?!";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
