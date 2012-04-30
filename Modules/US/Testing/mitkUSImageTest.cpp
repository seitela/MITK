/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2008-02-25 17:27:17 +0100 (Mo, 25 Feb 2008) $
Version:   $Revision: 7837 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "mitkUSImage.h"
#include "mitkTestingMacros.h"
#include "mitkUSImageMetadata.h"


class mitkUSImageTestClass
{
public:

  // Anm: Implementierung der einzelnen Testmethoden

  static void TestInstantiation()
  {
    // let's create an object of our class
    mitk::USImage::Pointer usImage = mitk::USImage::New();
    MITK_TEST_CONDITION_REQUIRED(usImage.IsNotNull(), "USImage should not be null after instantiation");
  }

  static void TestMetadataInProperties(){
    mitk::USImage::Pointer usImage = mitk::USImage::New();
    mitk::USImageMetadata::Pointer meta1 = mitk::USImageMetadata::New();
    // Create a complete metadataset and write it to the image
    meta1->SetDeviceComment("comment");
    meta1->SetDeviceIsVideoOnly(true);
    meta1->SetDeviceManufacturer("manufacturer");
    meta1->SetDeviceModel("model");
    meta1->SetProbeFrequency("7 MHz");
    meta1->SetProbeName("CA X3");
    meta1->SetZoom("3x");
    usImage->SetMetadata(meta1);

    //read metadata from image an compare for equality
    mitk::USImageMetadata::Pointer meta2 = usImage->GetMetadata();
    MITK_TEST_CONDITION_REQUIRED(meta1->GetDeviceComment().compare(meta2->GetDeviceComment()) == 0, "Comparing Metadata after write & read: Device Comment");
    MITK_TEST_CONDITION_REQUIRED(meta1->GetDeviceManufacturer().compare(meta2->GetDeviceManufacturer()) == 0, "Comparing Metadata after write & read: Device Manufacturer");
    MITK_TEST_CONDITION_REQUIRED(meta1->GetDeviceModel().compare(meta2->GetDeviceModel()) == 0, "Comparing Metadata after write & read: Device Model");
    MITK_TEST_CONDITION_REQUIRED(meta1->GetDeviceIsVideoOnly() == meta2->GetDeviceIsVideoOnly() , "Comparing Metadata after write & read: IsVideoOnly");
    MITK_TEST_CONDITION_REQUIRED(meta1->GetProbeName().compare(meta2->GetProbeName()) == 0, "Comparing Metadata after write & read: Probe Name");
    MITK_TEST_CONDITION_REQUIRED(meta1->GetProbeFrequency().compare(meta2->GetProbeFrequency()) == 0, "Comparing Metadata after write & read: Frequency");
    MITK_TEST_CONDITION_REQUIRED(meta1->GetZoom().compare(meta2->GetZoom()) == 0, "Comparing Metadata after write & read: Zoom Factor");
    
  }

};

/**
* This function is testing methods of the class USImage2D.
*/
int mitkUSImageTest(int /* argc */, char* /*argv*/[])
{
  MITK_TEST_BEGIN("mitkUSImage2DTest");

    mitkUSImageTestClass::TestInstantiation();
    mitkUSImageTestClass::TestMetadataInProperties();

  MITK_TEST_END();
}