#include <iostream>
#include <fstream>
#include "BitStream.h"
#include "utils.h"

/**This Function read bitstream from bitstream.bin
 * then check if InstMemsize,ConstMemsize,ShiftConstMemsize is right
 * finally print the bitstream
 * @return: true if no err happen
 */
bool DumpBitStream(BitStreamInfo* bitstream) {
    std::ifstream file("bitstream.bin", std::ios::binary);

    if (!file) {
				ERRS("Failed to open file.",ANSI_FG_RED);
        return false;
    }

    // Read bitstream from file
    file.read(reinterpret_cast<char*>(bitstream), sizeof(BitStreamInfo));

    if (!file) {
        ERRS("Error occurred while reading the file. May cause by InstMemSize,ConstMemSize,ShiftConstMemSize wrong configuration",ANSI_FG_RED); 
        return false;
    }

		if(bitstream->CheckInfo.InstMemSize!=CONFIG_CGRA_INSTMEM_SIZE){
        ERRS("Bitstream InstMem size == "<< bitstream->CheckInfo.InstMemSize <<" but EMU InstMem size is set "<<CONFIG_CGRA_INSTMEM_SIZE,ANSI_FG_RED);
				return false;
		}
		if(bitstream->CheckInfo.ConstMemSize!=CONFIG_CGRA_CONSTMEM_SIZE){
        ERRS("Bitstream ConstMem size == "<< bitstream->CheckInfo.ConstMemSize <<" but EMU ConstMem size is set "<<CONFIG_CGRA_CONSTMEM_SIZE,ANSI_FG_RED);
				return false;
		}
		if(bitstream->CheckInfo.ShiftConstMemSize!=CONFIG_CGRA_SHIFTCONSTMEM_SIZE){
        ERRS("Bitstream ShiftConstMem size == "<< bitstream->CheckInfo.ShiftConstMemSize <<" but EMU ShiftConstMem size is set "<<CONFIG_CGRA_SHIFTCONSTMEM_SIZE,ANSI_FG_RED);
				return false;
		}

#ifdef CONFIG_BITSTREAM_DUMP
    // Printing the bitstream
  	OUTS("\nBitstream information",ANSI_FG_BLUE);
		OUTS("==================================",ANSI_FG_CYAN); 
    for (int i = 0; i < 16; ++i) {
				OUTS("BitStreamInfoPE " << i << ":",ANSI_FG_CYAN);
        // Printing CGRAInstruction bitstream
        for (int j = 0; j < CONFIG_CGRA_INSTMEM_SIZE; ++j) {
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
        for (int j = 0; j < CONFIG_CGRA_CONSTMEM_SIZE; ++j) {
            std::cout << "    const1[" << j << "]: " << bitstream->BitstreaminfoOfPE[i].const1[j] << std::endl;
            /* Print other fields similarly */
        }
        OUTS( "  Constmem2:",ANSI_FG_MAGENTA); 
        for (int j = 0; j < CONFIG_CGRA_CONSTMEM_SIZE; ++j) {
            std::cout << "    const2[" << j << "]: " << bitstream->BitstreaminfoOfPE[i].const2[j] << std::endl;
            /* Print other fields similarly */
        }
        // Printing Shiftconstmem bitstream
        OUTS( "  Shiftconstmem1:",ANSI_FG_MAGENTA); 
        for (int j = 0; j < CONFIG_CGRA_SHIFTCONSTMEM_SIZE; ++j) {
            std::cout << "    shiftconst1[" << j << "]: " << bitstream->BitstreaminfoOfPE[i].shiftconst1[j] << std::endl;
            /* Print other fields similarly */
        }
        OUTS( "  Shiftconstmem2:",ANSI_FG_MAGENTA); 
        for (int j = 0; j < CONFIG_CGRA_SHIFTCONSTMEM_SIZE; ++j) {
            std::cout << "    shiftconst2[" << j << "]: " << bitstream->BitstreaminfoOfPE[i].shiftconst2[j] << std::endl;
            /* Print other fields similarly */
        }
        // Printing CtrlRegs bitstream
        OUTS( "  CtrlRegs:",ANSI_FG_MAGENTA); 
        std::cout << "    Instnum: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.Instnum << std::endl;
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
        std::cout << "    Instcnt: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.Instcnt << std::endl;
        std::cout << "    Constcnt1: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.Constcnt1 << std::endl;
        std::cout << "    Constcnt2: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.Constcnt2 << std::endl;
        std::cout << "    Shiftconstcnt1: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.Shiftconstcnt1 << std::endl;
        std::cout << "    Shiftconstcnt2: " << bitstream->BitstreaminfoOfPE[i].ctrlregs.Shiftconstcnt2 << std::endl;
        /* Print other fields similarly */
        std::cout << std::endl; // Adding separation between BitStreamInfoPE
    }
#endif

    file.close();

    return true;
}

