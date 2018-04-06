#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void menu(){
  cout << "\nPressione a tecla para ativar o filtro: \n"
	"a - Calcular modulo\n"
  "m - Media\n"
  "g - Gaussiano\n"
  "v - Vertical\n"
	"h - Horizontal\n"
  "l - Laplaciano\n"
  "x - Laplaciano do gaussiano\n"
  "s - Sem Filtro\n"
	"esc - Sair\n";
}

int main(int argvc, char** argv){
  VideoCapture video;
  float media[] = {1,1,1,
				   1,1,1,
				   1,1,1};
  float gauss[] = {1,2,1,
				   2,4,2,
				   1,2,1};
  float horizontal[]={-1,0,1,
					  -2,0,2,
					  -1,0,1};
  float vertical[]={-1,-2,-1,
					0,0,0,
					1,2,1};
  float laplacian[]={0,-1,0,
					 -1,4,-1,
					 0,-1,0};
  float sem[]={0,0,0,
           0,1,0,
           0,0,0};

  Mat cap, frame, frame32f, frameFiltered;
  Mat mask(3,3,CV_32F), mask1;
  Mat result, result1;

  int absolut, laplgauss = 0;
  char key;
  
  video.open(0); 
  if(!video.isOpened()) 
    return -1;

  namedWindow("filtroespacial",1);

  mask = Mat(3, 3, CV_32F, media); 
  scaleAdd(mask, 1/9.0, Mat::zeros(3,3,CV_32F), mask1);
  swap(mask, mask1);
  absolut=1; // calcs abs of the image

  menu();
  for(;;){
    video >> cap; 
    cvtColor(cap, frame, CV_BGR2GRAY);
    flip(frame, frame, 1);
    imshow("original", frame);

    frame.convertTo(frame32f, CV_32F);

    if (laplgauss){
      mask = Mat(3, 3, CV_32F, gauss);
      scaleAdd(mask, 1/16.0, Mat::zeros(3,3,CV_32F), mask1);
      mask = mask1;
      filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(1,1), 0);
      mask = Mat(3, 3, CV_32F, laplacian);
      filter2D(frameFiltered, frame32f, frame32f.depth(), mask, Point(1,1), 0);
      frameFiltered = frame32f;
      
    } else {
      filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(1,1), 0);
    }

    if(absolut){
      frameFiltered=abs(frameFiltered);
    }

    frameFiltered.convertTo(result, CV_8U);
    imshow("filtroespacial", result);

    key = (char) waitKey(10);
    if( key == 27 ) break; // esc pressed!

    switch(key){
    case 'a':
	  menu();
      absolut=!absolut;
      break;

    case 'm':
	  menu();
      mask = Mat(3, 3, CV_32F, media);
      scaleAdd(mask, 1/9.0, Mat::zeros(3,3,CV_32F), mask1);
      mask = mask1;
      laplgauss = 0;
      cout << "Filtro da Media!\n";
      break;

    case 'g':
	  menu();
      mask = Mat(3, 3, CV_32F, gauss);
      scaleAdd(mask, 1/16.0, Mat::zeros(3,3,CV_32F), mask1);
      mask = mask1;      
      laplgauss = 0;
      cout << "Filtro Gaussiano!\n";
      break;

    case 'h':
	  menu();
      mask = Mat(3, 3, CV_32F, horizontal);     
      laplgauss = 0;
      cout << "Filtro da Borda Horizontal!\n";
      break;

    case 'v':
	  menu();
      mask = Mat(3, 3, CV_32F, vertical);  
      laplgauss = 0;
      cout << "Filtro da Borda Vertical!\n";
      break;

    case 'l':
	  menu();
      mask = Mat(3, 3, CV_32F, laplacian);
      laplgauss = 0;
      cout << "Filtro Laplaciano!\n";
      break;

    case 's':
    menu();
      mask = Mat(3, 3, CV_32F, sem);
      laplgauss = 0;
      cout << "#nofilter\n";
      break;

    case 'x':
    menu();
      laplgauss = 1;
      cout << "Filtro Laplaciano do Gaussiano!\n";
      break;

    default:
      break;
    }
  }
  return 0;
}
