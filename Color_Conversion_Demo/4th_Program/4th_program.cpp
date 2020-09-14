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
	if(argc != 7) {
	    cerr << argv[0] << ": "
		 << "got " << argc-1
		 << " arguments. Expecting six: w1 h1 w2 h2 ImageIn ImageOut."
		 << endl ;
	    cerr << "Example: proj1b 0.2 0.1 0.8 0.5 fruits.jpg out.bmp" << endl;
	    return(-1);
	  }
	  double w1 = atof(argv[1]);
	  double h1 = atof(argv[2]);
	  double w2 = atof(argv[3]);
	  double h2 = atof(argv[4]);
	  char *inputName = argv[5];
	  char *outputName = argv[6];

	  if(w1<0 || h1<0 || w2<=w1 || h2<=h1 || w2>1 || h2>1) {
	    cerr << " arguments must satisfy 0 <= w1 < w2 <= 1"
		 << " ,  0 <= h1 < h2 <= 1" << endl;
	    return(-1);
	  }

	  Mat inputImage = imread(inputName);
	  if(inputImage.empty()) {
	    cout <<  "Could not open or find the image " << inputName << endl;
	    return(-1);
	  }

	  string windowInput("input: ");
	  windowInput += inputName;

	  namedWindow(windowInput, WINDOW_AUTOSIZE);
	  imshow(windowInput, inputImage);

	  if(inputImage.type() != CV_8UC3) {
	    cout <<  inputName << " is not a standard 8UC3 color image  " << endl;
	    return(-1);
	  }
	  int depth1 = CV_8UC3;
	  int depth2 = CV_32FC3;
	  int height = inputImage.rows;
	  int width = inputImage.cols;

	  //Initialize the needed intermediate and final images
	  Mat nsRGB(height, width, depth1);
	  Mat nRGB(height, width, depth2);
	  Mat lRGB(height, width, depth2);
	  Mat XYZ(height, width, depth2);
	  Mat xyY(height, width, depth2);
	  Mat stretchxyY(height, width, depth2);
	  Mat XYZ2(height, width, depth2);
	  Mat lRGB2(height, width, depth2);
	  Mat nRGB2(height, width, depth2);
	  Mat outputImage(height, width, depth1);
	  Mat outputImageBGR(height, width, depth1);

	  cout << "Starting color conversions." << endl;

	  //Convert input image (nsRGB) to Luv in 5 steps
  	  cvtColor(inputImage, nsRGB, COLOR_RGB2BGR);
  	  cout << "Completed nsBGR to nsRGB conversion." << endl;
	  nsRGBtonRGB(nsRGB,nRGB);
	  cout << "Completed nsRGB to nRGB conversion." << endl;
	  nRGBtolRGB(nRGB,lRGB);
	  cout << "Completed nRGB to lRGB conversion." << endl;
	  lRGBtoXYZ(lRGB,XYZ);
	  cout << "Completed lRGB to XYZ conversion." << endl;
	  XYZtoxyY(XYZ,xyY);
	  cout << "Completed XYZ to xyY conversion." << endl;

	  //Stretch Y in window in xyY image
	  WindowStretchxyY(xyY, stretchxyY, w1, w2, h1, h2);
	  cout << "Completed Y stretch using window Y values." << endl;

	  //Convert stretched xyY to nonlinear scaled RGB in 4 steps
	  xyYtoXYZ(stretchxyY,XYZ2);
	  cout << "Completed xyY to XYZ conversion." << endl;
	  XYZtolRGB(XYZ2,lRGB2);
  	  cout << "Completed XYZ to lRGB conversion." << endl;
  	  lRGBtonRGB(lRGB2,nRGB2);
  	  cout << "Completed lRGB to nRGB conversion." << endl;
  	  nRGBtonsRGB(nRGB2,outputImage);
  	  cout << "Completed nRGB to nsRGB conversion." << endl;

  	  //Convert RGB to BGR
  	  cvtColor(outputImage, outputImageBGR, COLOR_RGB2BGR);

  	  cout << "All conversions complete." << endl;

  	  //Show the stretched Luv image converted to non-linear scaled BGR
  	  namedWindow("Y stretched image",WINDOW_AUTOSIZE);
  	  imshow("Y stretched image", outputImageBGR);
  	  waitKey(0); // Wait for a keystroke

  	  //Write out output image
  	  imwrite(outputName,outputImageBGR);

return(0);
}
