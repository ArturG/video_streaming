#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main()
{
  int sock;
  int send_len;
  int c;
  struct sockaddr_in addr;
  double weight = 320;
  double height = 240;
  CvCapture *capture = 0;
  IplImage *frame = 0;
  cv::Mat Mimg;
  vector<uchar> ibuff;
  vector<int> param = vector<int>(2);
  char windowName[] = "Client Camera";
  int sendSize = 8000;
  char buff[sendSize];
  
  sock = socket(AF_INET, SOCK_DGRAM, 0);
    
  addr.sin_family = AF_INET;
  addr.sin_port = htons(9000);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
  
  capture = cvCreateCameraCapture(0);
  cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH, weight);
  cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT, height);
  cvNamedWindow (windowName, CV_WINDOW_AUTOSIZE);
    
    
  //jpeg compression
    
  frame = cvQueryFrame (capture);
  Mimg=frame;  
  param[0] = CV_IMWRITE_JPEG_QUALITY;
  param[1] = 55; //default(95) 0-100

  imencode(".jpg", Mimg, ibuff, param);
  //cout<<"coded file size(jpg)"<<ibuff.size()<<endl;
    
  while (1) {
    frame = cvQueryFrame (capture);
    Mimg=frame;
    if (ibuff.size() < sendSize)
    {
      imencode(".jpg", Mimg, ibuff, param);
      for (int i = 0; i < ibuff.size(); i++)
      {
        buff[i]=ibuff[i];
      }
    }
        
    send_len = sendto(sock, buff, sendSize,0, (struct sockaddr *)&addr, sizeof(addr));
    
    if (send_len==-1)
    {
      perror("socket");
    } else
      {
        printf("%d \n",send_len);
      }

    imshow(windowName, Mimg);
    c = cvWaitKey (1);
    if (c == '\x1b')
    {
      break;
    }
  }
         
  cvDestroyWindow(windowName);
  close(sock);
  return 0;
}