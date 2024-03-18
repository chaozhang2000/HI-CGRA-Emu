#include"BitStream.h"
#include "CGRA.h"
//#include "mm2data.h"
#include "conv3data.h"
#include <iostream>
#include "config.h"

CONFIG_INFO config_info;
int main(){
			config_info.rows = 4;
			config_info.cols = 4;
			config_info.mrrgsize = 200;
			config_info. instmemsize= 8 ;
			config_info. constmemsize= 8 ;
	config_info. shiftconstmemsize= 8;
	config_info. datamemsize = 400;
	config_info. loop0start= 1;
	config_info. loop0inc= 1;
	config_info. loop0end= 19;
	config_info. loop1start=0 ;
	config_info. loop1inc= 1;
	config_info. loop1end= 1;
	config_info. loop2start= 0;
	config_info. loop2inc= 1;
	config_info. loop2end= 1;
	config_info. maxsimcycle= 1000;

	BitStreamInfo* bitstream =new BitStreamInfo;
	ConstructBitStream(bitstream);
	bool bitstreamNoErr = DumpBitStream(bitstream);
	if(!bitstreamNoErr){delete bitstream;return 1;}

	CGRA* cgra = new CGRA(config_info.rows,config_info.cols);

	cgra->CGRAReset();
	cgra->CGRALoadBitStream(bitstream);
	
	loaddataforconv3(cgra);
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
	printresultconv3(cgra);
	checkresultconv3(cgra);

	delete cgra;
	deleteBitStream(bitstream);
}
