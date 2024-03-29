#ifndef BitStream_H
#define BitStream_H
#include "generated/autoconf.h"
#include <string>
struct CGRALinkInst{
	int Dkey;
	int DelayII;
};
struct CGRAFuInst{
	int Fukey;
	int Src1key;
	int Src2key;
	int FudelayII;
	bool Shiftconst1;//
	bool Shiftconst2;//
};
struct CGRAInstruction {
	CGRAFuInst FuInst;
	CGRALinkInst LinkInsts[4];
};
struct CtrlRegs {
	int Instnum;
	int IInum;
	int Startcyclenum;
	int FinishInstcnt;
	int FinishIIcnt;
	int Constnum1;
	int Constnum2;
	int Shiftconstnum1;
	int Shiftconstnum2;
	int I_init;
	int J_init;
	int K_init;
	int I_inc;
	int J_inc;
	int K_inc;
	int I_thread;
	int J_thread;
	int K_thread;

	int Startcyclecnt;
	int Instcnt;
	int IIcnt;
	int Constcnt1;
	int Constcnt2;
	int Shiftconstcnt1;
	int Shiftconstcnt2;
	int K;
	int J;
	int I;
	bool Finish;
};

struct BitStreamInfoPE{
	CGRAInstruction *insts;
	int *const1;
	int *const2;
	int *shiftconst1;
	int *shiftconst2;
	CtrlRegs ctrlregs;
};
struct BitStreamCheck{
	int InstMemSize;
	int ConstMemSize;
	int ShiftConstMemSize;
};
struct BitStreamInfo{
	BitStreamCheck CheckInfo;
	BitStreamInfoPE *BitstreaminfoOfPE;
};

void ConstructBitStream(BitStreamInfo* bitstream);
void deleteBitStream(BitStreamInfo* bitstream);
bool DumpBitStream(BitStreamInfo* bitstream,std::string& filename);
#endif
