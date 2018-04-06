#include <iostream>
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

int main(int argc, char** argv){

  Mat image;
  int width, height;
  unsigned char aux;
  string path = "/home/dan/opencv-3.4.1/scratch/images/";
  path.append(argv[1]);

  image= imread(path,CV_LOAD_IMAGE_GRAYSCALE);

  width=image.size().width;
  height=image.size().height;
	

  if(!image.data){
  	cout << "Erro ao abrir imagem.png" << endl;
  } else {

    	for(int i=0;i<height/2;i++){
     	 for(int j=0;j<width/2;j++){
	  aux = image.at<uchar>(i,j);
       	  image.at<uchar>(i,j) = image.at<uchar>(i+height/2,j+width/2);
	 image.at<uchar>(i+height/2,j+width/2) = aux;	
     	}
   	 }

	for(int i=height/2;i<height;i++){
     	 for(int j=0;j<width/2;j++){
	  aux = image.at<uchar>(i,j);
       	  image.at<uchar>(i,j) = image.at<uchar>(i-height/2,j+width/2);
	 image.at<uchar>(i-height/2,j+width/2) = aux;	
     	}
   	 }
   
	namedWindow("Troca Regioes",WINDOW_AUTOSIZE);
	imshow("Troca Regioes", image);
	waitKey();

  }


return 0;
}
 

