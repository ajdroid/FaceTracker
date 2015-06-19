
#include <FaceTracker/Tracker.h>
#include <opencv/highgui.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <iomanip>

using namespace std;
//=============================================================================
CV_INLINE  int  cvRound( float value )
{
#if defined HAVE_LRINT || defined CV_ICC || defined __GNUC__
  return (int)lrint(value);
#else
  // while this is not IEEE754-compliant rounding, it's usually a good enough approximation
  return (int)(value + (value >= 0 ? 0.5f : -0.5f));
#endif
}
//=============================================================================
void Draw(cv::Mat &image,cv::Mat &shape,cv::Mat &con,cv::Mat &tri,cv::Mat &visi)
{
  int i,n = shape.rows/2; cv::Point p1,p2; cv::Scalar c;

  //draw points
  for(i = 0; i < n; i++){    
    if(visi.at<int>(i,0) == 0)continue;
    p1 = cv::Point(shape.at<double>(i,0),shape.at<double>(i+n,0));
    c = CV_RGB(0,255,0); cv::circle(image,p1,2,c);
    //cv::putText(image, std::to_string(i+1),p1,CV_FONT_HERSHEY_PLAIN,0.5,cv::Scalar::all(0));
  }

  //cout<<typeid(shape.at<double>(i,0)).name();

}
//=============================================================================
int set_param(char* ftFile,char* conFile,char* triFile,
	      bool &fcheck,double &scale,int &fpd)
{
  fcheck = false; scale = 1; fpd = -1;

  //new assignments
  strcpy(triFile,"model/face.tri");
  strcpy(conFile,"model/face.con");
  strcpy(ftFile,"model/face2.tracker");
  //fcheck = true;
  return 0;
}
//=============================================================================
int main(int argc, char** argv)
{

  //set parameters
  char ftFile[256],conFile[256],triFile[256];
  string name(argv[1]);
  name.erase(name.find("."));
  string name_mirror (name); name_mirror += "_mirror.vector";
  name+= ".vector";
  bool fcheck = false; double scale = 1; int fpd = -1; bool showfps = true;
  if(set_param(ftFile,conFile,triFile,fcheck,scale,fpd)<0)return 0;

  //set other tracking parameters
  std::vector<int> wSize1(1); wSize1[0] = 7;
  std::vector<int> wSize2(3); wSize2[0] = 11; wSize2[1] = 9; wSize2[2] = 7;
  int nIter = 5; double clamp=3,fTol=0.01; 
  FACETRACKER::Tracker model(ftFile);
  cv::Mat tri=FACETRACKER::IO::LoadTri(triFile);
  cv::Mat con=FACETRACKER::IO::LoadCon(conFile);
  
  //initialize camera and display window
  cv::Mat tmp,frame,gray,im, mi, graymirr; char sss[256]; std::string text;


  //loop until quit (i.e user presses quit)
  bool failed = true;
				//grab image, resize and flip

				frame=cv::imread(argv[1],CV_LOAD_IMAGE_COLOR);
				if (frame.data==0)
				{
					cout<<"\nLol hui gawa reading "<<argv[1]; 
				}
				
				if(scale == 1)im = frame;
				else cv::resize(frame,im,cv::Size(scale*frame.cols,scale*frame.rows));
				cv::flip(im,mi,1); cv::cvtColor(im,gray,CV_BGR2GRAY);
				cv::cvtColor(mi,graymirr,CV_BGR2GRAY);
				//SimplestCB(im,im,1);

//=============	 track this image
				std::vector<int> wSize; if(failed)wSize = wSize2; else wSize = wSize1;
				;
				if(model.Track(gray,wSize,fpd,nIter,clamp,fTol,fcheck) == 0)
				{
				  int idx = model._clm.GetViewIdx(); failed = false;
				  Draw(im,model._shape,con,tri,model._clm._visi[idx]);
				  //put to file
				  int n = model._shape.rows/2;
				  fstream file;
				      file.open(name.c_str(), ios_base::out);
				      for(int i = 0; i < n; i++)
					  file <<"   "<< std::fixed << std::setprecision(8)<< model._shape.at<double>(i,0)<<"   "<< model._shape.at<double>(i+n,0)<<endl;
				      file.close();
				      
				  model.Track(graymirr,wSize,fpd,nIter,clamp,fTol,fcheck);
				  Draw(mi,model._shape,con,tri,model._clm._visi[idx]);
				
				      file.open(name_mirror.c_str(), ios_base::out);
				      for(int i = 0; i < n; i++)
					  file <<"   "<< std::fixed << std::setprecision(8)<< model._shape.at<double>(i,0)<<"   "<< model._shape.at<double>(i+n,0)<<endl;
				      file.close();
			  


				}
				else
				{
				  if(showfps){cv::Mat R(im,cvRect(0,0,150,50)); R = cv::Scalar(0,0,255);}
				  model.FrameReset(); failed = true;
				}


  	  	  return 0;
}
//==================================================================================================================================================================================
