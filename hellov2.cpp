
#include "highgui.h"

int main(int argvc, char** argv) {
	cvNamedWindow("Video1", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Video2", CV_WINDOW_AUTOSIZE);

	CvCapture* capture = cvCreateFileCapture(argv[1]);
	IplImage* frame;
	while(1){
		frame = cvQueryFrame(capture);
		if(!frame) break;
		cvShowImage("Video1", frame);
		cvShowImage("Video2", frame);
		char c = cvWaitKey(33);
		if (c==27) break;
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("Video1");
	cvDestroyWindow("Video2");	
}
