#include"CGRA.h"
#include <iostream>
int conv3data1[20] ={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
int conv3data2[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
int conv3data3[20] ={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

void loaddataforconv3(CGRA* cgra){
	cgra->nodes[0][0]->datamem->writeDatas(conv3data1,0,20);
	cgra->nodes[0][1]->datamem->writeDatas(conv3data2,0,20);
	cgra->nodes[0][2]->datamem->writeDatas(conv3data3,0,20);
}
void printresultconv3(CGRA*cgra){
	std::cout<<"Print result:"<<std::endl;
	for(int i = 1; i<=18;i++){
		std::cout<<"address = "<<i<<" data = "<<cgra->nodes[0][3]->datamem->readData(i)<<std::endl;
	} 
}
