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


int main(int argc, char *argv[])
{
  IplImage* image = 0; 

  int optlen = strlen("--cascade=");
  const char* input_name;

  if( argc > 1 && strncmp( argv[1], "--cascade=", optlen ) == 0 )
  {
      cascade_name = argv[1] + optlen;
      input_name = argc > 2 ? argv[2] : 0;
  }	
  else
  {
      cascade_name = "/opt/local/var/macports/software/opencv/1.0.0_0/opt/local/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
      input_name = argc > 1 ? argv[1] : 0;
  }

  if( !input_name )
  {
      fprintf( stderr, "ERROR: Need a valid filename\n" );
      fprintf( stderr,
      "Usage: facedetect --cascade=\"<cascade_path>\" [filename]\n" );
      return -1;
  }

  cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );
  
  if( !cascade )
  {
      fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
      fprintf( stderr,
      "Usage: facedetect --cascade=\"<cascade_path>\" [filename]\n" );
      return -1;
  }
  storage = cvCreateMemStorage(0);
  
  // Load the image
  image = cvLoadImage( input_name , 1); 
  if (!image) {
	printf("Could not load image file: %s\n", input_name);
	exit(0);
  }
	

  if( argc > 1 && strncmp( argv[1], "--cascade=", optlen ) == 0 )
  {
      cascade_name = argv[1] + optlen;
      input_name = argc > 2 ? argv[2] : 0;
  }

  if(argc<2){
    printf("Usage: nfaces <image-file-name>\n");
    exit(0);
  }

  // load an image  
  img=cvLoadImage(argv[1]);

  if(!img){
    printf("Could not load image file: %s\n",argv[1]);
    exit(0);
  }

  printf(nfaces(img));

  // release the image
  cvReleaseImage(&img );
  return 0;

}

void nfaces( IplImage* img )
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

    if( cascade )
    {
        double t = (double)cvGetTickCount();
        CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage,
                                            1.1, 2, 0/*CV_HAAR_DO_CANNY_PRUNING*/,
                                            cvSize(30, 30) );
        t = (double)cvGetTickCount() - t;
        printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );
        for( i = 0; i < (faces ? faces->total : 0); i++ )
        {
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
            CvPoint center;
            int radius;
            center.x = cvRound((r->x + r->width*0.5)*scale);
            center.y = cvRound((r->y + r->height*0.5)*scale);
            radius = cvRound((r->width + r->height)*0.25*scale);
            cvCircle( img, center, radius, colors[i%8], 3, 8, 0 );
        }
    }

    cvShowImage( "result", img );
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );
}