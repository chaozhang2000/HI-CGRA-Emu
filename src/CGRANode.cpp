#include "CGRANode.h"
#include "CGRALink.h"
#include "utils.h"
#include "encode.h"
#include <cstring>
#include <cassert>
#include "config.h"


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
Src CGRANode::emptyopt(Src src1, Src src2){
				std::cout<<"empty opt"<<std::endl;
				return {0,true};
}
Src CGRANode::addopt(Src src1, Src src2){
				std::cout<<"exec add"<<std::endl;
				return {src1.data + src2.data,src1.valid&&src2.valid};
}
Src CGRANode::mulopt(Src src1, Src src2){
				std::cout<<"exec mul"<<std::endl;
				return {src1.data * src2.data,src1.valid&& src2.valid};
}
Src CGRANode::loadopt(Src src1, Src src2){
				std::cout<<"exec load"<<std::endl;
				if(src1.valid && src2.valid)return datamem->fureadData(src1.data);
				else return {0,false};
}
Src CGRANode::storeopt(Src src1, Src src2){
				std::cout<<"exec store"<<std::endl;
				//datamem->writeData(src2.data,src1.data);
				return {0,src1.valid&&src2.valid};
}
Src CGRANode::shlopt(Src src1,Src src2){
				std::cout<<"exec shl"<<std::endl;
				return {src1.data << src2.data,src1.valid && src2.valid};
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
	InstMem = new CGRANodeInst[config_info.instmemsize];
	ConstMem1=new int[config_info.constmemsize];
	ConstMem2 = new int[config_info.constmemsize];
	ShiftconstMem1 = new int[config_info.shiftconstmemsize];
	ShiftconstMem2 = new int[config_info.shiftconstmemsize];
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
fuopts[FU_SHL] = &CGRANode::shlopt;

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
	delete [] InstMem ;
	delete [] ConstMem1;
	delete [] ConstMem2;
	delete [] ShiftconstMem1;
	delete [] ShiftconstMem2;
}

void CGRANode::CGRANodeReset(){
	memset(&Regs, 0, sizeof(CGRANodeRegs));
	memset(&Regsupdate, 0, sizeof(CGRANodeRegs));
	memset(InstMem, 0, config_info.instmemsize*sizeof(CGRAInstruction));
	memset(ConstMem1, 0, sizeof(int)*config_info.constmemsize);
	memset(ConstMem2, 0, sizeof(int)*config_info.constmemsize);
	memset(ShiftconstMem1, 0, sizeof(int)*config_info.shiftconstmemsize);
	memset(ShiftconstMem2, 0, sizeof(int)*config_info.shiftconstmemsize);
}
void CGRANode::CGRANodeUpdate(){
	memcpy(&Regs,&Regsupdate,sizeof(CGRANodeRegs));
}
void CGRANode::CGRANodeLoadBitStream(BitStreamInfoPE* PEbitstream){
	memcpy(&(Regs.ctrlregs),&(PEbitstream->ctrlregs),sizeof(CtrlRegs));
	memcpy(&(Regsupdate.ctrlregs),&(PEbitstream->ctrlregs),sizeof(CtrlRegs));
	memcpy(InstMem,PEbitstream->insts,config_info.instmemsize*sizeof(CGRAInstruction));
	memcpy(ConstMem1,PEbitstream->const1,sizeof(int)*config_info.constmemsize);
	memcpy(ConstMem2,PEbitstream->const2,sizeof(int)*config_info.constmemsize);
	memcpy(ShiftconstMem1,PEbitstream->shiftconst1, sizeof(int)*config_info.shiftconstmemsize);
	memcpy(ShiftconstMem2,PEbitstream->shiftconst2, sizeof(int)*config_info.shiftconstmemsize);
}

void CGRANode::CGRANodeExecOnecycle(){
				/* Fetch LinkInst print Inst */
				std::cout<<"-------CGRANodeID:"<<m_id<<"-------"<<std::endl;
				if(Regs.ctrlregs.Finish == true){
					std::cout << "this PE has finished state stop changing"<<std::endl;
					return;
				}
				CGRANodeInst Inst = InstMem[Regs.ctrlregs.Instcnt];
            std::cout << "Fetch FuInst: (Fukey: " << Inst.FuInst.Fukey
                      << ", Src1key: " << Inst.FuInst.Src1key
                      << ", Src2key: " << Inst.FuInst.Src2key
                      << ", FudelayII: " << Inst.FuInst.FudelayII
                      << ", Shiftconst1: " << Inst.FuInst.Shiftconst1
                      << ", Shiftconst2: " << Inst.FuInst.Shiftconst2
                      << ") ";
         std::cout << std::endl;
				for(int i = 0; i< 4; ++ i){
          	std::cout << "Fetch LinkInst " << i << ": (Dkey: "
           	         << Inst.LinkInsts[i].Dkey
           	         << ", DelayII: " << Inst.LinkInsts[i].DelayII
           	         << ") "<<std::endl;
					}
            std::cout << std::endl;

				/* Decode */
				bool const1 =Inst.FuInst.Src1key == SRC_OCCUPY_FROM_CONST_MEM;
				bool const2 =Inst.FuInst.Src2key == SRC_OCCUPY_FROM_CONST_MEM;
				bool shiftconst1 =(Inst.FuInst.Src1key ==SRC_OCCUPY_FROM_LOOP0||Inst.FuInst.Src1key ==SRC_OCCUPY_FROM_LOOP1||Inst.FuInst.Src1key ==SRC_OCCUPY_FROM_LOOP1)&& Inst.FuInst.Shiftconst1;
				bool shiftconst2 =(Inst.FuInst.Src2key ==SRC_OCCUPY_FROM_LOOP0||Inst.FuInst.Src2key ==SRC_OCCUPY_FROM_LOOP1||Inst.FuInst.Src2key ==SRC_OCCUPY_FROM_LOOP1)&& Inst.FuInst.Shiftconst2;
				bool needtosendout[4];
			  for(int i = 0;i <4;i++){needtosendout[i]	= (Inst.LinkInsts[i].Dkey !=LINK_NOT_OCCUPY && Inst.LinkInsts[i].Dkey !=LINK_OCCUPY_EMPTY);}

				/*fu out and crossbar out*/
				std::cout << "Check if srcs ready:" << std::endl;
				std::cout << "Fu srcs:" << std::endl;
				bool fuinstskip = Regs.ctrlregs.IIcnt < Inst.FuInst.FudelayII || Regs.ctrlregs.IIcnt >= Regs.ctrlregs.IInum + Inst.FuInst.FudelayII;
				bool fuinstnotskip = !fuinstskip;
				bool linkinstskip[4];
				bool linkinstnotskip[4];
				for(int i = 0; i<4;i++){
					linkinstskip[i] = Regs.ctrlregs.IIcnt < Inst.LinkInsts[i].DelayII || Regs.ctrlregs.IIcnt >= Regs.ctrlregs.IInum + Inst.LinkInsts[i].DelayII;
					linkinstnotskip[i] = !linkinstskip[i];
				}
				/*def wire src1 and src2 into fu*/
				Src fusrc1 = {0,false};
				Src fusrc2 = {0,false};
				/*wire src1 and src2 get data and valid from linkin or other srcs */
				fusrc1 = (this->*getsrc[Inst.FuInst.Src1key])(0);
				fusrc2 = (this->*getsrc[Inst.FuInst.Src2key])(1);
				fusrc1.data =Inst.FuInst.Shiftconst1 ? fusrc1.data + ShiftconstMem1[Regs.ctrlregs.Shiftconstcnt1]:fusrc1.data;
				fusrc2.data =Inst.FuInst.Shiftconst2 ? fusrc2.data + ShiftconstMem2[Regs.ctrlregs.Shiftconstcnt2]:fusrc2.data;
				furesult = (this->*fuopts[Inst.FuInst.Fukey])(fusrc1,fusrc2);
				furesult.valid = (fusrc1.valid && fusrc2.valid); //&& fuinstnotskip;
																												 //
				Src crossbarouts[4];
				for(int i =0;i<4;i++){
					crossbarouts[i] = (this->*getsrclink[Inst.LinkInsts[i].Dkey])(0);
				}
				bool fufinish = furesult.valid | fuinstskip; //furesult.valid is the wire directly out from alu,furesultoutvalid is another wire
				bool linkfinish[4] ;
				for(int i = 0; i<4;i++){
					//linkfinish[i] =linkinstskip[i] | (crossbarouts[i].valid);
					linkfinish[i] =linkinstskip[i] |crossbarouts[i].valid;
				}

				/*wire crossbar's out to links*/
				std::cout << "Link srcs:" << std::endl;

				bool canexe;
				canexe = fufinish & linkfinish[LINK_DIRECTION_TO_N] & linkfinish[LINK_DIRECTION_TO_S]& linkfinish[LINK_DIRECTION_TO_W] & linkfinish[LINK_DIRECTION_TO_E]
								;
				bool fureg_wen = canexe& furesult.valid & (Inst.FuInst.Fukey != FU_EMPTY) & fuinstnotskip;

				bool linkout_wen[4];
			  for(int i = 0;i <4;i++){linkout_wen[i]	= canexe & needtosendout[i] &&linkinstnotskip[i];}

				if(canexe){
				std::cout<<"srcs ready"<<std::endl;
				std::cout << std::endl;
				}
				else{
				std::cout <<"srcs not ready" << std::endl;
				std::cout<<"not exec this cycle"<<std::endl;
				std::cout<<"fusrc1.valid:" <<fusrc1.valid<<std::endl;
				std::cout<<"fusrc2.valid:" <<fusrc2.valid<<std::endl;
				std::cout<<"crossbaroutsN.valid:" <<crossbarouts[LINK_DIRECTION_TO_N].valid<<std::endl;
				std::cout<<"crossbaroutsS.valid:" <<crossbarouts[LINK_DIRECTION_TO_S].valid<<std::endl;
				std::cout<<"crossbaroutsW.valid:" <<crossbarouts[LINK_DIRECTION_TO_W].valid<<std::endl;
				std::cout<<"crossbaroutsE.valid:" <<crossbarouts[LINK_DIRECTION_TO_E].valid<<std::endl;
				std::cout << std::endl;
				}

				/*exe update the state in CGRANode and Link*/
				if(canexe){
					/*fuexe if delay or finish do nothing*/
					std::cout<<"Fu calculate result:"<<std::endl;
					if(Regs.ctrlregs.IIcnt >= Inst.FuInst.FudelayII && Regs.ctrlregs.IIcnt < Regs.ctrlregs.IInum + Inst.FuInst.FudelayII){
						std::cout << "fusrc1 = "<<fusrc1.data<<" valid = "<<fusrc1.valid<<std::endl;
						std::cout << "fusrc2 = "<<fusrc2.data<<" valid = "<<fusrc2.valid<<std::endl;
						std::cout << "furesult: data = "<<furesult.data <<" valid = "<<furesult.valid<< std::endl;
					}
					else if(Regs.ctrlregs.IIcnt < Inst.FuInst.FudelayII){
						std::cout<<"fu do nothing because of delay"<<std::endl;
					}else{
						std::cout<<"fu do nothing because of finish"<<std::endl;
					}
					std::cout << std::endl;

					std::cout<<"Update state in PE:(current state-> next state)"<<endl;
					//update CGRANode state
#define PRINT_STATE_UPDATE(name,oldValue, newValue)\
					std::cout<<name<<" : "<<oldValue<<" -> "<<newValue<<std::endl;
					/*Instcnt update*/
					Regsupdate.ctrlregs.Instcnt = Regs.ctrlregs.Instcnt == Regs.ctrlregs.Instnum -1 ?0:Regs.ctrlregs.Instcnt + 1;
					PRINT_STATE_UPDATE("ctrlregs.Instcnt",Regs.ctrlregs.Instcnt,Regsupdate.ctrlregs.Instcnt);
					/*IIcnt update*/
					Regsupdate.ctrlregs.IIcnt = Regs.ctrlregs.Instcnt == Regs.ctrlregs.Instnum -1 ? Regs.ctrlregs.IIcnt +1: Regs.ctrlregs.IIcnt;
					PRINT_STATE_UPDATE("ctrlregs.IIcnt",Regs.ctrlregs.IIcnt,Regsupdate.ctrlregs.IIcnt);
					/*Constcnt1 update*/
					if(const1)Regsupdate.ctrlregs.Constcnt1 = Regs.ctrlregs.Constcnt1== Regs.ctrlregs.Constnum1 -1 ?0:Regs.ctrlregs.Constcnt1 + 1 ; 
					PRINT_STATE_UPDATE("ctrlregs.Constcnt1",Regs.ctrlregs.Constcnt1,Regsupdate.ctrlregs.Constcnt1);
					/*Constcnt2 update*/
					if(const2)Regsupdate.ctrlregs.Constcnt2 = Regs.ctrlregs.Constcnt2== Regs.ctrlregs.Constnum2 -1 ?0:Regs.ctrlregs.Constcnt2 + 1; 
					PRINT_STATE_UPDATE("ctrlregs.Constcnt2",Regs.ctrlregs.Constcnt2,Regsupdate.ctrlregs.Constcnt2);
					/*Shiftconstcnt1 update*/
					if(shiftconst1)Regsupdate.ctrlregs.Shiftconstcnt1 = Regs.ctrlregs.Shiftconstcnt1== Regs.ctrlregs.Shiftconstnum1 -1 ?0:Regs.ctrlregs.Shiftconstcnt1 + 1; 
					PRINT_STATE_UPDATE("ctrlregs.Shiftconstcnt1",Regs.ctrlregs.Shiftconstcnt1,Regsupdate.ctrlregs.Shiftconstcnt1);
					/*Constcnt2 update*/
					if(shiftconst2)Regsupdate.ctrlregs.Shiftconstcnt2 = Regs.ctrlregs.Shiftconstcnt2== Regs.ctrlregs.Shiftconstnum2 -1 ? 0:Regs.ctrlregs.Shiftconstcnt2 + 1; 
					PRINT_STATE_UPDATE("ctrlregs.Shiftconstcnt2",Regs.ctrlregs.Shiftconstcnt2,Regsupdate.ctrlregs.Shiftconstcnt2);

					/*K update*/
					bool Kchange = Regs.ctrlregs.Instcnt == Regs.ctrlregs.Instnum -1;
					bool Kinit = (Regs.ctrlregs.K_inc>0 ? Regs.ctrlregs.K + Regs.ctrlregs.K_inc>=Regs.ctrlregs.K_thread:Regs.ctrlregs.K + Regs.ctrlregs.K_inc<=Regs.ctrlregs.K_thread);
					int Knew = Kinit ? Regs.ctrlregs.K_init : Regs.ctrlregs.K + Regs.ctrlregs.K_inc;
					Regsupdate.ctrlregs.K =Kchange ? Knew : Regs.ctrlregs.K;
					PRINT_STATE_UPDATE("ctrlregs.K",Regs.ctrlregs.K,Regsupdate.ctrlregs.K);
					
					/*J update*/
					bool Jchange = Kinit;
					bool Jinit = (Regs.ctrlregs.J_inc>0 ? Regs.ctrlregs.J + Regs.ctrlregs.J_inc>=Regs.ctrlregs.J_thread:Regs.ctrlregs.J + Regs.ctrlregs.J_inc<=Regs.ctrlregs.J_thread);
					int Jnew = Jinit ? Regs.ctrlregs.J_init : Regs.ctrlregs.J + Regs.ctrlregs.J_inc;
					Regsupdate.ctrlregs.J = Jchange? Jnew:Regs.ctrlregs.J;
					PRINT_STATE_UPDATE("ctrlregs.J",Regs.ctrlregs.J,Regsupdate.ctrlregs.J);

					/*I update*/
					bool Ichange = Jinit;
					bool Iinit = (Regs.ctrlregs.I_inc>0 ? Regs.ctrlregs.I + Regs.ctrlregs.I_inc>=Regs.ctrlregs.I_thread:Regs.ctrlregs.I + Regs.ctrlregs.I_inc<=Regs.ctrlregs.I_thread);
					int Inew = Iinit ? Regs.ctrlregs.I_init : Regs.ctrlregs.I + Regs.ctrlregs.I_inc;
					Regsupdate.ctrlregs.I = Ichange ? Inew:Regs.ctrlregs.I;
					PRINT_STATE_UPDATE("ctrlregs.I",Regs.ctrlregs.I,Regsupdate.ctrlregs.I);

					/*finishreg update*/
					Regsupdate.ctrlregs.Finish = (Regs.ctrlregs.IIcnt == Regs.ctrlregs.FinishIIcnt)&&(Regs.ctrlregs.Instcnt == Regs.ctrlregs.FinishInstcnt);
					PRINT_STATE_UPDATE("ctrlregs.Finish",Regs.ctrlregs.Finish,Regsupdate.ctrlregs.Finish);
					
					/*fureg update*/
					if(fureg_wen) {Regsupdate.fureg = furesult.data;
					PRINT_STATE_UPDATE("fureg",Regs.fureg,Regsupdate.fureg);
					}

#undef PRINT_STATE_UPDATE
					/*store*/
					if(Inst.FuInst.Fukey ==FU_STORE){datamem->writeData(fusrc2.data,fusrc1.data);}

					std::cout<<std::endl;


					/*link data update*/
					std::cout<<"Send data to Link:"<<std::endl;
					for(int i = 0; i< 4; ++ i){
						if(linkout_wen[i]){
							outLinks[i]->Regsupdate.data = crossbarouts[i].data;
							outLinks[i]->Regsupdate.valid= crossbarouts[i].valid;
						}
					}
				}
}
bool CGRANode::CGRANodeFinish(){
	return Regs.ctrlregs.Finish;
}
