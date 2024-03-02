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
	
	loaddataforconv3(cgra);
	//mm2
	//cgra->nodes[0][0]->datamem->writeDatas(dataA,0,9);
	//cgra->nodes[0][1]->datamem->writeDatas(dataB,0,9);

	int cycle = 0;
	for(cycle = 0; cycle< 1000;cycle++){
	std::cout<<std::endl;
	std::cout<<"----------------cycle"<<cycle << "------------"<<std::endl;
	cgra->CGRAExecOnecycle();
	if (cgra->IfFinished()){
		break;
	}
	}

	std::cout<<"-----------finish simulate-----------"<<std::endl;
	std::cout<<"Simulate cycle = "<<cycle<<std::endl;
	printresultconv3(cgra);

	delete cgra;
	delete bitstream;
}
