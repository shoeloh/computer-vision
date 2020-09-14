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

 Program to read in an image and then create a thresholded version of that image  using OpenCV 
*/

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
 
  if(argc != 2) {
    cerr << argv[0] << ": "
	 << "got " << argc-1 << " arguments. Expecting one: an image." 
	 << endl ;
    return(-1);
  }

  Mat inputImage = imread(argv[1], IMREAD_UNCHANGED);  // Read the image
  if(inputImage.empty()) {
    cerr <<  "Could not open or find the image " << argv[1] << endl ;
    return(-1);
  }

  namedWindow("input",WINDOW_AUTOSIZE);
  imshow("input", inputImage);
  int rows = inputImage.rows;
  int cols = inputImage.cols;

  // Make sure image is gray level
  Mat grayImage(rows, cols, CV_8UC1);
  if(inputImage.type() == CV_8UC1) grayImage = inputImage;
  else {
    if(inputImage.type() == CV_8UC3){
      cout << "Convert color image to grayscale." << "\n" ;
      cvtColor(inputImage, grayImage, COLOR_BGR2GRAY);
    } else {
      cerr <<  "Can't deal with image " << argv[1] << endl ;
      return(-1);
    }
  }
  imshow("grayImage", grayImage);

  // Compute the histogram
  int hist[256];
  for(int k = 0 ; k < 256 ; k++) hist[k] = 0;
  for(int i = 0 ; i < rows ; i++)
    for(int j = 0 ; j < cols ; j++) 
      hist[grayImage.at<uchar>(i,j)]++;

  // Print the histogram
  for(int i=0 ; i<256 ; i++){
	  cout << hist[i] << " ";
  }

  // Use the histogram to compute threshold value t.
  double E,Emin;
  int tmin;
  double q1,q2;
  double chi,hchi;
  double num1,denom1;
  double num2,denom2;

  for(int t=1 ; t<256 ; t++){
	  num1=0,denom1=0;
	  num2=0,denom2=0;
	  E=0.0;

	  //For the given t, calculate q1
	  for(int j=0; j<t; j++){
		  chi=j;
		  hchi=hist[j];
		  num1+=chi*hchi;
		  denom1+=hchi;
	  }
	  if(denom1>=0.0000001||denom1<-0.0000001){
		  q1=num1/denom1;
		  cout << "q1 " << q1 << endl;
	  }else{
		  cout << "Problem with denom1." << endl;
		  q1=999999;
	  }

	  //For the given t, calculate q2
	  for(int j=t; j<256; j++){
		  chi=j;
		  hchi=hist[j];
		  num2+=chi*hchi;
		  denom2+=hchi;
	  }

	  if(denom2>=0.0000001||denom2<-0.0000001){
		  q2=num2/denom2;
		  cout << "q2 " << q2 << endl;
	  }else{
		  cout << "Problem with denom2." << endl;
		  q2=999999;
	  }

	  //For the given t, calculate the first summation for E
	  for(int j=0; j<t; j++){
		  chi=j;
		  hchi=hist[j];
//		  cout << "E+= " << pow((chi-q2),2)*hchi << endl;
		  E+=pow((chi-q1),2)*hchi;
	  }

	  //For a given t, calculate the second summation for E
	  for(int j=t; j<256; j++){
		  chi=j;
		  hchi=hist[j];
// 		  cout << "E+= " << pow((chi-q2),2)*hchi << endl;
		  E+=pow((chi-q2),2)*hchi;
	  }

	  //Compare computed E(t,q1,q2) to Emin. If less then store off t and E as tmin and Emin
	  if(t==1){
		  cout << "E = " << E << endl;
		  Emin=E;
		  tmin=t;
		  cout << "t = " << t << " and Emin = " << Emin << endl;
	  }else{
		  if(E<Emin){
			  Emin=E;
			  tmin=t;
			  cout << "t = " << t << " and Emin = " << Emin << endl;
		  }else{
			  cout << "E > Emin " << endl;
			  cout << "t = " << t << " and E = " << E << " and Emin = " << Emin << endl;
		  }
	  }
  //Loop back for all t values
  }

  cout << "Threshold value is " << tmin << endl;
  cout << "Threshold energy is " << Emin << endl;

  Mat thresholdedImage(rows, cols, CV_8UC1);
  threshold(grayImage, thresholdedImage, tmin, 255, THRESH_BINARY);
  imshow("thresholded Image", thresholdedImage);

  waitKey(0); // Wait for a keystroke
  return(0);
}

