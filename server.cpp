#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>


using namespace std;

//To hold info on the neighborhood
struct houseinfo {
	string neighborhood;
	string nprice;
};



int main(){
	string file, nbhd, nbhdprice;
	int portnum;
	struct houseinfo house;
	vector<struct houseinfo> htable;
	
	
	cout<< "Please enter the name of the file: ";
	cin >> file;
	
	ifstream myfile (file);
	
	if(myfile.is_open()){
		while(myfile.good()){
			getline(myfile,nbhd, ',');
			house.neighborhood = nbhd;
			myfile.ignore();
			getline(myfile,nbhdprice);
			house.nprice = nbhdprice;
			htable.push_back(house);
		}
		myfile.close();
	}
	
	/*for(unsigned int i=0;i<htable.size()-1;i++){
		cout << htable[i].nprice << endl;
	}*/
	
	cout << "\nEnter server port number: ";
	cin >> portnum;
	
	//creating the socket
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	
	
	//first call to socket() fucntion
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sockfd < 0){
		perror("ERROR opening socket");
		exit(1);
	}
	
	// Initialze socket structure
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = portnum;
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	
	//now bind the host address using bind() call
	
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
		perror("ERROR on binding");
		exit(1);
	}
	
	//listening for the clients, going to sleep mode and wating for incoming connection
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	
	// Accept actual connection from the client
	newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
	
	if(newsockfd < 0){
		perror("ERROR on accept");
		exit(1);
	}
	while(1){
	//If connection is established then start communication
		bzero(buffer,256);
		
		n = read(newsockfd,buffer,255);
		
		if(buffer[0] == '\n'){
			close(0);
			exit(0);
		}
		
		if(n < 0){
			perror("ERROR reading from socket");
			exit(1);
		}
		
		
	//Write a response to the client
	
		printf("Here is the message: %s \n",buffer);
		const char* cprice;
	//Searching for the price of the neighborhood
		for(unsigned int i=0; i < htable.size()-1;i++){
			if(strcmp(htable[i].neighborhood.c_str(),buffer) == 0){
				cprice = htable[i].nprice.c_str();
				break;
			}
			else{
				cprice = "-1";
			}
		}
		
		n = write(newsockfd, cprice, sizeof(buffer));

	
		if(n < 0){
			perror("ERROR writing to socket");
			exit(1);
		}
		
	}
	//closing my socket
	close(0);
		
	return 0;
}