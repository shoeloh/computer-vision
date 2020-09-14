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
#include <cmath>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

//Function takes non-linear scaled [0-255] byte (uint) RGB Mat object reference
//and updates nonlinear [0-1] float RGB Mat object reference
void nsRGBtonRGB(const Mat& nsRGB, Mat& nRGB){
	int width,height;

	width=nsRGB.cols;
	height=nsRGB.rows;

	for(int i = 0 ; i < width ; i++){
		for(int j = 0 ; j < height ; j++) {
			Vec3b nsRGBval = nsRGB.at<Vec3b>(j, i);
			Vec3f color;

			uint nsR = nsRGBval[0];
			uint nsG = nsRGBval[1];
			uint nsB = nsRGBval[2];
			float nR,nG,nB;

			nR=nsR/255.0;
			nG=nsG/255.0;
			nB=nsB/255.0;

        	if(nR<0.0) nR=0.0;
        	if(nG<0.0) nG=0.0;
        	if(nB<0.0) nB=0.0;
        	if(nR>1.0) nR=1.0;
        	if(nG>1.0) nG=1.0;
        	if(nB>1.0) nB=1.0;

			color[0]=nR;
			color[1]=nG;
			color[2]=nB;
			nRGB.at<Vec3f>(j,i)=color;
		}
	}
return void();
}

//Function computes inverse gamma correction of a float
float invgamma(float v){
	float answer;

	if(v<0.03928){
		answer = v/12.92;
	}else{
		answer = pow((v+0.055)/1.055,2.4);
	}
return answer;
}

//Function takes non-linear [0-1] RGB object reference
//and returns linear [0-1] RGB object reference
void nRGBtolRGB(const Mat& nRGB, Mat& lRGB){
	int width,height;

	width=nRGB.cols;
	height=nRGB.rows;

	for(int i = 0 ; i < width ; i++){
		for(int j = 0 ; j < height ; j++) {
			Vec3f nRGBval = nRGB.at<Vec3f>(j, i);
			Vec3f color;

			float nR = nRGBval[0];
			float nG = nRGBval[1];
			float nB = nRGBval[2];
			float lR,lG,lB;

			lR=invgamma(nR);
			lG=invgamma(nG);
			lB=invgamma(nB);

        	if(lR<0.0) lR=0.0;
        	if(lG<0.0) lG=0.0;
        	if(lB<0.0) lB=0.0;
        	if(lR>1.0) lR=1.0;
        	if(lG>1.0) lG=1.0;
        	if(lB>1.0) lB=1.0;

			color[0]=lR;
			color[1]=lG;
			color[2]=lB;
			lRGB.at<Vec3f>(j,i)=color;
		}
	}
return void();
}

//Function takes linear [0-1] RGB Mat object reference
//and updates XYZ Mat object reference
void lRGBtoXYZ(const Mat& lRGB, Mat& XYZ){
	int width,height;

	width=lRGB.cols;
	height=lRGB.rows;

	for(int i = 0 ; i < width ; i++){
	    for(int j = 0 ; j < height ; j++) {
	        Vec3f lRGBval = lRGB.at<Vec3f>(j, i);
	        Vec3f color;

	        float lR = lRGBval[0];
	        float lG = lRGBval[1];
	        float lB = lRGBval[2];
	        float X,Y,Z;

	    	X=0.412*lR+0.358*lG+0.180*lB;
	    	Y=0.213*lR+0.715*lG+0.072*lB;
	    	Z=0.019*lR+0.119*lG+0.950*lB;

        	if(X<0.0) X=0.0;
	       	if(Y<0.0) Y=0.0;
	       	if(Z<0.0) Z=0.0;

	        color[0]=X;
	        color[1]=Y;
	        color[2]=Z;

	        XYZ.at<Vec3f>(j,i)=color;
	    }
	}
return void();
}

//Function takes an XYZ Mat object reference
//and updates an xyY Mat object reference
void XYZtoxyY(const Mat& XYZ, Mat& xyY){
	int width,height;

	width=XYZ.cols;
	height=XYZ.rows;

	for(int i = 0 ; i < width ; i++){
		for(int j = 0 ; j < height ; j++) {
	        Vec3f XYZval = XYZ.at<Vec3f>(j, i);
	        Vec3f color;

	        float X = XYZval[0];
	        float Y = XYZval[1];
	        float Z = XYZval[2];
	        float x,y;

	        if(X<0.000001 && Y<0.000001 && Z<0.000001){
	        	x=0.0;
	        	y=0.0;
	        	Y=0.0;
	        	color[0]=x;
	        	color[1]=y;
	        	color[2]=Y;
	        	xyY.at<Vec3f>(j,i)=color;
	        }else{
	        	x=X/(X+Y+Z);
	        	y=Y/(X+Y+Z);
	        	if(x<0.0) x=0.0;
	        	if(y<0.0) y=0.0;
	        	if(x>1.0) x=1.0;
	        	if(y>1.0) y=1.0;
	        	if(Y<0.0) Y=0.0;
		       	color[0]=x;
		       	color[1]=y;
		       	color[2]=Y;
		       	xyY.at<Vec3f>(j,i)=color;
	        }
	    }
	}
return void();
}

//Function takes an XYZ Mat object reference
//and updates an Luv Mat object reference
void XYZtoLuv(const Mat& XYZ, Mat& Luv){
	int width,height;

	width=XYZ.cols;
	height=XYZ.rows;

	//Assume D65 white
	float Xw=0.95;
  	float Yw=1.0;
  	float Zw=1.09;

	//Compute uw and vw
 	float uw=4.0*Xw/(Xw+15.0*Yw+3.0*Zw);
  	float vw=9.0*Yw/(Xw+15.0*Yw+3.0*Zw);

	for(int i = 0 ; i < width ; i++){
		for(int j = 0 ; j < height ; j++) {
			Vec3f XYZval = XYZ.at<Vec3f>(j, i);
		    Vec3f color;

	        float X = XYZval[0];
	        float Y = XYZval[1];
	        float Z = XYZval[2];
	        float L,u,v;
	        float uprime, vprime;

			//Compute t and L
			float t=Y/Yw;
			if(t>0.008856){
				L=116.0*pow(t,1.0/3.0)-16.0;
			}else{
				L=903.3*t;
			}
        	if(L<0.000001) L=0.0;
        	if(L>100.0) L=100.0;

			//Compute d, uprime and vprime
			float d=X+15.0*Y+3.0*Z;
			if(d>0.000001){
				uprime=4.0*X/d;
				vprime=9.0*Y/d;
			}else{
				uprime=0.0;
				vprime=0.0;
			}

			//Compute u and v
        	if(L>0.000001){
        		u=13.0*L*(uprime-uw);
        		v=13.0*L*(vprime-vw);
        	}else{
        		u=0.0;
        		v=0.0;
        	}

	        color[0]=L;
	        color[1]=u;
	        color[2]=v;
	        Luv.at<Vec3f>(j,i)=color;
	    }
	}
return void();
}

//Function takes Luv Mat object reference and updates stretchLuv Mat object reference
//with linearly stretched [0-100] L values
void stretchLuv(const Mat& Luv, Mat& stretchLuv){
	int width,height,depth;
	Point min_loc, max_loc;
	double min,max;

	width=Luv.cols;
	height=Luv.rows;
	depth=Luv.depth();

	Mat L(height, width, depth);
	Mat u(height, width, depth);
	Mat v(height, width, depth);
	Mat Lstretch(height, width, depth);

	vector<Mat> Luv_planes;

	split(Luv, Luv_planes);
	L = Luv_planes[0];
	u = Luv_planes[1];
	v = Luv_planes[2];

	minMaxLoc(L, &min, &max, &min_loc, &max_loc);

	Lstretch=(L-min)*100/max;

	Mat new_planes[] = {Lstretch,u,v};
	Mat sLuv;
	merge(new_planes, 3, sLuv);

	sLuv.copyTo(stretchLuv);

	return void();
}

//Function takes an xyY Mat object reference
//and updates XYZ Mat object reference
void xyYtoXYZ(const Mat& xyY, Mat& XYZ){
	int width,height;

	width=xyY.cols;
	height=xyY.rows;

	for(int i = 0 ; i < width ; i++){
		for(int j = 0 ; j < height ; j++) {
			Vec3f xyYval = xyY.at<Vec3f>(j, i);
			Vec3f color;

			float x = xyYval[0];
			float y = xyYval[1];
			float Y = xyYval[2];
			float X,Z;

			if(y>0.000001){
				X=x*Y/y;
				Z=(1.0-x-y)*Y/y;
				if(X<0.0) X=0.0;
				if(Y<0.0) Y=0.0;
				if(Z<0.0) Z=0.0;
				color[0]=X;
				color[1]=Y;
				color[2]=Z;
				XYZ.at<Vec3f>(j,i)=color;
			}else{
		       	color[0]=0.0;
		       	color[1]=0.0;
		       	color[2]=0.0;
		       	XYZ.at<Vec3f>(j,i)=color;
	       }
	    }
	}
return void();
}

//Function takes an Luv RGB Mat object reference
//and updates XYZ Mat object reference
void LuvtoXYZ(const Mat& Luv, Mat& XYZ){
	int width,height;

	width=Luv.cols;
	height=Luv.rows;

	//Assume D65 white
  	float Xw=0.95;
  	float Yw=1.0;
  	float Zw=1.09;

  	//Compute uw and vw
  	float uw=4.0*Xw/(Xw+15.0*Yw+3.0*Zw);
  	float vw=9.0*Yw/(Xw+15.0*Yw+3.0*Zw);

	for(int i = 0 ; i < width ; i++){
		for(int j = 0 ; j < height ; j++) {
			Vec3f Luvval = Luv.at<Vec3f>(j, i);
		    Vec3f color;

	        float L = Luvval[0];
	        float u = Luvval[1];
	        float v = Luvval[2];
	        float X,Y,Z;
	        float uprime, vprime;

	        //Compute uprime and vprime
	        if(L>0.000001){
	        	uprime=(u+13.0*uw*L)/(13.0*L);
	        	vprime=(v+13.0*vw*L)/(13.0*L);

	        	//Compute Y
	        	if(L>7.9996){
	        		Y=pow((L+16.0)/116.0,3.0)*Yw;
	        	}else{
	        		Y=L*Yw/903.3;
	        	}

	        	//Compute X and Z
	        	if(vprime<0.001){
	        		X=0.0;
	        		Z=0.0;
	        	}else{
	        		X=Y*2.25*uprime/vprime;
	        		Z=Y*(3.0-0.75*uprime-5.0*vprime)/vprime;
	        	}
	        }else{
	        	X=0.0;
	        	Y=0.0;
	        	Z=0.0;
	        }

	        if(X<0.0) X=0.0;
	        if(Y<0.0) Y=0.0;
	        if(Z<0.0) Z=0.0;
	        color[0]=X;
	        color[1]=Y;
	        color[2]=Z;
	        XYZ.at<Vec3f>(j,i)=color;
	    }
	}
	return void();
}



//Function takes an XYZ Mat object reference
//and updates a linear RGB Mat object reference
void XYZtolRGB(const Mat& XYZ, Mat& lRGB){
	int width,height;

	width=XYZ.cols;
	height=XYZ.rows;

	for(int i = 0 ; i < width ; i++){
		    for(int j = 0 ; j < height ; j++) {
		        Vec3f XYZval = XYZ.at<Vec3f>(j, i);
		        Vec3f color;

		        float X = XYZval[0];
		        float Y = XYZval[1];
		        float Z = XYZval[2];
		        float R,G,B;

		        R=(3.240479*X-1.53715*Y-0.498535*Z);
		        G=(-0.969256*X+1.875991*Y+0.041556*Z);
		        B=(0.055648*X-0.204043*Y+1.057311*Z);

	        	if(R<0.0) R=0.0;
	        	if(G<0.0) G=0.0;
	        	if(B<0.0) B=0.0;
	        	if(R>1.0) R=1.0;
	        	if(G>1.0) G=1.0;
	        	if(B>1.0) B=1.0;
		        color[0]=R;
		        color[1]=G;
		        color[2]=B;
		        lRGB.at<Vec3f>(j,i)=color;
		    }
	}

	return void();
}

//Function computes gamma correction of a single plane Mat object
float gamma(float v){
	float answer;

	if(v<0.00304){
		answer = v*12.92;
	}else{
		answer = 1.055*pow(v,1.0/2.4)-0.055;
	}

	return answer;
}

//Function takes linear [0-1] RGB Mat object reference
//and updates XYZ Mat object reference
void lRGBtonRGB(const Mat& lRGB, Mat& nRGB){
	int width,height;

	width=lRGB.cols;
	height=lRGB.rows;

	for(int i = 0 ; i < width ; i++){
		for(int j = 0 ; j < height ; j++) {
			Vec3f lRGBval = lRGB.at<Vec3f>(j, i);
			Vec3f color;

			float lR = lRGBval[0];
			float lG = lRGBval[1];
			float lB = lRGBval[2];
			float nR,nG,nB;

			nR=gamma(lR);
			nG=gamma(lG);
			nB=gamma(lB);

        	if(nR<0.0) nR=0.0;
        	if(nG<0.0) nG=0.0;
        	if(nB<0.0) nB=0.0;
        	if(nR>1.0) nR=1.0;
        	if(nG>1.0) nG=1.0;
        	if(nB>1.0) nB=1.0;

			color[0]=nR;
			color[1]=nG;
			color[2]=nB;
			nRGB.at<Vec3f>(j,i)=color;
		}
	}
return void();
}

//Function takes non-linear [0-1] RGB Mat object reference
//and updates nonlinear scaled [0-255] RGB Mat object reference
void nRGBtonsRGB(const Mat& nRGB, Mat& nsRGB){
	int width,height;

	width=nRGB.cols;
	height=nRGB.rows;

	for(int i = 0 ; i < width ; i++){
		for(int j = 0 ; j < height ; j++) {
			Vec3f nRGBval = nRGB.at<Vec3f>(j, i);
			Vec3b color;

			float nR = nRGBval[0];
			float nG = nRGBval[1];
			float nB = nRGBval[2];
			uint nsR,nsG,nsB;

			nsR=255*nR;
			nsG=255*nG;
			nsB=255*nB;

        	if(nsR<0) nsR=0;
        	if(nsG<0) nsG=0;
        	if(nsB<0) nsB=0;
        	if(nsR>255) nsR=255;
        	if(nsG>255) nsG=255;
        	if(nsB>255) nsB=255;

			color[0]=nsR;
			color[1]=nsG;
			color[2]=nsB;
			nsRGB.at<Vec3b>(j,i)=color;
		}
	}
return void();
}

//Function takes Luv Mat object reference and window coordinates (w1,w2,h1,h2)
//and updates stretchLuv Mat object reference with linearly stretched [0-100] L values
//using stretch values from window coordinates
void WindowStretchLuv(const Mat& Luv, Mat& stretchLuv, double w1, double w2, double h1, double h2){
	int width,height,inputType, depth;
	Point min_loc, max_loc;
	double min,max;

	width=Luv.cols;
	height=Luv.rows;
	depth=CV_32FC1;

	inputType=Luv.type();
	if(inputType!=CV_32FC3){
		cout << "WARNING: Input Luv image type is not CV_32FC3." << endl;
	}

	Mat L(height, width, depth);
	Mat u(height, width, depth);
	Mat v(height, width, depth);

	vector<Mat> Luv_planes;
	split(Luv, Luv_planes);
	L = Luv_planes[0];
	u = Luv_planes[1];
	v = Luv_planes[2];

	int ih1= (int) (h1*(height-1));
	int ih2= (int) (h2*(height-1));
	int iw1= (int) (w1*(height-1));
	int iw2= (int) (w2*(height-1));

	int height2=(ih2-ih1);
	int width2=(iw2-iw1);
	Mat Ltemp(height2, width2, depth);
	Mat utemp(height2, width2, depth);
	Mat vtemp(height2, width2, depth);

	for(int i=0; i<height2; i++)
		for(int j=0; j<width2; j++){
			Ltemp.at<float>(i,j)=L.at<float>(i+ih1,j+iw1);
			utemp.at<float>(i,j)=u.at<float>(i+ih1,j+iw1);
			vtemp.at<float>(i,j)=v.at<float>(i+ih1,j+iw1);
		}

	minMaxLoc(Ltemp, &min, &max, &min_loc, &max_loc);

	Mat Lstretch(height, width, depth);

	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			Lstretch.at<float>(i,j)=(L.at<float>(i,j)-min)*100.0/(max-min);
			if(Lstretch.at<float>(i,j)>100.0) Lstretch.at<float>(i,j)=100.0;
			if(Lstretch.at<float>(i,j)<0.0) Lstretch.at<float>(i,j)=0.0;
		}
	}

	Mat new_planes[] = {Lstretch,u,v};
	Mat sLuv;
	merge(new_planes, 3, sLuv);

	sLuv.copyTo(stretchLuv);

return void();
}

//Function takes xyY Mat object reference and window coordinates (w1,w2,h1,h2)
//and updates stretchxyY Mat object reference with linearly stretched [0-1] Y values
//using stretch values from window coordinates
void WindowStretchxyY(const Mat& xyY, Mat& stretchxyY, double w1, double w2, double h1, double h2){
	int width,height,inputType, depth;
	Point min_loc, max_loc;
	double min,max;

	width=xyY.cols;
	height=xyY.rows;
	depth=CV_32FC1;

	inputType=xyY.type();
	if(inputType!=CV_32FC3){
		cout << "WARNING: Input xyY image type is not CV_32FC3." << endl;
	}

	Mat x(height, width, depth);
	Mat y(height, width, depth);
	Mat Y(height, width, depth);

	vector<Mat> xyY_planes;
	split(xyY, xyY_planes);
	x = xyY_planes[0];
	y = xyY_planes[1];
	Y = xyY_planes[2];

	int ih1= (int) (h1*(height-1));
	int ih2= (int) (h2*(height-1));
	int iw1= (int) (w1*(height-1));
	int iw2= (int) (w2*(height-1));

	int height2=(ih2-ih1);
	int width2=(iw2-iw1);
	Mat xtemp(height2, width2, depth);
	Mat ytemp(height2, width2, depth);
	Mat Ytemp(height2, width2, depth);

	for(int i=0; i<height2; i++)
		for(int j=0; j<width2; j++){
			xtemp.at<float>(i,j)=x.at<float>(i+ih1,j+iw1);
			ytemp.at<float>(i,j)=y.at<float>(i+ih1,j+iw1);
			Ytemp.at<float>(i,j)=Y.at<float>(i+ih1,j+iw1);
		}

	minMaxLoc(Ytemp, &min, &max, &min_loc, &max_loc);

	Mat Ystretch(height, width, depth);

	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			Ystretch.at<float>(i,j)=(Y.at<float>(i,j)-min)*1.0/(max-min);
			if(Ystretch.at<float>(i,j)>1.0) Ystretch.at<float>(i,j)=1.0;
			if(Ystretch.at<float>(i,j)<0.0) Ystretch.at<float>(i,j)=0.0;
		}
	}

	Mat new_planes[] = {x,y,Ystretch};
	Mat sxyY;
	merge(new_planes, 3, sxyY);

	sxyY.copyTo(stretchxyY);

return void();
}

//Function takes Luv Mat object reference and window coordinates (w1,w2,h1,h2)
//and updates equLuv Mat object reference with histogram equalized [0-100] L values
//using L values from window coordinates
void LequLuv(const Mat& Luv, Mat& equLuv, double w1, double w2, double h1, double h2){
	int width,height,inputType, depth;
	Point min_loc, max_loc;
	double min,max;

	width=Luv.cols;
	height=Luv.rows;
	inputType=Luv.type();
	depth=CV_32FC1;

	if(inputType!=CV_32FC3){
		cout << "WARNING: Input Luv image type is not CV_32FC3." << endl;
	}

	Mat L(height, width, depth);
	Mat u(height, width, depth);
	Mat v(height, width, depth);

	vector<Mat> Luv_planes;
	split(Luv, Luv_planes);
	L = Luv_planes[0];
	u = Luv_planes[1];
	v = Luv_planes[2];

	//Pixel coordinates for the height and width box corners
	int ih1= (int) (h1*(height-1));
	int ih2= (int) (h2*(height-1));
	int iw1= (int) (w1*(height-1));
	int iw2= (int) (w2*(height-1));
	
	//Size of the box is coordinates +1
	int height2=(ih2-ih1)+1;
	int width2=(iw2-iw1)+1;

	//Pull out window L values
	Mat Ltemp(height2, width2, depth);
	for(int i=0; i<height2; i++)
		for(int j=0; j<width2; j++){
			Ltemp.at<float>(i,j)=L.at<float>(i+ih1,j+iw1);
		}

	//Discretize L in window
	Mat Lbyte(height2, width2, CV_8UC1);
	for(int i=0; i<height2; i++)
		for(int j=0; j<width2; j++){
			Lbyte.at<uchar>(i,j)=(uchar)floor(Ltemp.at<float>(i,j)+0.5);
		}

	cout << "Through discretization." << endl;
	minMaxLoc(Lbyte, &min, &max, &min_loc, &max_loc);

	// Compute the histogram
	int hist[101];
	for(int k = 0 ; k < 101 ; k++) hist[k] = 0;
	cout << "Hist allocated." << endl;

	for(int i = 0 ; i < height2 ; i++)
		for(int j = 0 ; j < width2 ; j++){
			hist[(int)Lbyte.at<uchar>(i,j)]++;
		}

	cout << "Histogram computed." << endl;
	
	//Print the histogram
	for(int i=0 ; i<101 ; i++)
		cout << hist[i] << " ";

	//Compute the sum_hist
	int accum=0;
	int sum_hist[101];
	for(int i=0 ; i<101 ; i++){
		accum+=hist[i];
		sum_hist[i]=accum;
	}
	cout << endl << "Sum Hist computed." << endl;
	for(int i=0 ; i<101 ; i++)
		cout << sum_hist[i] << " ";

	//Create the mapping
	int pix_map[101];
	pix_map[0]=(int)floor( ((0+sum_hist[0])/2.0)*(100.0/(width2*height2)) );
	for(int i=1 ; i<101 ; i++){
		pix_map[i]=(int)floor( ((sum_hist[i-1]+sum_hist[i])/2.0)*(100.0/(width2*height2)) );
	}
	cout << endl << "Pix map computed." << endl;

	for(int j=0 ; j<101 ; j++)
		cout << pix_map[j] << " ";

	Mat Lequ(height, width, depth);

	cout << endl << "Lequ allocated." << endl;

	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			Lequ.at<float>(i,j)=(float)pix_map[(int)floor(L.at<float>(i,j))];
		}
	}

	cout << "Lequ computed." << endl;

	Mat new_planes[] = {Lequ,u,v};
	Mat newLuv(height, width, CV_32FC3);
	merge(new_planes, 3, newLuv);

	newLuv.copyTo(equLuv);
	cout << "End of function." << endl;
return void();
}
