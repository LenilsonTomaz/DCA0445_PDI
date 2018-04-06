#include <iostream>
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

int main(int argc, char** argv){

  Mat image;
  int width, height;
  Vec3b aux;
  // para pegar a imagem na pasta "images"
  string path = "/home/dan/opencv-3.4.1/scratch/images/";
  path.append(argv[1]);

  image= imread(path,IMREAD_COLOR);

  width=image.size().width;
  height=image.size().height;
	

  if(!image.data){
  	cout << "Erro ao abrir imagem" << endl;
  } else {

    	for(int i=0;i<height/2;i++){
     	 for(int j=0;j<width/2;j++){
	  aux = image.at<Vec3b>(i,j);
       	  image.at<Vec3b>(i,j) = image.at<Vec3b>(i+height/2,j+width/2);
	 image.at<Vec3b>(i+height/2,j+width/2) = aux;	
     	}
   	 }

	for(int i=height/2;i<height;i++){
     	 for(int j=0;j<width/2;j++){
	  aux = image.at<Vec3b>(i,j);
       	  image.at<Vec3b>(i,j) = image.at<Vec3b>(i-height/2,j+width/2);
	 image.at<Vec3b>(i-height/2,j+width/2) = aux;	
     	}
   	 }
   
	namedWindow("Troca Regioes",WINDOW_NORMAL);
	imshow("Troca Regioes", image);
  imwrite("imagem_trocada.jpg",image);
	waitKey();

  }


return 0;
}
 

