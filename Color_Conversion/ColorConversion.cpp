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

float invgamma(float v){
	if(v<0.03928){
		return(v/12.92);
	}else{
		return(pow((v+0.055)/1.055,2.4));
	}
}

vector<float> XYZtoxyY(vector<float> XYZ){
	float X,Y,Z;
	float x,y;
	vector<float> answer (3);

	X=XYZ.at(0);
	Y=XYZ.at(1);
	Z=XYZ.at(2);

	if((X<0.00001)&(Y<0.00001)&(Z<0.00001)){
		answer.at(0)=0.0;
		answer.at(1)=0.0;
		answer.at(2)=0.0;
	}else{
		x=X/(X+Y+Z);
		y=Y/(X+Y+Z);
		answer.at(0)=x;
		answer.at(1)=y;
		answer.at(2)=Y;
	}
	return(answer);
}

int main(int argc, char** argv) {
  if(argc != 1) {
    cout << argv[0] << ": no arguments" << endl ;
    return(-1);
  }
  
  const int rows = 4;
  const int cols = 4;

// Create the scaled non-linear RGB image as a rows x cols x 3 float matrix
  // Sgtarting image is in (r,g,b) triplets
  int colorValues[rows][cols][3] = {
    {{0,0,0},{0,0,0},{0,0,0},{0,0,0}},
    {{255,0,0},{255,0,0},{255,0,0},{255,0,0}},
    {{100,100,100},{100,100,100},{100,100,100},{100,100,100}},
    {{0,100,100},{0,100,100},{0,100,100},{0,100,100}}
  };

//Create the scaled non-linear RGB image
  Mat Rscaled(rows, cols, CV_32F);
  Mat Gscaled(rows, cols, CV_32F);
  Mat Bscaled(rows, cols, CV_32F);

  for(int i = 0 ; i < rows ; i++){
    for(int j = 0 ; j < cols ; j++) {
      Rscaled.at<float>(i,j) = (float)colorValues[i][j][0];
      Gscaled.at<float>(i,j) = (float)colorValues[i][j][1];
      Bscaled.at<float>(i,j) = (float)colorValues[i][j][2];
    }
  }

  Mat RGBimage;
  Mat planes[] = {Rscaled, Gscaled, Bscaled};
  merge(planes, 3, RGBimage);
  cout << "Scaled non-linear RGB image=" << RGBimage << endl << endl;

//Create the non-linear RGB image from the scaled non-linear RGB image
  Mat R(rows, cols, CV_32F);
  Mat G(rows, cols, CV_32F);
  Mat B(rows, cols, CV_32F);

  R = Rscaled/255.0;
  G = Gscaled/255.0;
  B = Bscaled/255.0;

  Mat rgbimage;
  Mat planes1[] = {R, G, B};
  merge(planes1, 3, rgbimage);
  cout << "Non-linear RGB image=" << rgbimage << endl << endl;

//Create linear RGB image from the non-linear RGB image
  Mat Rlinear(rows, cols, CV_32F);
  Mat Glinear(rows, cols, CV_32F);
  Mat Blinear(rows, cols, CV_32F);
  vector<Mat> RGB_planes;

  split(rgbimage, RGB_planes);
  Rlinear = RGB_planes[0];
  Glinear = RGB_planes[1];
  Blinear = RGB_planes[2];

  for(int i = 0 ; i < rows ; i++){
    for(int j = 0 ; j < cols ; j++) {
      Rlinear.at<float>(i,j) = invgamma(Rlinear.at<float>(i,j));
      Glinear.at<float>(i,j) = invgamma(Glinear.at<float>(i,j));
      Blinear.at<float>(i,j) = invgamma(Blinear.at<float>(i,j));
    }
  }

  Mat rgblinearimage;
  Mat planes2[] = {Rlinear, Glinear, Blinear};
  merge(planes2, 3, rgblinearimage);
  cout << "Linear RGB image=" << rgblinearimage << endl << endl;

//Convert non-linear RGB image to XYZ
  Mat xyzimage(rgbimage);
  cvtColor(xyzimage, xyzimage, COLOR_RGB2XYZ);
  cout << "XYZ image =" << xyzimage << endl << endl;

//Convert XYZ image to xyY
  Mat x(rows, cols, CV_32F);
  Mat y(rows, cols, CV_32F);
  Mat Y(rows, cols, CV_32F);
  vector<Mat> XYZ_planes;
  vector<float> xyY(3),XYZ(3);

  split(xyzimage, XYZ_planes);
  x = XYZ_planes[0];
  y = XYZ_planes[1];
  Y = XYZ_planes[2];

  for(int i = 0 ; i < rows ; i++){
    for(int j = 0 ; j < cols ; j++) {
      XYZ.at(0)=x.at<float>(i,j);
      XYZ.at(1)=y.at<float>(i,j);
      XYZ.at(2)=Y.at<float>(i,j);
      xyY = XYZtoxyY(XYZ);
      x.at<float>(i,j)=xyY.at(0);
      y.at<float>(i,j)=xyY.at(1);
      Y.at<float>(i,j)=xyY.at(2);
    }
  }

  Mat xyYimage;
  Mat planes3[] = {x,y,Y};
  merge(planes3, 3, xyYimage);
  cout << "xyY image=" << xyYimage << endl << endl;

//Convert non-linear RGB image to Luv
  Mat luvimage(rgbimage);
  cvtColor(luvimage, luvimage, COLOR_RGB2Luv);
  cout << "Luv image =" << luvimage << endl << endl;

//Stretch L to [0,100]
  vector<Mat> Luv_planes;
  double minL,maxL;

  split(luvimage, Luv_planes);
  Mat L = Luv_planes[0];
  Mat u = Luv_planes[1];
  Mat v = Luv_planes[2];

  minMaxLoc(L,&minL,&maxL);

  for(int i=0;i<rows;i++){
	  for(int j=0;j<cols;j++){
		  L.at<float>(i,j)=(L.at<float>(i,j)-minL)*100.0/(maxL-minL);
	  }
  }

  Mat stretchedLuvimage;
  Mat planes4[] = {L,u,v};
  merge(planes4, 3, stretchedLuvimage);
  cout << "Stretched Luv image=" << stretchedLuvimage << endl << endl;

//Convert stretched Luv back to RGB
  Mat newimage(stretchedLuvimage);
  cvtColor(newimage, newimage, COLOR_Luv2RGB);
  cout << "New RGB image =" << newimage << endl << endl;
  
  return(0);
}
