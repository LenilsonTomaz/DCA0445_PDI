#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace cv;
using namespace std;

double alpha, focus, posicao;
double d;
int width, height;

int alpha_slider = 20;
int alpha_slider_max = 99;

int focus_slider = 30;
int focus_slider_max = 100;

int posicao_slider = 50;
int posicao_slider_max = 100;

char TrackbarName[50];

Mat image,borrada,resultado1,resultado2,peso1,peso2,final,mask1;
Mat mask(3,3,CV_32F);

void faz_tudo(){

  for (int i=0;i<height;i++){

    alpha = 255*(0.5)*(tanh((i - posicao + focus)/d)-tanh((i - posicao - focus)/d));
    //cout << alpha << " ";
    int A = alpha;
    int B = 255 - alpha;
   // cout << A << " ";
    //cout << B << " ";

    for (int j=0;j<width;j++){
      peso1.at<Vec3b>(i,j) = Vec3b(A,A,A);
      peso2.at<Vec3b>(i,j) = Vec3b(B,B,B);
    }
  }

  multiply(image,peso1,resultado1,1/255.0,-1);
  multiply(borrada,peso2,resultado2,1/255.0,-1);

  final = resultado1 + resultado2;

  imshow("tiltshift",final);
  imwrite("tiltshifted.jpg",final);
  
 // imshow("tiltshift2",peso1);

}

void on_trackbar_alpha(int, void*){
 d = (double) alpha_slider + 1;
 faz_tudo();
}

void on_trackbar_focus(int, void*){
  focus = (double) focus_slider/focus_slider_max;
  int limit = focus*height/2;
  focus = limit;
  //cout << height << endl;
  //cout << focus_slider << endl;
  //cout << focus << endl;
  faz_tudo();
}

void on_trackbar_posicao(int, void*){
  posicao = (double) posicao_slider/posicao_slider_max;
  int limit = posicao*height;
  posicao = limit;
  faz_tudo();

}

int main(int argvc, char** argv){
 
  float media[] = {1,1,1,
           1,1,1,
           1,1,1};

  string path = "/home/dan/opencv-3.4.1/scratch/images/";
  path.append(argv[1]);
  image= imread(path,IMREAD_COLOR);

  width=image.size().width;
  height=image.size().height;

  namedWindow("tiltshift",WINDOW_KEEPRATIO);

  mask = Mat(3, 3, CV_32F, media); 
  scaleAdd(mask, 1/9.0, Mat::zeros(3,3,CV_32F), mask1);
  mask = mask1;

  peso1.create(height,width,CV_8UC(3));
  peso2.create(height,width,CV_8UC(3));
  final.create(height,width,CV_8UC(3));
  resultado1.create(height,width,CV_8UC(3));
  borrada.create(height,width,CV_8UC(3));
  resultado2.create(height,width,CV_8UC(3));

  filter2D(image, borrada, image.depth(), mask, Point(1,1), 0);
  filter2D(borrada, borrada, image.depth(), mask, Point(1,1), 0);
  filter2D(borrada, borrada, image.depth(), mask, Point(1,1), 0);

 // imshow("aa",borrada);
 // imshow("bb",image);

  sprintf(TrackbarName, "Alpha x %d", alpha_slider_max);
  createTrackbar( TrackbarName, "tiltshift",
				  &alpha_slider,
				  alpha_slider_max,
				  on_trackbar_alpha );
  on_trackbar_alpha(alpha_slider, 0 );
  
  sprintf(TrackbarName, "Foco x %d", focus_slider_max);
  createTrackbar( TrackbarName, "tiltshift",
				  &focus_slider,
				  focus_slider_max,
				  on_trackbar_focus);
  on_trackbar_focus(focus_slider, 0 );

  sprintf(TrackbarName, "Centro do Foco x %d", posicao_slider_max);
  createTrackbar(TrackbarName, "tiltshift",
          &posicao_slider,
          posicao_slider_max,
          on_trackbar_posicao);
  on_trackbar_posicao(posicao_slider, 0 );

  waitKey(0);
  return 0;
}
