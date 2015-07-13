#ifndef rectshow
#define rectshow 0
#endif

#include <FaceTracker/Tracker.h>
#include <opencv/highgui.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <FaceTracker/vecthelp.h>
//#include <svm.h>

using namespace cv;
using namespace std;

void SimplestCB(Mat& in, Mat& out, float percent) {
	assert(in.channels() == 3);
	assert(percent > 0 && percent < 100);

	float half_percent = percent / 200.0f;

	vector<Mat> tmpsplit;
	split(in, tmpsplit);
	for (int i = 0; i < 3; i++) {
		//find the low and high precentile values (based on the input percentile)
		Mat flat;
		tmpsplit[i].reshape(1, 1).copyTo(flat);
		cv::sort(flat, flat, CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);
		int lowval = flat.at<uchar>(
				cvFloor(((float) flat.cols) * half_percent));
		int highval = flat.at<uchar>(
				cvCeil(((float) flat.cols) * (1.0 - half_percent)));


		//saturate below the low percentile and above the high percentile
		tmpsplit[i].setTo(lowval, tmpsplit[i] < lowval);
		tmpsplit[i].setTo(highval, tmpsplit[i] > highval);

		//scale the channel
		normalize(tmpsplit[i], tmpsplit[i], 0, 255, NORM_MINMAX);
	}
	merge(tmpsplit, out);
}

CV_INLINE int cvRound(float value) {
#if defined HAVE_LRINT || defined CV_ICC || defined __GNUC__
	return (int) lrint(value);
#else
	// while this is not IEEE754-compliant rounding, it's usually a good enough approximation
	return (int)(value + (value >= 0 ? 0.5f : -0.5f));
#endif
}
//=============================================================================
void Draw(cv::Mat &image, cv::Mat &shape, cv::Mat &con, cv::Mat &tri,
		cv::Mat &visi, cv::Mat &rshape) {
	int i, n = shape.rows / 2;
	cv::Point p1, p2;
	cv::Scalar c;

	// draw rshape
	for (i = 0; i < n; i++) {
		if (visi.at<int>(i, 0) == 0)
			continue;
		p1 = cv::Point(rshape.at<double>(i, 0), rshape.at<double>(i + n, 0));
		c = CV_RGB(0,255,0);
		cv::circle(image, p1, 2, c);
		//cv::putText(image, std::to_string(i+1),p1,CV_FONT_HERSHEY_PLAIN,0.5,cv::Scalar::all(0));
	}

	//draw points
	for (i = 0; i < n; i++) {
		if (visi.at<int>(i, 0) == 0)
			continue;
		p1 = cv::Point(shape.at<double>(i, 0), shape.at<double>(i + n, 0));
		c = CV_RGB(255,0,0);
		cv::circle(image, p1, 2, c);
		//cv::putText(image, std::to_string(i+1),p1,CV_FONT_HERSHEY_PLAIN,0.5,cv::Scalar::all(0));
	}
	return;

}
//=============================================================================
int set_param(char* ftFile, char* conFile, char* triFile, bool &fcheck,

		double &scale, int &fpd) {
	fcheck = false;
	scale = 1;
	fpd = -1;

	//new assignments
	strcpy(triFile, "model/face.tri");
	strcpy(conFile, "model/face.con");
	strcpy(ftFile, "model/face2.tracker");
	//fcheck = true;
	return 0;
}
//=============================================================================
void DrawHP (cv::Mat &im, const cv::Mat &pose)
{
		double pitch = pose.at<double>(1, 0);
		double yaw = pose.at<double>(2, 0);
		double roll = pose.at<double>(3, 0);
		char sss[256];
		sprintf(sss, "pitch:%.3f", pitch * 180 / 3.14);
		cv::putText(im, sss, cv::Point(10, 50), CV_FONT_HERSHEY_DUPLEX, 0.5,
				CV_RGB(0,255,0), 2);
		sprintf(sss, "yaw:%.3f", yaw * 180 / 3.14);
		cv::putText(im, sss, cv::Point(10, 80), CV_FONT_HERSHEY_DUPLEX, 0.5,
				CV_RGB(0,255,0), 2);
		sprintf(sss, "roll:%.3f", roll * 180 / 3.14);
		cv::putText(im, sss, cv::Point(10, 110), CV_FONT_HERSHEY_DUPLEX, 0.5,
				CV_RGB(0,255,0), 2);
}
//=============================================================================
int main() {
	//set parameters
	char ftFile[256], conFile[256], triFile[256];
	bool fcheck = false;
	double scale = 1;
	int fpd = -1;
	bool showfps = true;
	if (set_param(ftFile, conFile, triFile, fcheck, scale, fpd) < 0)
		return 0;

	//set other tracking parameters
	std::vector<int> wSize1(1);
	wSize1[0] = 7;
	std::vector<int> wSize2(3);
	wSize2[0] = 11;
	wSize2[1] = 9;
	wSize2[2] = 7;
	int nIter = 5;
	double clamp = 3, fTol = 0.01;
	FACETRACKER::Tracker model(ftFile);
	cv::Mat tri; //=FACETRACKER::IO::LoadTri(triFile);
	cv::Mat con; //=FACETRACKER::IO::LoadCon(conFile);
	std::string impath =
			"/home/abhijat/Downloads/Intern/git/CUROP---Face-Tracker/svm_training_multi_18/";
	std::string imfile;


	//initialize camera and display window
	cv::Mat tmp, frame, gray, im;
	double fps = 0;
	char sss[256];
	std::string text;
	cv::VideoCapture camera(-1);
	if (!camera.isOpened())
		return -1;
	int64 t1, t0 = cvGetTickCount();
	int fnum = 0;

	std::cout << "Hot keys: " << std::endl << "\t q - Quit" << std::endl
			<< "\t d - Re detect" << std::endl;

	const int eigsize = 18;
	std::vector<double> test, feat, mu, sigma, eigv[eigsize];
	file2eig("pca_archive_wt.txt",eigv, eigsize);
	file2vect("pca_archive_mu.txt", mu);
	file2vect("pca_archive_sigma.txt", sigma);

	//loop until quit (i.e user presses quit)
	bool failed = true;
	while (1) {
		//grab image, resize and flip


		camera>>frame;

		imfile = impath + "vector.png";
		//frame = imread(imfile.c_str(), CV_LOAD_IMAGE_COLOR);

		if (scale == 1)
			im = frame;
		else
			cv::resize(frame, im, cv::Size(scale * frame.cols, scale * frame.rows));
		//scale = 0.25;
		cv::resize(frame, im, cv::Size(scale * frame.cols, scale * frame.rows));
		cv::flip(im,im,1);
		cv::cvtColor(im, gray, CV_BGR2GRAY);
		//equalizeHist(gray,gray);
		SimplestCB(im,im,1);
//============================== set face equalization region extremities
		cv::Rect facereg;
		setEqlim(model._shape, im.rows, im.cols, facereg);

		Mat ROI;
		try {
			ROI = gray(facereg);
		} catch (...) {
			//continue;
		}
		cv::rectangle(gray, facereg, cv::Scalar(0, 0, 0));
		//cv::rectangle(gray, model._rect, cv::Scalar(255,255,255));
		cv::equalizeHist(ROI, ROI);


//=============	 track this image
		std::vector<int> wSize;
		if (failed)
			wSize = wSize2;
		else
			wSize = wSize1;
		;

		if (model.Track(gray, wSize, fpd, nIter, clamp, fTol, fcheck) == 0) {
			int idx = model._clm.GetViewIdx();
			failed = false;
			Draw(im, model._shape, con, tri, model._clm._visi[idx],
					model._rshape);
			vect2test(model._shape, test);
			//printf("Size of the test vector = %f\n", test.front());
			pca_project(test, eigv, mu, sigma, eigsize, feat);

		} else {
			if (showfps) {
				cv::Mat R(im, cv::Rect(0, 0, 150, 50));
				R = cv::Scalar(0, 0, 255);
			}
			model.FrameReset();
			failed = true;
		}

#if rectshow
		cv::Point pTopLeft(im.cols/3, im.rows/4), pBottomRight(2*im.cols/3, 3*im.rows/4);
		cv::Rect R(pTopLeft, pBottomRight);
		cv::rectangle(im, R, cv::Scalar(0,0,0));
#endif

//============= draw framerate on display image
		if (fnum >= 9) {
			t1 = cvGetTickCount();
			fps = 10.0 / ((double(t1 - t0) / cvGetTickFrequency()) / 1e+6);
			t0 = t1;
			fnum = 0;
		} else
			fnum += 1;

		if (showfps) {
			sprintf(sss, "%f frames/sec", (fps));
			text = sss;
			cv::putText(im, sss, cv::Point(10, 20),
			CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(255,255,255));
		}

//========================== Draw head pose
		const Mat& pose = model._clm._pglobl;
		DrawHP(im, pose);

//======================= show image and check for user input

		imshow("Face Tracker", gray);
		imshow("Face Tracker1", im);
		int c = cvWaitKey(10);
		if (char(c) == 'q')
			break;
		else if (char(c) == 'd')
			model.FrameReset();
		else if (char(c) == 'c')
			imwrite(
					"/home/abhijat/Downloads/Intern/git/CUROP---Face-Tracker/svm_training/cap.png",
					im);
	}
	return 0;
}
//=============================================================================
