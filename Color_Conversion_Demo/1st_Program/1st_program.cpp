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

 Implementation and demonstration of various color conversion algorithms using OpenCV 
*/

#include <opencv2/highgui.hpp>
#include <iostream>
#include "color_conversions.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
  //Check that input arguments were valid
  if(argc != 3) {
    cout << argv[0] << ": "
	 << "got " << argc-1 << " arguments. Expecting two: width height." 
	 << endl ;
    return(-1);
  }

  //Set height and width variables from input arguments
  int height = atoi(argv[1]);
  int width = atoi(argv[2]);
  cout << "Height and Width = {" << height << "," << width << "}" << endl;

  //Initialize the xyY and Luv images
  //Luv and XYZ are floating point images
  int depth=CV_32FC1;
  int depth2=CV_32FC3;
  Mat x(height, width, depth);
  Mat y(height, width, depth);
  Mat Y(height, width, depth);
  Mat L(height, width, depth);
  Mat u(height, width, depth);
  Mat v(height, width, depth);
  
  for(int i = 0 ; i <  height; i++){
    for(int j = 0 ; j < width; j++){
	x.at<float>(i,j)=(float)j/(float)width;
	y.at<float>(i,j) = (float)i/(float)height;
	Y.at<float>(i,j) = 1.0;

	L.at<float>(i,j) = 90.0;
	u.at<float>(i,j) = x.at<float>(i,j) * 512.0 - 255.0;
	v.at<float>(i,j) = y.at<float>(i,j) * 512.0 - 255.0;
    }
  }
  
  //
  // Translate xyY to byte sRGB and Luv to byte sRGB
  //
 
  // Create Mat files for xyY and Luv images
  Mat xyY_planes[] = {x,y,Y};
  Mat Luv_planes[] = {L,u,v};
  Mat xyY(height, width, depth2);
  Mat Luv(height, width, depth2);
  merge(xyY_planes, 3, xyY);
  merge(Luv_planes, 3, Luv);

  //Initialize the needed intermediate and final xyY images
  Mat xyY2XYZ(height, width, depth2);
  Mat xyY2lRGB(height, width, depth2);
  Mat xyY2nRGB(height, width, depth2);
  
  //non-linear scaled RGB images are [0-255] byte (uint) images
  int depth3=CV_8UC3;
  Mat xyY2nsRGB(height, width, depth3);
  Mat xyY2nsBGR(height, width, depth3);

  //Initialize the needed intermediate and final Luv images
  Mat Luv2XYZ(height, width, depth2);
  Mat Luv2lRGB(height, width, depth2);
  Mat Luv2nRGB(height, width, depth2);
  
  //non-linear scaled RGB images are [0-255] byte (uint) images
  Mat Luv2nsRGB(height, width, depth3);
  Mat Luv2nsBGR(height, width, depth3);

  cout << "Starting color conversions." << endl;

  //Convert xyY to nonlinear scaled RGB in 4 steps
  xyYtoXYZ(xyY,xyY2XYZ);
  cout << "Completed xyY to XYZ conversion." << endl;
  XYZtolRGB(xyY2XYZ,xyY2lRGB);
  cout << "Completed xyY to lRGB conversion." << endl;
  lRGBtonRGB(xyY2lRGB,xyY2nRGB);
  cout << "Completed xyY to nRGB conversion." << endl;
  nRGBtonsRGB(xyY2nRGB,xyY2nsRGB);
  cout << "Completed xyY to nsRGB conversion." << endl;

  //Convert Luv to nonlinear scaled RGB in 4 steps
  LuvtoXYZ(Luv,Luv2XYZ);
  cout << "Completed LuV to XYZ conversion." << endl;
  XYZtolRGB(Luv2XYZ,Luv2lRGB);
  cout << "Completed Luv to lRGB conversion." << endl;
  lRGBtonRGB(Luv2lRGB,Luv2nRGB);
  cout << "Completed Luv to nRGB conversion." << endl;
  nRGBtonsRGB(Luv2nRGB,Luv2nsRGB);
  cout << "Completed Luv to nsRGB conversion." << endl;

  //Convert RGB to BGR
  cvtColor(xyY2nsRGB, xyY2nsBGR, COLOR_RGB2BGR);
  cvtColor(Luv2nsRGB, Luv2nsBGR, COLOR_RGB2BGR);

  cout << "All conversions complete." << endl;

  //Show the xyY image converted to non-linear scaled BGR
  namedWindow("xyY to nsBGR",WINDOW_AUTOSIZE);
  imshow("xyY to nsBGR", xyY2nsBGR);
  imwrite("xyY.png",xyY2nsRGB);

  //Show the Luv image converted to non-linear scaled BGR
  namedWindow("Luv to nsBGR",WINDOW_AUTOSIZE);
  imshow("Luv to nsBGR", Luv2nsBGR);
  imwrite("Luv.png",Luv2nsRGB);

  //Test to see what OpenCV 3.0 does directly
  //Mat BGR(height, width, depth3);
  //cvtColor(Luv, BGR, COLOR_Luv2BGR);
  //namedWindow("Luv to nsBGR - OpenCV",WINDOW_AUTOSIZE);
  //imshow("Luv to nsBGR - OpenCV", BGR);

  waitKey(0); // Wait for a keystroke

  return(0);
}
