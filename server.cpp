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

int main(int argc, const char * argv[])
{
  int sock;
  struct sockaddr_in addr;
  double weight = 320;
  double height = 240;
  char windowName[] = "Server Camera";
  cv::Mat image = cv::Mat(weight, height, CV_8UC3);
  int receiveSize = 8000;
  char buff[receiveSize];   
  int c;
  int received;
  vector<uchar> ibuff;
  
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(9000);
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_len = sizeof(addr);
       
  if( (::bind(sock, (struct sockaddr *)&addr, sizeof(addr)))<0){
    perror("socket");
    exit(EXIT_FAILURE);
  }
   
  cvNamedWindow( windowName, CV_WINDOW_AUTOSIZE );
  memset(buff, 0, 5);
  
  while(1){
    printf("Recieved\n");
    memset(buff, 0, receiveSize);
    received = recvfrom(sock, buff, receiveSize, 0, NULL,NULL);
       
    if(received != -1)
    {
      for(int i = 0; i < sizeof(buff); i++)
      {
        ibuff.push_back((uchar)buff[i]);
      }
            
      image = imdecode( Mat(ibuff), CV_LOAD_IMAGE_COLOR);
      cv::imshow(windowName, image);
      ibuff.clear();
    } else
      {
        perror("sock");
      }
        
    c = cvWaitKey (1);
    if (c == '\x1b')
      break;
  }

  close(sock);
  return 0;
}