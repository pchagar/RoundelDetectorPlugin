# RoundelDetectorPlugin

This plugin can be used to track a BMW roundel by using a trained haar cascade for roundels and returning the point at which the logo is located.

Tools Used:

Visual Studio 15, Windows 10, OpenCV 3.1 (https://github.com/sylvain-prevost/opencv-hololens)

This plugin can be built and used in the Hololens (towards Release x86) and dropped into Unity (specifically in Assets->Plugins->WSA) with the relevant OpenCV dlls that are contained in the Release folder of the plugin project when built, for example opencv_core310.dll.
