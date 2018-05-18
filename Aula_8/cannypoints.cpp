#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>


using namespace std;
using namespace cv;

#define STEP 12
#define JITTER 4
#define RAIO 11 // raio da base

#define RAIO2 2 // Raio dos circulos de borda


int top_slider = 10;
int top_slider_max = 200;

char TrackbarName[50];

Mat image, border, frame, points, points2, final;

vector<int> yrange;
vector<int> xrange;

int width, height, gray;
int x, y;

void pontilhar(){

  // sobrepoe a imagem pontilhada das bordas na imagem base
  points2 = points.clone();

  for (int i = 0; i < height; i++){
    for (int j = 0; j < width; j++){
      
      if (border.at<uchar>(i,j) != 0){
        gray = image.at<uchar>(i,j);

        circle(points2,cv::Point(j,i),
        RAIO2,CV_RGB(gray,gray,gray),
        -1,CV_AA);

      }

    }
  }

  imshow("final", points2);
  imwrite("pontilhada.jpg",points2);
}


// calcula os pontos de borda
void on_trackbar_canny(int, void*){
  Canny(image, border, top_slider, 3*top_slider);
  imshow("canny", border);
  imwrite("bordas.jpg",border);
  pontilhar();
}

int main(int argc, char**argv){

  srand(time(0));

  // string path = "/home/dan/opencv-3.4.1/scratch/images/";
  // path.append(argv[1]);

  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

  sprintf(TrackbarName,"TL");

  namedWindow("canny",WINDOW_KEEPRATIO);
  namedWindow("base",WINDOW_KEEPRATIO);
  namedWindow("final",WINDOW_KEEPRATIO);


  width=image.size().width;
  height=image.size().height;

  xrange.resize(height/STEP);
  yrange.resize(width/STEP);

  iota(xrange.begin(), xrange.end(), 0);
  iota(yrange.begin(), yrange.end(), 0);

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*STEP+STEP/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*STEP+STEP/2;
  }

  points = Mat(height, width, CV_8U, Scalar(255));

  random_shuffle(xrange.begin(), xrange.end());

  // cria a imagem pontilhada base
  for(auto i : xrange){
    random_shuffle(yrange.begin(), yrange.end());
    for(auto j : yrange){
      x = i+rand()%(2*JITTER)-JITTER+1;
      y = j+rand()%(2*JITTER)-JITTER+1;
      gray = image.at<uchar>(x,y);
      circle(points,
             cv::Point(y,x),
             RAIO,
             CV_RGB(gray,gray,gray),
             -1,
             CV_AA);
    }
  }

  imshow("base", points);
  imwrite("base.jpg",points);
  createTrackbar( TrackbarName, "canny",
                &top_slider,
                top_slider_max,
                on_trackbar_canny );
  on_trackbar_canny(top_slider, 0 );


  waitKey();

  return 0;
}