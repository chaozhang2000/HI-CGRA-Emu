#include "CGRANode.h"
#include "CGRALink.h"
#include "utils.h"
#include "encode.h"
#include <cstring>
#include <cassert>


Src CGRANode::getsrcnull(int key){
	Src src = {0,true};
	return src;
}
Src CGRANode::getsrcfromFureg(int key){
	Src src;
	src.data = Regs.fureg;
	src.valid = true;
	return src;
}
Src CGRANode::getsrcformconstmem(int key){
	assert(key == 0 || key == 1);
	Src src;
	src.data = key == 0 ? ConstMem1[Regs.ctrlregs.Constcnt1]:ConstMem2[Regs.ctrlregs.Constcnt2];
	src.valid = true;
	std::cout<<"src"<<key+1<<"const = "<<src.data<<std::endl;
	return src;
}
Src CGRANode::getsrcfromNlink(int key){
	Src src = {0,false};
	src = inLinks[LINK_DIRECTION_TO_N]->getSrc();
	std::cout<<"src"<<key+1<<"linkS_in = "<<src.data<<endl;
	return src;
}	
Src CGRANode::getsrcfromSlink(int key){
	Src src = {0,false};
	src = inLinks[LINK_DIRECTION_TO_S]->getSrc();
	std::cout<<"src"<<key+1<<"linkN_in = "<<src.data<<endl;
	return src;
}
Src CGRANode::getsrcfromWlink(int key){
	Src src = {0,false};
	src = inLinks[LINK_DIRECTION_TO_W]->getSrc();
	std::cout<<"src"<<key+1<<"linkE_in = "<<src.data<<endl;
	return src;
}
Src CGRANode::getsrcfromElink(int key){
	Src src = {0,false};
	src = inLinks[LINK_DIRECTION_TO_E]->getSrc();
	std::cout<<"src"<<key+1<<"linkW_in = "<<src.data<<endl;
	return src;
}
Src CGRANode::getsrcfromloop0reg(int key){
	std::cout<<"src"<<key+1<<"loop0k = "<<Regs.ctrlregs.K<<endl;
	Src src;
	src.data = Regs.ctrlregs.K;
	src.valid = true;
	return src;
}
Src CGRANode::getsrcfromloop1reg(int key){
	std::cout<<"src"<<key+1<<"loop1j = "<<Regs.ctrlregs.J<<endl;
	Src src;
	src.data = Regs.ctrlregs.J;
	src.valid = true;
	return src;
}
Src CGRANode::getsrcfromloop2reg(int key){
	std::cout<<"src"<<key+1<<"loop2i = "<<Regs.ctrlregs.I<<endl;
	Src src;
	src.data = Regs.ctrlregs.I;
	src.valid = true;
	return src;
}
Src CGRANode::getsrcfromFu(int key){
	return furesult;
}
int CGRANode::emptyopt(int src1, int src2){
				return 0;
}
int CGRANode::addopt(int src1, int src2){
				std::cout<<"exec add"<<std::endl;
				return src1 + src2;
}
int CGRANode::mulopt(int src1, int src2){
				std::cout<<"exec mul"<<std::endl;
				return src1 * src2;
}
int CGRANode::loadopt(int src1, int src2){
				std::cout<<"exec load"<<std::endl;
				return datamem->readData(src1);
}
int CGRANode::storeopt(int src1, int src2){
				std::cout<<"exec store"<<std::endl;
				datamem->writeData(src2,src1);
				return 0;
}
#define COMMON_OPTS(f)\
				f(mul) f(add) f(getelementptr)
#define LOAD_STORE_OPTS(f)\
				f(load) f(store)
#define M_SUPPORTOPTS_INSERT(k) m_supportOpts.insert(#k);

CGRANode::CGRANode(int t_id, int t_x, int t_y) {
	datamem = NULL;
  m_id = t_id;
  m_disabled = false;
	m_hasDataMem = true;
  m_x = t_x;
  m_y = t_y;
	COMMON_OPTS(M_SUPPORTOPTS_INSERT);
	if(m_hasDataMem){
		LOAD_STORE_OPTS(M_SUPPORTOPTS_INSERT);
		datamem = new DataMem(m_id);
	}
	m_neighbors = NULL;
	furesult = {0,false};
getsrc[SRC_NOT_OCCUPY] = &CGRANode::getsrcnull;
getsrc[SRC_OCCUPY_FROM_FU] = &CGRANode::getsrcfromFureg;
getsrc[SRC_OCCUPY_FROM_CONST_MEM] = &CGRANode::getsrcformconstmem;
getsrc[SRC_OCCUPY_FROM_N] = &CGRANode::getsrcfromNlink;
getsrc[SRC_OCCUPY_FROM_S] = &CGRANode::getsrcfromSlink;
getsrc[SRC_OCCUPY_FROM_W] = &CGRANode::getsrcfromWlink;
getsrc[SRC_OCCUPY_FROM_E] = &CGRANode::getsrcfromElink;
getsrc[SRC_OCCUPY_FROM_LOOP0] = &CGRANode::getsrcfromloop0reg;
getsrc[SRC_OCCUPY_FROM_LOOP1] = &CGRANode::getsrcfromloop1reg;
getsrc[SRC_OCCUPY_FROM_LOOP2] = &CGRANode::getsrcfromloop2reg;
fuopts[FU_EMPTY] = &CGRANode::emptyopt;
fuopts[FU_MUL] = &CGRANode::mulopt;
fuopts[FU_ADD] = &CGRANode::addopt;
fuopts[FU_GETELEMENTPTR] = &CGRANode::addopt;
fuopts[FU_LOAD] = &CGRANode::loadopt;
fuopts[FU_STORE] = &CGRANode::storeopt;

getsrclink[LINK_NOT_OCCUPY] = &CGRANode::getsrcnull;
getsrclink[LINK_OCCUPY_EMPTY] = &CGRANode::getsrcnull;
getsrclink[LINK_OCCUPY_FROM_N] = &CGRANode::getsrcfromNlink;
getsrclink[LINK_OCCUPY_FROM_S] = &CGRANode::getsrcfromSlink;
getsrclink[LINK_OCCUPY_FROM_W] = &CGRANode::getsrcfromWlink;
getsrclink[LINK_OCCUPY_FROM_E] = &CGRANode::getsrcfromElink;
getsrclink[LINK_OCCUPY_FROM_FU] = &CGRANode::getsrcfromFu;
getsrclink[LINK_OCCUPY_FROM_FUREG] = &CGRANode::getsrcfromFureg;


	CGRANodeReset();
}

int CGRANode::getID(){
	return m_id;
}
int CGRANode::getx(){
	return m_x;
}
int CGRANode::gety(){
	return m_y;
}
bool CGRANode::hasDataMem(){
	return m_hasDataMem;
}
bool CGRANode::isdisable(){
	return m_disabled;
}
void CGRANode::attachInLink(CGRALink* t_link) {
  m_inLinks.push_back(t_link);
	inLinks[t_link->getdirection()] = t_link;
}

void CGRANode::attachOutLink(CGRALink* t_link) {
  m_outLinks.push_back(t_link);
	outLinks[t_link->getdirection()] = t_link;
}

bool CGRANode::canSupport(string t_optsname){
	return m_supportOpts.find(t_optsname)!=m_supportOpts.end();
}

list<CGRANode*>* CGRANode::getNeighbors(){
	if(m_neighbors == NULL){
		m_neighbors = new list<CGRANode*>;
		for(CGRALink* link : m_outLinks){
			m_neighbors->push_back(link->getdst());
		}
	}
	return m_neighbors;
}

CGRALink* CGRANode::getOutLinkto(CGRANode* t_node){
	for(CGRALink* link:m_outLinks){
		if(link->getdst() == t_node)
			return link;
	}
	return NULL;
}

CGRANode::~CGRANode(){
	if(m_neighbors != NULL){
		delete m_neighbors;
	}	
	if(datamem != NULL){
		delete datamem;
	}
}

void CGRANode::CGRANodeReset(){
	memset(&Regs, 0, sizeof(CGRANodeRegs));
	memset(&Regsupdate, 0, sizeof(CGRANodeRegs));
	memset(InstMem, 0, sizeof(InstMem));
	memset(ConstMem1, 0, sizeof(ConstMem1));
	memset(ConstMem2, 0, sizeof(ConstMem2));
	memset(ShiftconstMem1, 0, sizeof(ShiftconstMem1));
	memset(ShiftconstMem2, 0, sizeof(ShiftconstMem2));
}
void CGRANode::CGRANodeUpdate(){
	memcpy(&Regs,&Regsupdate,sizeof(CGRANodeRegs));
}
void CGRANode::CGRANodeLoadBitStream(BitStreamInfoPE* PEbitstream){
	memcpy(&(Regs.ctrlregs),&(PEbitstream->ctrlregs),sizeof(CtrlRegs));
	memcpy(&(Regsupdate.ctrlregs),&(PEbitstream->ctrlregs),sizeof(CtrlRegs));
	memcpy(InstMem,PEbitstream->insts,sizeof(InstMem));
	memcpy(ConstMem1,PEbitstream->const1,sizeof(ConstMem1));
	memcpy(ConstMem2,PEbitstream->const2,sizeof(ConstMem2));
	memcpy(ShiftconstMem1,PEbitstream->shiftconst1, sizeof(ShiftconstMem1));
	memcpy(ShiftconstMem2,PEbitstream->shiftconst2, sizeof(ShiftconstMem2));
/*
				OUTS("BitStreamInfoCGRANode " << m_id<< ":",ANSI_FG_CYAN);
        // Printing CGRAInstruction bitstream
        for (int j = 0; j < CONFIG_CGRA_INSTMEM_SIZE; ++j) {
            OUTS( "  CGRAInstruction " << j << ": ",ANSI_FG_MAGENTA);
            // Printing FuInst
            std::cout << "FuInst: (Fukey: " << InstMem[j].FuInst.Fukey
                      << ", Src1key: " << InstMem[j].FuInst.Src1key
                      << ", Src2key: " << InstMem[j].FuInst.Src2key
                      << ", FudelayII: " << InstMem[j].FuInst.FudelayII
                      << ", Shiftconst1: " << InstMem[j].FuInst.Shiftconst1
                      << ", Shiftconst2: " << InstMem[j].FuInst.Shiftconst2
                      << ") ";
            // Printing LinkInsts
            for (int k = 0; k < 4; ++k) {
                std::cout << "LinkInst " << k << ": (Dkey: "
                          << InstMem[j].LinkInsts[k].Dkey
                          << ", DelayII: " << InstMem[j].LinkInsts[k].DelayII
                          << ") ";
            }
            std::cout << std::endl;
        }
				*/
}

void CGRANode::CGRANodeExecOnecycle(){
				std::cout<<"-------CGRANodeID:"<<m_id<<"-------"<<std::endl;
				furesult.valid = false;
				bool canexe = false;
				bool const1 = false;
				bool const2 = false;
				bool shiftconst1 = false;
				bool shiftconst2 = false;
				CGRANodeInst Inst = InstMem[Regs.ctrlregs.Instcnt];
            std::cout << "FuInst: (Fukey: " << Inst.FuInst.Fukey
                      << ", Src1key: " << Inst.FuInst.Src1key
                      << ", Src2key: " << Inst.FuInst.Src2key
                      << ", FudelayII: " << Inst.FuInst.FudelayII
                      << ", Shiftconst1: " << Inst.FuInst.Shiftconst1
                      << ", Shiftconst2: " << Inst.FuInst.Shiftconst2
                      << ") ";

            std::cout << std::endl;
						std::cout << "IIcnt = "<<Regs.ctrlregs.IIcnt<<std::endl;

				/* can exe ?*/
				/* fu's src should valid,and all outlink's src should valid too, if delay or have finished,the src is considered valid, but when exe we just stay the state not change*/
				Src fusrc1 = (this->*getsrc[Inst.FuInst.Src1key])(0);
				Src fusrc2 = (this->*getsrc[Inst.FuInst.Src2key])(1);
				if(Inst.FuInst.Shiftconst1)fusrc1.data = fusrc1.data + ShiftconstMem1[Regs.ctrlregs.Shiftconstcnt1];
				if(Inst.FuInst.Shiftconst2)fusrc2.data = fusrc2.data + ShiftconstMem2[Regs.ctrlregs.Shiftconstcnt2];

				if(Regs.ctrlregs.IIcnt < Inst.FuInst.FudelayII || Regs.ctrlregs.IIcnt >= Regs.ctrlregs.IInum + Inst.FuInst.FudelayII){
					fusrc1.valid = true;
					fusrc2.valid = true;
				}

				if(fusrc1.valid && fusrc2.valid && Regs.ctrlregs.IIcnt >= Inst.FuInst.FudelayII && Regs.ctrlregs.IIcnt < Regs.ctrlregs.IInum + Inst.FuInst.FudelayII) furesult.valid = true;

				Src linksrcs[4];
				linksrcs[LINK_DIRECTION_TO_N] = (this->*getsrclink[Inst.LinkInsts[LINK_DIRECTION_TO_N].Dkey])(0);
				linksrcs[LINK_DIRECTION_TO_S] = (this->*getsrclink[Inst.LinkInsts[LINK_DIRECTION_TO_S].Dkey])(0);
				linksrcs[LINK_DIRECTION_TO_W] =(this->*getsrclink[Inst.LinkInsts[LINK_DIRECTION_TO_W].Dkey])(0);
				linksrcs[LINK_DIRECTION_TO_E] = (this->*getsrclink[Inst.LinkInsts[LINK_DIRECTION_TO_E].Dkey])(0);
				if(Regs.ctrlregs.IIcnt < Inst.LinkInsts[LINK_DIRECTION_TO_N].DelayII || Regs.ctrlregs.IIcnt >= Regs.ctrlregs.IInum + Inst.LinkInsts[LINK_DIRECTION_TO_N].DelayII)linksrcs[LINK_DIRECTION_TO_N].valid = true;
				if(Regs.ctrlregs.IIcnt < Inst.LinkInsts[LINK_DIRECTION_TO_S].DelayII || Regs.ctrlregs.IIcnt >= Regs.ctrlregs.IInum + Inst.LinkInsts[LINK_DIRECTION_TO_S].DelayII)linksrcs[LINK_DIRECTION_TO_S].valid = true;
				if(Regs.ctrlregs.IIcnt < Inst.LinkInsts[LINK_DIRECTION_TO_W].DelayII || Regs.ctrlregs.IIcnt >= Regs.ctrlregs.IInum + Inst.LinkInsts[LINK_DIRECTION_TO_W].DelayII)linksrcs[LINK_DIRECTION_TO_W].valid = true;
				if(Regs.ctrlregs.IIcnt < Inst.LinkInsts[LINK_DIRECTION_TO_E].DelayII || Regs.ctrlregs.IIcnt >= Regs.ctrlregs.IInum + Inst.LinkInsts[LINK_DIRECTION_TO_E].DelayII)linksrcs[LINK_DIRECTION_TO_E].valid = true;

				canexe = fusrc1.valid & fusrc2.valid & linksrcs[LINK_DIRECTION_TO_N].valid & linksrcs[LINK_DIRECTION_TO_S].valid & linksrcs[LINK_DIRECTION_TO_W].valid & linksrcs[LINK_DIRECTION_TO_E].valid;
				if(canexe){
				std::cout<<"exec this cycle"<<std::endl;
				}
				else{
				std::cout<<"not exec this cycle"<<std::endl;
				std::cout<<"fusrc1.valid:" <<fusrc1.valid<<std::endl;
				std::cout<<"fusrc2.valid:" <<fusrc2.valid<<std::endl;
				std::cout<<"linksrcsN.valid:" <<linksrcs[LINK_DIRECTION_TO_N].valid<<std::endl;
				std::cout<<"linksrcsS.valid:" <<linksrcs[LINK_DIRECTION_TO_S].valid<<std::endl;
				std::cout<<"linksrcsW.valid:" <<linksrcs[LINK_DIRECTION_TO_W].valid<<std::endl;
				std::cout<<"linksrcsE.valid:" <<linksrcs[LINK_DIRECTION_TO_E].valid<<std::endl;
				}

				/*exe update the state in CGRANode and Link*/
				if(canexe){
					/*fuexe if delay or finish do nothing*/
					if(Regs.ctrlregs.IIcnt >= Inst.FuInst.FudelayII && Regs.ctrlregs.IIcnt < Regs.ctrlregs.IInum + Inst.FuInst.FudelayII){
						std::cout<<"fuexe:"<<std::endl;
						std::cout << "fusrc1 = "<<fusrc1.data<<" valid = "<<fusrc1.valid<<std::endl;
						std::cout << "fusrc2 = "<<fusrc2.data<<" valid = "<<fusrc2.valid<<std::endl;
						furesult.data = (this->*fuopts[Inst.FuInst.Fukey])(fusrc1.data,fusrc2.data);
						furesult.valid = Inst.FuInst.Fukey == FU_EMPTY ? false:true;
						std::cout << "furesult "<<furesult.data << std::endl;
					}
					else{
						std::cout<<"fu do nothing because of delay or finish"<<std::endl;
					}

					//update CGRANode state
					if(Inst.FuInst.Src1key == SRC_OCCUPY_FROM_CONST_MEM)const1 = true;
					if(Inst.FuInst.Src2key == SRC_OCCUPY_FROM_CONST_MEM)const2 = true;
					if((Inst.FuInst.Src1key ==SRC_OCCUPY_FROM_LOOP0||Inst.FuInst.Src1key ==SRC_OCCUPY_FROM_LOOP1||Inst.FuInst.Src1key ==SRC_OCCUPY_FROM_LOOP1)&& Inst.FuInst.Shiftconst1) shiftconst1 = true;
					if((Inst.FuInst.Src2key ==SRC_OCCUPY_FROM_LOOP0||Inst.FuInst.Src2key ==SRC_OCCUPY_FROM_LOOP1||Inst.FuInst.Src2key ==SRC_OCCUPY_FROM_LOOP1)&& Inst.FuInst.Shiftconst2) shiftconst2 = true;

					/*Instcnt update*/
					Regsupdate.ctrlregs.Instcnt = Regs.ctrlregs.Instcnt == Regs.ctrlregs.Instnum -1 ?0:Regs.ctrlregs.Instcnt + 1;
					/*IIcnt update*/
					Regsupdate.ctrlregs.IIcnt = Regs.ctrlregs.Instcnt == Regs.ctrlregs.Instnum -1 ? Regs.ctrlregs.IIcnt +1: Regs.ctrlregs.IIcnt;
					/*Constcnt1 update*/
					if(const1)Regsupdate.ctrlregs.Constcnt1 = Regs.ctrlregs.Constcnt1== Regs.ctrlregs.Constnum1 -1 ?0:Regs.ctrlregs.Constcnt1 + 1 ; 
					/*Constcnt2 update*/
					if(const2)Regsupdate.ctrlregs.Constcnt2 = Regs.ctrlregs.Constcnt2== Regs.ctrlregs.Constnum2 -1 ?0:Regs.ctrlregs.Constcnt2 + 1; 
					/*Shiftconstcnt1 update*/
					if(shiftconst1)Regsupdate.ctrlregs.Shiftconstcnt1 = Regs.ctrlregs.Shiftconstcnt1== Regs.ctrlregs.Shiftconstnum1 -1 ?0:Regs.ctrlregs.Shiftconstcnt1 + 1; 
					/*Constcnt2 update*/
					if(shiftconst2)Regsupdate.ctrlregs.Shiftconstcnt2 = Regs.ctrlregs.Shiftconstcnt2== Regs.ctrlregs.Shiftconstnum2 -1 ? 0:Regs.ctrlregs.Shiftconstcnt2 + 1; 

					/*K update*/
					bool Kchange = Regs.ctrlregs.Instcnt == Regs.ctrlregs.Instnum -1;
					bool Kinit = Kchange && (Regs.ctrlregs.K_inc>0 ? Regs.ctrlregs.K + Regs.ctrlregs.K_inc>=Regs.ctrlregs.K_thread:Regs.ctrlregs.K + Regs.ctrlregs.K_inc<=Regs.ctrlregs.K_thread);
					int Knew = Kinit ? Regs.ctrlregs.K_init : Regs.ctrlregs.K + Regs.ctrlregs.K_inc;
					Regsupdate.ctrlregs.K =Kchange ? Knew : Regs.ctrlregs.K;
					
					/*J update*/
					bool Jchange = Kinit;
					bool Jinit = Jchange && (Regs.ctrlregs.J_inc>0 ? Regs.ctrlregs.J + Regs.ctrlregs.J_inc>=Regs.ctrlregs.J_thread:Regs.ctrlregs.J + Regs.ctrlregs.J_inc<=Regs.ctrlregs.J_thread);
					int Jnew = Jinit ? Regs.ctrlregs.J_init : Regs.ctrlregs.J + Regs.ctrlregs.J_inc;
					Regsupdate.ctrlregs.J = Jchange? Jnew:Regs.ctrlregs.J;

					/*I update*/
					bool Ichange = Jinit;
					bool Iinit = Ichange && (Regs.ctrlregs.I_inc>0 ? Regs.ctrlregs.I + Regs.ctrlregs.I_inc>=Regs.ctrlregs.I_thread:Regs.ctrlregs.I + Regs.ctrlregs.I_inc<=Regs.ctrlregs.I_thread);
					int Inew = Iinit ? Regs.ctrlregs.I_init : Regs.ctrlregs.I + Regs.ctrlregs.I_inc;
					Regsupdate.ctrlregs.I = Ichange ? Inew:Regs.ctrlregs.I;
					
					/*fureg update*/
					if(furesult.valid) Regsupdate.fureg = furesult.data;


					/*link data update*/
					for(int i = 0; i< 4; ++ i){
          	std::cout << "LinkInst " << i << ": (Dkey: "
           	         << Inst.LinkInsts[i].Dkey
           	         << ", DelayII: " << Inst.LinkInsts[i].DelayII
           	         << ") "<<std::endl;
						
						if(Inst.LinkInsts[i].Dkey !=LINK_NOT_OCCUPY && Inst.LinkInsts[i].Dkey !=LINK_OCCUPY_EMPTY){
							if(Regs.ctrlregs.IIcnt >= Inst.LinkInsts[i].DelayII && Regs.ctrlregs.IIcnt < Regs.ctrlregs.IInum + Inst.LinkInsts[i].DelayII){
								Src linksrc = (this->*getsrclink[Inst.LinkInsts[i].Dkey])(0);
								std::cout<<"send "<<linksrc.data<<" to direction"<<i<<std::endl;
								outLinks[i]->Regsupdate.data = linksrc.data;
								outLinks[i]->Regsupdate.valid = linksrc.valid;
							}
						}
					}
				}
}
