#include"BitStream.h"
#include "CGRA.h"
//#include "mm2data.h"
#include "kernel.h"
#include <iostream>
#include "config.h"

int main(int argc,char* argv[]){

	if(argc < 3){
		std::cout<<"Need more params.This program need two params!!!"<<std::endl;
		return false;
	}
	std::string bitstreamfilename = argv[1];
	std::string configfilename = argv[2];

  if(!config_info.getconfig(configfilename))return false;

	BitStreamInfo* bitstream =new BitStreamInfo;
	ConstructBitStream(bitstream);
	bool bitstreamNoErr = DumpBitStream(bitstream,bitstreamfilename);
	if(!bitstreamNoErr){delete bitstream;return 1;}

	CGRA* cgra = new CGRA(config_info.rows,config_info.cols);

	cgra->CGRAReset();
	cgra->CGRALoadBitStream(bitstream);
	
	loaddataforkernel(cgra);
	//mm2
	//cgra->nodes[0][0]->datamem->writeDatas(dataA,0,9);
	//cgra->nodes[0][1]->datamem->writeDatas(dataB,0,9);

	int cycle = 0;
	for(cycle = 0; cycle< config_info.maxsimcycle;cycle++){
	std::cout<<std::endl;
	std::cout<<"----------------cycle"<<cycle << "------------"<<std::endl;
	cgra->CGRAExecOnecycle();
	if (cgra->IfFinished()){
		break;
	}
	}

	std::cout<<"-----------finish simulate-----------"<<std::endl;
	std::cout<<"Simulate cycle = "<<cycle<<std::endl;
	printresult(cgra);
	checkresult(cgra);

	delete cgra;
	deleteBitStream(bitstream);
}
