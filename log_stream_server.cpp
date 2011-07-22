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

void send_img(int socket, IplImage *img)
{
	vector<uchar> buff;

	vector<int> param = vector<int>(2);
        param[0] = CV_IMWRITE_JPEG_QUALITY;
        param[1] = 95;

	imencode(".jpg", img, buff, param);		

	char len[10];
	sprintf(len, "%.8d", buff.size());
	
	// printf("%s \n", len);
	
	/* sending length */
	send(socket, len, strlen(len), 0);
	
	/* send the image */
	send(socket, &buff[0], buff.size(), 0);
	buff.clear();

}

int main(int argc, char** argv)
{
	//cvNamedWindow("img", CV_WINDOW_AUTOSIZE);
	
	CvCapture* camera = cvCaptureFromCAM(0);
	//cvSetCaptureProperty( camera, CV_CAP_PROP_FRAME_WIDTH, 160);
	//cvSetCaptureProperty( camera, CV_CAP_PROP_FRAME_HEIGHT, 120);

        struct sockaddr_in server;	


        if ((serversock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {             
		quit("socket() failed", 1);
	}    	

	/* setup server's IP and port */                                        
	memset(&server, 0, sizeof(server));                                     
	server.sin_family = AF_INET;                                            
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = INADDR_ANY;                                    
                       

	if (bind(serversock, (struct sockaddr *)&server, sizeof(server)) == -1) {     
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

		/* read command for image sending */
		char recvdata[128];
		int bytes = recv(clientsock, recvdata, 128, 0);
		
		printf("%d\n", bytes);
		
		if (bytes == 0){
			printf("Connection should be closed now\n");
			close(clientsock);

			if ((clientsock = accept(serversock, NULL, NULL)) == -1) {              
				quit("accept() failed", 1);                                     
			}        		
		}

		if (strcmp(recvdata, "img") == 0){
			send_img(clientsock, img);
		} else if (strcmp(recvdata, "q") == 0) {
			printf("Connection should be closed now\n");
			close(clientsock);

			if ((clientsock = accept(serversock, NULL, NULL)) == -1) {              
				quit("accept() failed", 1);                                     
			}        		
		}
	}
	
	printf("This should not be printed\n");

	close(serversock);
	close(clientsock);
		


}

void quit(char* msg, int retval)
{
	fprintf(stderr, "%s\n", msg);

	exit(retval);
}

