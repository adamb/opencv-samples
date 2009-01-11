
#include "highgui.h"

int main(int argvc, char** argv) {
	cvNamedWindow("Video", CV_WINDOW_AUTOSIZE);
	CvCapture* capture = cvCreateFileCapture(argv[1]);
	IplImage* frame;
	while(1){
		frame = cvQueryFrame(capture);
		if(!frame) break;
		cvShowImage("Video", frame);
		char c = cvWaitKey(33);
		if (c==27) break;
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("Video");
}
