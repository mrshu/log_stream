#include <cv.h>
#include <highgui.h>
#include <cxcore.h>


#include <netinet/in.h>                                                         
#include <sys/socket.h>                                                         
#include <arpa/inet.h>  

#include <stdio.h>
#include <stdlib.h>

#include <iostream>


#define PORT 1212

IplImage *img, *imgThreshed; 

using namespace std;
using namespace cv;

void quit(char* msg, int retval);

int serversock, clientsock; 

int main(int argc, char** argv)
{
	//cvNamedWindow("img", CV_WINDOW_AUTOSIZE);
	
	CvCapture* camera = cvCaptureFromCAM(0);
	cvSetCaptureProperty( camera, CV_CAP_PROP_FRAME_WIDTH, 160);
	cvSetCaptureProperty( camera, CV_CAP_PROP_FRAME_HEIGHT, 120);

	vector<uchar> buff;

	
	/* sending file */
        struct sockaddr_in server;	

        if ((serversock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {             
		quit("socket() failed", 1);
	}    	

	/* setup server's IP and port */                                        
	memset(&server, 0, sizeof(server));                                     
	server.sin_family = AF_INET;                                            
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = INADDR_ANY;                                    
                       

	if (bind(serversock, (struct sockaddr *)&server, sizeof(struct
	sockaddr)) == -1) {     
		quit("bind() failed", 1);                                       
	}     
	

	/* wait for connection */                                               
	if (listen(serversock, 10) == -1) {                                     
		quit("listen() failed.", 1);                                    
	}                                                                       

	printf("Waiting for connection on port %d\n", PORT);
										
	/* accept a client */                                                   
	if ((clientsock = accept(serversock, NULL, NULL)) == -1) {              
		quit("accept() failed", 1);                                     
	}        
	


	while(1) {
		img = cvQueryFrame(camera);
		if (img == NULL)
			continue;
		
		vector<int> param = vector<int>(2);
        	param[0] = CV_IMWRITE_JPEG_QUALITY;
        	param[1] = 95;

		imencode(".jpg", img, buff, param);		
			
		uchar recvdata[128];
		/* read command for image sending */
		int bytes = recv(clientsock, recvdata, 128, 0);
		
		if (bytes > 1){
			/* sendign length */
			char len[20];
			sprintf(len, "%d:", buff.size());
			
			send(clientsock, len, strlen(len), 0);
			
			/* send the image */
			send(clientsock, &buff[0], buff.size(), 0);
			//printf("finished\n");
		}

		
		buff.clear();
	}

	close(serversock);
	close(clientsock);
		


}

void quit(char* msg, int retval)
{
	fprintf(stderr, "%s\n", msg);

	exit(retval);
}

