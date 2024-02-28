#include"BitStream.h"
#include "CGRA.h"
//#include "mm2data.h"
#include "conv3data.h"
#include <iostream>

int main(){
	BitStreamInfo* bitstream = new BitStreamInfo;
	bool bitstreamNoErr = DumpBitStream(bitstream);
	if(!bitstreamNoErr){delete bitstream;return 1;}

	CGRA* cgra = new CGRA(4,4);

	cgra->CGRAReset();
	cgra->CGRALoadBitStream(bitstream);

	cgra->nodes[0][0]->datamem->writeDatas(conv3data1,0,20);
	cgra->nodes[0][1]->datamem->writeDatas(conv3data2,0,20);
	cgra->nodes[0][2]->datamem->writeDatas(conv3data3,0,20);

	//mm2
	//cgra->nodes[0][0]->datamem->writeDatas(dataA,0,9);
	//cgra->nodes[0][1]->datamem->writeDatas(dataB,0,9);

	for(int i = 0; i< 100;i++){
	std::cout<<std::endl;
	std::cout<<"----------------cycle"<<i << "------------"<<std::endl;
	cgra->CGRAExecOnecycle();
	}

	delete cgra;
	delete bitstream;
}
