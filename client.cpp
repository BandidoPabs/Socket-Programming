#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>


using namespace std;

int main(){
	string hostname, nbhd;
	int pnum;
	
	cout << "Enter server hostname: ";
	cin >> hostname;
	
	cout << "\nEnter server port number: ";
	cin >> pnum;
	
	const char* chostname = hostname.c_str();
	
	
	
	// Initializing my socket
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];
	
	/*if (argc < 3){
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}*/
	
	portno = pnum;
	
	//Create a socket point
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sockfd < 0){
		perror("ERROR opening socket");
		exit(0);
	}
	
	server = gethostbyname(chostname);
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	
	// Now connect to the server
	if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0){
		perror("ERROR connecting");
		exit(1);
	}
	
	// Asking for a message from the user, message will be read by the server
	int i=0;
	while(1){
		cout << endl;
		
		printf("Enter a Houston neighborhood: ");
		if(i == 0){
			bzero(buffer,256);
			getchar();
			fgets(buffer,255,stdin);
		}
		else{
			bzero(buffer,256);
			fgets(buffer,255,stdin);
		}
		
		if(buffer[0] == '\n'){
			close(0);
			exit(0);
		}
		buffer[strlen(buffer)-1]='\0';
		string house = buffer;
	//Send message to the server
		
		n= write(sockfd, buffer, strlen(buffer));
	
		if(n < 0){
			perror("ERROR writing to socket");
			exit(1);
		}
		
		// Read server response
		bzero(buffer,256);
		n = read(sockfd, buffer, sizeof(buffer));
		if(buffer[0] == '-' && buffer[1] == '1'){
			cout << "The neighborhood is not in the table.";
		}
		else{
			cout << "The average price of the houses in " << house << " is " << buffer;
		}
	
		if (n < 0) {
			perror("ERROR reading from socket");
			exit(1);
		}
		//close(0);
		i++;
		//printf("%s\n",buffer);
	}
	
	

	
	return 0;
}