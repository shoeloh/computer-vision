/* MIT License
 
 Copyright (c) 2019 Shane Zabel 

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 =============================================================================

 Implementation of various color conversion algorithms using OpenCV 
*/

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

#ifndef COLOR_CONVERSIONS_HPP_
#define COLOR_CONVERSIONS_HPP_

//Function takes non-linear scaled [0-255] RGB Mat object reference and updates nonlinear [0-1] RGB Mat object reference
void nsRGBtonRGB(const Mat& nsRGB, Mat& nRGB);
//Function takes non-linear [0-1] RGB object reference and returns linear [0-1] RGB object reference
void nRGBtolRGB(const Mat& nRGB, Mat& lRGB);
//Function takes linear [0-1] RGB Mat object reference and updates XYZ Mat object reference
void lRGBtoXYZ(const Mat& lRGB, Mat& XYZ);
//Function takes an XYZ Mat object reference and updates an xyY Mat object reference
void XYZtoxyY(const Mat& XYZ, Mat& xyY);
//Function takes an XYZ Mat object reference and updates an Luv Mat object reference
void XYZtoLuv(const Mat& XYZ, Mat& Luv);
//Function takes Luv Mat object reference and updates stretchLuv Mat object reference with linearly stretched [0-100] L values
void stretchLuv(const Mat& Luv, Mat& stretchLuv);
//Function takes an xyY Mat object reference and updates XYZ Mat object reference
void xyYtoXYZ(const Mat& xyY, Mat& XYZ);
//Function takes lan Luv Mat object reference and updates XYZ Mat object reference
void LuvtoXYZ(const Mat& Luv, Mat& XYZ);
//Function takes an XYZ Mat object reference and updates a linear RGB Mat object reference
void XYZtolRGB(const Mat& XYZ, Mat& lRGB);
//Function takes linear [0-1] RGB Mat object reference and updates XYZ Mat object reference
void lRGBtonRGB(const Mat& lRGB, Mat& nRGB);
//Function takes non-linear [0-1] RGB Mat object reference and updates nonlinear scaled [0-255] RGB Mat object reference
void nRGBtonsRGB(const Mat& nRGB, Mat& nsRGB);
//Function takes non-linear scaled [0-255] RGB image and stretches L in Luv domain based on window {h1,w1},{h2,w2}
void WindowStretchLuv(const Mat& Luv, Mat& stretchLuv, double w1, double w2, double h1, double h2);
//Function takes xyY image and stretches Y [0.0-1.0] in xyY domain based on window {h1,w1},{h2,w2}
void WindowStretchxyY(const Mat& xyY, Mat& stretchxyY, double w1, double w2, double h1, double h2);
//Function takes Luv image and histogram equalizes L [0.0-100.0] in Luv domain based on window {h1,w1},{h2,w2}
void LequLuv(const Mat& Luv, Mat& equLuv, double w1, double w2, double h1, double h2);

#endif /* COLOR_CONVERSIONS_HPP_ */
