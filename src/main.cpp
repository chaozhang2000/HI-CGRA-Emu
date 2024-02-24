#include"BitStream.h"
#include "CGRA.h"

int main(){
	BitStreamInfo* bitstream = new BitStreamInfo;
	bool bitstreamNoErr = DumpBitStream(bitstream);
	if(!bitstreamNoErr){delete bitstream;return 1;}

	CGRA* cgra = new CGRA(4,4);

	cgra->CGRAReset();
	cgra->CGRALoadBitStream(bitstream);

	cgra->CGRAExecOnecycle();

	delete cgra;
	delete bitstream;
}
