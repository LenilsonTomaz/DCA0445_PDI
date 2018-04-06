#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image, mask;
  int width, height;
  int nobjects=0;
  int nburacos=0;
 
  CvPoint p;
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  
  if(!image.data){
    cout << "imagem nao carregou corretamente\n";
    return(-1);
  }
  width=image.size().width;
  height=image.size().height;
  
  p.x=0;
  p.y=0;

  image.row(0) = 255;
  image.row(height-1) = 255;	
  image.col(width-1) = 255;
  image.col(0) = 255;
  floodFill(image,p,0);
  floodFill(image,p,254);

  // busca objetos com buracos presentes
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      

	if(image.at<uchar>(i,j) == 255){
	// achou um objeto
          nobjects++;
	       p.x=j;
          p.y=i;
	  floodFill(image,p,nobjects+1);
 	}


	if((image.at<uchar>(i,j) == 0)){
	// achou um buraco
          nburacos++;
	        p.x=j;
          p.y=i;
	  floodFill(image,p,nburacos);		
 	}
         
     }
   }

  cout << nburacos << endl;
  cout << nobjects << endl;

  imshow("image", image);
  imwrite("labeling.png", image);
  waitKey();
  return 0;
}
