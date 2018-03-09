#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>  //Library for close
#include <sys/time.h>

#define MAX_MSG 3

#define BUFFERSIZE 1024
#define SERVERPORT 5000
#define SERVERADDR "127.0.0.1"

int main(int argc, char **argv)
{
    struct timeval t1, t2;
    long delay_ = 0;
    int msg_count = 0;
    
    int sock, bytes_sent, bytes_recv, conn_status;
    struct sockaddr_in s_server;
    char send_buf[BUFFERSIZE], recv_buf[BUFFERSIZE];

	// Creating Socket
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock<0)
	{
		printf("Socket creation failed. \n");
		return 1;
	}
	else
	{
		printf("Socket creation successful with descriptor %d\n", sock);
	}

	s_server.sin_family = AF_INET;
	s_server.sin_port = htons(SERVERPORT);
	inet_aton(SERVERADDR, &s_server.sin_addr);

	conn_status = connect(sock, (struct sockaddr*)&s_server, sizeof(s_server));
	if(conn_status == -1)
	{
		printf("Connection to server failed.\n");
		close(sock);
		return 1;
	}
	else
	{
		printf("Connected to Server.\n");
	}

	while(msg_count < MAX_MSG)
	  {
	    printf("Enter the message\n");
	    scanf("%s", send_buf);

	    gettimeofday(&t1, NULL);
	    bytes_sent = send(sock, send_buf, strlen(send_buf), 0);
	    printf("%d bytes sent : %s\n", bytes_sent, send_buf);

	    msg_count++;

	    do
	      {
		bytes_recv = recv(sock, recv_buf, sizeof(recv_buf), 0);
	      } while(!bytes_recv);
		
	    if(bytes_recv)
	      {
		recv_buf[bytes_recv] = '\0';
		printf("%d Bytes Received: %s\n",bytes_recv, recv_buf);
	      }
	    gettimeofday(&t2, NULL);

	    delay_ += t2.tv_sec*1000000 + t2.tv_usec - t1.tv_sec*1000000 - t1.tv_usec;

	  }
	// Reading message from keyboard and send
	

	/* int i, packet_num = 6; */
	/* char *sentence[] = {" ", "Start"," My", " name", " is", " Ansuman", " end"}; */
	/* strcpy(sentence[0], argv[1]); */
	
	/* for (i =0; i<packet_num; i++) */
	/*   { */
	/*     strcpy(send_buf, sentence[i]); */
	/*     bytes_sent = send(sock, send_buf, strlen(send_buf), 0); */
	/*     printf("%d bytes sent : %s\n", bytes_sent, send_buf); */
	/*     sleep(1); */
	/*   } */

	printf("Done sending %d messages\n", msg_count);
	printf("Average Round Trip Time = %f\n", delay_*1.0/msg_count);
	

	close(sock);

	return 0;
}
