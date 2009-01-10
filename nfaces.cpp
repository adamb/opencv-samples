////////////////////////////////////////////////////////////////////////
//
// nfaces.cpp
//
// This is a simple program to find faces and print out how many it found.
//
////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>


int nfaces( IplImage* img, CvHaarClassifierCascade* cascade )
{
    static CvScalar colors[] = 
    {
        {{0,0,255}},
        {{0,128,255}},
        {{0,255,255}},
        {{0,255,0}},
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}}
    };

	static CvMemStorage* storage = cvCreateMemStorage(0);
  
    double scale = 1.3;
    IplImage* gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
    IplImage* small_img = cvCreateImage( cvSize( cvRound (img->width/scale),
                         cvRound (img->height/scale)),
                     8, 1 );
    int i;

    cvCvtColor( img, gray, CV_BGR2GRAY );
    cvResize( gray, small_img, CV_INTER_LINEAR );
    cvEqualizeHist( small_img, small_img );
    cvClearMemStorage( storage );


    CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage,
                                        1.1, 2, 0/*CV_HAAR_DO_CANNY_PRUNING*/,
                                        cvSize(30, 30) );

    return (faces ? faces->total : 0);
	
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );
}

int main(int argc, char *argv[])
{
  IplImage* image = 0; 

  int optlen = strlen("--cascade=");
  const char* input_name;
  const char* cascade_name;

  // deal with the argument list
  if( argc > 1 && strncmp( argv[1], "--cascade=", optlen ) == 0 ) {
      cascade_name = argv[1] + optlen;
      input_name = argc > 2 ? argv[2] : 0;
  }	
  else {	  // use a default cascade if none provided
      cascade_name = "/opt/local/var/macports/software/opencv/1.0.0_0/opt/local/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
      input_name = argc > 1 ? argv[1] : 0;
  }

  if( !input_name ) {
      fprintf( stderr, "ERROR: Need a valid filename\n" );
      fprintf( stderr,
      "Usage: facedetect --cascade=\"<cascade_path>\" [filename]\n" );
      return -1;
  }

  static CvHaarClassifierCascade* cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );
  
  if( !cascade ) {
      fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
      fprintf( stderr,
      "Usage: facedetect --cascade=\"<cascade_path>\" [filename]\n" );
      return -1;
  }

  
  // load the image
  image = cvLoadImage( input_name , 1); 
  if (!image) {
	printf("Could not load image file: %s\n", input_name);
	return -1;
  }
	

  printf("%d\n",nfaces(image, cascade));

  // release the image
  cvReleaseImage(&image );
  return 0;

}
