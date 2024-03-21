#include <iostream>
#include <fstream>
#include "BitStream.h"
#include "utils.h"
#include "config.h"

/**This Function read bitstream from bitstream.bin
 * then check if InstMemsize,ConstMemsize,ShiftConstMemsize is right
 * finally print the bitstream
 * @return: true if no err happen
 */
bool DumpBitStream(BitStreamInfo* bitstream,std::string& filename){
    std::ifstream file(filename, std::ios::binary);

    if (!file) {
				ERRS("Failed to open file.",ANSI_FG_RED);
        return false;
    }

    // Read bitstream from file
    file.read(reinterpret_cast<char*>(bitstream), sizeof(BitStreamCheck));

    if (!file) {
        ERRS("Error occurred while reading the file. May cause by InstMemSize,ConstMemSize,ShiftConstMemSize wrong configuration",ANSI_FG_RED); 
        return false;
    }

		if(bitstream->CheckInfo.InstMemSize!=config_info.instmemsize){
        ERRS("Bitstream InstMem size == "<< bitstream->CheckInfo.InstMemSize <<" but EMU InstMem size is set "<<config_info.instmemsize,ANSI_FG_RED);
				return false;
		}
		if(bitstream->CheckInfo.ConstMemSize!=config_info.constmemsize){
        ERRS("Bitstream ConstMem size == "<< bitstream->CheckInfo.ConstMemSize <<" but EMU ConstMem size is set "<<config_info.constmemsize,ANSI_FG_RED);
				return false;
		}
		if(bitstream->CheckInfo.ShiftConstMemSize!=config_info.shiftconstmemsize){
        ERRS("Bitstream ShiftConstMem size == "<< bitstream->CheckInfo.ShiftConstMemSize <<" but EMU ShiftConstMem size is set "<<config_info.shiftconstmemsize,ANSI_FG_RED);
				return false; 
		}
		
	for(int i = 0;i<config_info.rows * config_info.cols ;i++){
					file.read((char*)(bitstream->BitstreaminfoOfPE[i].insts),sizeof(CGRAInstruction)*config_info.instmemsize);
					file.read((char*)(bitstream->BitstreaminfoOfPE[i].const1),sizeof(int)*config_info.constmemsize);
					file.read((char*)(bitstream->BitstreaminfoOfPE[i].const2),sizeof(int)*config_info.constmemsize);
					file.read((char*)(bitstream->BitstreaminfoOfPE[i].shiftconst1),sizeof(int)*config_info.shiftconstmemsize);
					file.read((char*)(bitstream->BitstreaminfoOfPE[i].shiftconst2),sizeof(int)*config_info.shiftconstmemsize);
					file.read((char*)(&(bitstream->BitstreaminfoOfPE[i].ctrlregs)),sizeof(CtrlRegs));
	}

#ifdef CONFIG_BITSTREAM_DUMP
    // Printing the bitstream
  	OUTS("\nBitstream information",ANSI_FG_BLUE);
		OUTS("==================================",ANSI_FG_CYAN); 
    for (int i = 0; i < 16; ++i) {
				OUTS("BitStreamInfoPE " << i << ":",ANSI_FG_CYAN);
        // Printing CGRAInstruction bitstream
        for (int j = 0; j < config_info.instmemsize; ++j) {
            OUTS( "  CGRAInstruction " << j << ": ",ANSI_FG_MAGENTA);
            // Printing FuInst
            std::cout << "FuInst: (Fukey: " << bitstream->BitstreaminfoOfPE[i].insts[j].FuInst.Fukey
                      << ", Src1key: " << bitstream->BitstreaminfoOfPE[i].insts[j].FuInst.Src1key
                      << ", Src2key: " << bitstream->BitstreaminfoOfPE[i].insts[j].FuInst.Src2key
                      << ", FudelayII: " << bitstream->BitstreaminfoOfPE[i].insts[j].FuInst.FudelayII
                      << ", Shiftconst1: " << bitstream->BitstreaminfoOfPE[i].insts[j].FuInst.Shiftconst1
                      << ", Shiftconst2: " << bitstream->BitstreaminfoOfPE[i].insts[j].FuInst.Shiftconst2
                      << ") ";
            // Printing LinkInsts
            for (int k = 0; k < 4; ++k) {
                std::cout << "LinkInst " << k << ": (Dkey: "
                          << bitstream->BitstreaminfoOfPE[i].insts[j].LinkInsts[k].Dkey
                          << ", DelayII: " << bitstream->BitstreaminfoOfPE[i].insts[j].LinkInsts[k].DelayII
                          << ") ";
            }
            std::cout << std::endl;
        }
        // Printing Constmem bitstream
        OUTS( "  Constmem1:",ANSI_FG_MAGENTA); 
        for (int j = 0; j < config_info.constmemsize; ++j) {
            std::cout << "    const1[" << j << "]: " << bitstream->BitstreaminfoOfPE[i].const1[j] << std::endl;
            /* Print other fields similarly */
        }
        OUTS( "  Constmem2:",ANSI_FG_MAGENTA); 
        for (int j = 0; j < config_info.constmemsize; ++j) {
            std::cout << "    const2[" << j << "]: " << bitstream->BitstreaminfoOfPE[i].const2[j] << std::endl;
            /* Print other fields similarly */
        }
        // Printing Shiftconstmem bitstream
        OUTS( "  Shiftconstmem1:",ANSI_FG_MAGENTA); 
        for (int j = 0; j < config_info.shiftconstmemsize; ++j) {
            std::cout << "    shiftconst1[" << j << "]: " << bitstream->BitstreaminfoOfPE[i].shiftconst1[j] << std::endl;
            /* Print other fields similarly */
        }
        OUTS( "  Shiftconstmem2:",ANSI_FG_MAGENTA); 
        for (int j = 0; j < config_info.shiftconstmemsize; ++j) {
            std::cout << "    shiftconst2[" << j << "]: " << bitstream->BitstreaminfoOfPE[i].shiftconst2[j] << std::endl;
            /* Print other fields similarly */
        }
        // Printing CtrlRegs bitstream
        OUTS( "  CtrlRegs:",ANSI_FG_MAGENTA); 
        std::cout << "    Instnum: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.Instnum << std::endl;
        std::cout << "    IInum: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.IInum << std::endl;
        std::cout << "    Startcyclenum: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.Startcyclenum<< std::endl;
        std::cout << "    FinishInstcnt: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.FinishInstcnt << std::endl;
        std::cout << "    FinishIIcnt: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.FinishIIcnt<< std::endl;
        std::cout << "    Constnum1: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.Constnum1 << std::endl;
        std::cout << "    Constnum2: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.Constnum2 << std::endl;
        std::cout << "    Shiftconstnum1: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.Shiftconstnum1 << std::endl;
        std::cout << "    Shiftconstnum2: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.Shiftconstnum2 << std::endl;
        std::cout << "    I_init: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.I_init << std::endl;
        std::cout << "    J_init: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.J_init << std::endl;
        std::cout << "    K_init: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.K_init << std::endl;
        std::cout << "    I_inc: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.I_inc << std::endl;
        std::cout << "    J_inc: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.J_inc << std::endl;
        std::cout << "    K_inc: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.K_inc << std::endl;
        std::cout << "    I_thread: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.I_thread << std::endl;
        std::cout << "    J_thread: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.J_thread << std::endl;
        std::cout << "    K_thread: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.K_thread << std::endl;
        std::cout << "    Startcyclecnt: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.Startcyclecnt<< std::endl;
        std::cout << "    Instcnt: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.Instcnt << std::endl;
        std::cout << "    IIcnt: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.IIcnt << std::endl;
        std::cout << "    Constcnt1: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.Constcnt1 << std::endl;
        std::cout << "    Constcnt2: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.Constcnt2 << std::endl;
        std::cout << "    Shiftconstcnt1: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.Shiftconstcnt1 << std::endl;
        std::cout << "    Shiftconstcnt2: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.Shiftconstcnt2 << std::endl;
        std::cout << "    K: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.K << std::endl;
        std::cout << "    J: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.J<< std::endl;
        std::cout << "    I: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.I<< std::endl;
        std::cout << "    Finish: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.Finish<< std::endl;
        /* Print other fields similarly */
        std::cout << std::endl; // Adding separation between BitStreamInfoPE
    }
#endif

    file.close();

    return true;
}
void ConstructBitStream(BitStreamInfo* bitstream){
	bitstream->BitstreaminfoOfPE = new BitStreamInfoPE[config_info.rows * config_info.cols];
	for(int i = 0; i< config_info.rows;i++){
		for(int j = 0; j< config_info.cols;j++){
			bitstream->BitstreaminfoOfPE[i*config_info.cols+j].insts = new CGRAInstruction[config_info.instmemsize];
			bitstream->BitstreaminfoOfPE[i*config_info.cols+j].const1= new int[config_info.constmemsize];
			bitstream->BitstreaminfoOfPE[i*config_info.cols+j].const2= new int[config_info.constmemsize];
			bitstream->BitstreaminfoOfPE[i*config_info.cols+j].shiftconst1= new int[config_info.shiftconstmemsize];
			bitstream->BitstreaminfoOfPE[i*config_info.cols+j].shiftconst2= new int[config_info.shiftconstmemsize];
		}
	}
}
void deleteBitStream(BitStreamInfo* bitstream){
	for( int i = 0; i<config_info.rows;i++){
		for(int j = 0; j<config_info.cols;j++){
			delete [] bitstream->BitstreaminfoOfPE[i*config_info.cols+j].insts; 
			delete [] bitstream->BitstreaminfoOfPE[i*config_info.cols+j].const1;
			delete [] bitstream->BitstreaminfoOfPE[i*config_info.cols+j].const2;
			delete [] bitstream->BitstreaminfoOfPE[i*config_info.cols+j].shiftconst1;
			delete [] bitstream->BitstreaminfoOfPE[i*config_info.cols+j].shiftconst2;
	}
	}
	delete [] bitstream->BitstreaminfoOfPE;
	delete bitstream;
}

