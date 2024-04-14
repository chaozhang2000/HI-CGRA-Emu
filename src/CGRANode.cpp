#include "CGRANode.h"
#include "CGRALink.h"
#include "utils.h"
#include "encode.h"
#include <cstring>
#include <cassert>
#include "config.h"
#include <algorithm>

string encode_optname_map[30];
int CGRANode::getsrcnull(int key){
	return 0;
}
int CGRANode::getsrcfromFureg(int key){
	return Regs.fureg;
}
int CGRANode::getsrcformconstmem(int key){
	assert(key == 0 || key == 1);
	return key == 0 ? ConstMem1[Regs.ctrlregs.Constcnt1]:ConstMem2[Regs.ctrlregs.Constcnt2];
}
int CGRANode::getsrcfromNlink(int key){
	return inLinks[LINK_DIRECTION_TO_N]->getSrc();
}	
int CGRANode::getsrcfromSlink(int key){
	return inLinks[LINK_DIRECTION_TO_S]->getSrc();
}
int CGRANode::getsrcfromWlink(int key){
	return inLinks[LINK_DIRECTION_TO_W]->getSrc();
}
int CGRANode::getsrcfromElink(int key){
	return inLinks[LINK_DIRECTION_TO_E]->getSrc();
}
int CGRANode::getsrcfromloop0reg(int key){
	return Regs.ctrlregs.K;
}
int CGRANode::getsrcfromloop1reg(int key){
	return Regs.ctrlregs.J;
}
int CGRANode::getsrcfromloop2reg(int key){
	return Regs.ctrlregs.I;
}
int CGRANode::getsrcfromFu(int key){
	return furesult.data;
}
Src CGRANode::emptyopt(int src1, int src2){
				std::cout<<"empty opt"<<std::endl;
				return {0,false};
}
Src CGRANode::addopt(int src1, int src2){
				std::cout<<"exec add"<<std::endl;
				return {src1 + src2,true};
}
Src CGRANode::mulopt(int src1, int src2){
				std::cout<<"exec mul"<<std::endl;
				return {src1 * src2,true};
}
Src CGRANode::loadopt(int src1, int src2){
				assert(datamem!=NULL);
				std::cout<<"exec load"<<std::endl;
				return datamem->fureadData(src1);
}
Src CGRANode::storeopt(int src1, int src2){
				assert(datamem!=NULL);
				std::cout<<"exec store"<<std::endl;
				datamem->writeData(src2,src1);
				return {0,true};
}
Src CGRANode::shlopt(int src1,int src2){
				std::cout<<"exec shl"<<std::endl;
				return {src1 << src2,true};
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
fuopts[FU_mul] = &CGRANode::mulopt;
fuopts[FU_add] = &CGRANode::addopt;
fuopts[FU_getelementptr] = &CGRANode::addopt;
fuopts[FU_load] = &CGRANode::loadopt;
fuopts[FU_store] = &CGRANode::storeopt;
fuopts[FU_shl] = &CGRANode::shlopt;

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

				furesult.data = 0;
				furesult.valid = 0;
				int fusrc1 = 0;
				int fusrc2 = 0;
				/* Decode */
				int alukey = Inst.FuInst.Fukey;
				int src1key = Inst.FuInst.Src1key;
				int src2key = Inst.FuInst.Src2key;
				int linkkeys[4];
				for(int i = 0;i<4;i++){linkkeys[i] = Inst.LinkInsts[i].Dkey;}
				bool const1 =Inst.FuInst.Src1key == SRC_OCCUPY_FROM_CONST_MEM;
				bool const2 =Inst.FuInst.Src2key == SRC_OCCUPY_FROM_CONST_MEM;
				bool shiftconst1 =Inst.FuInst.Shiftconst1;
				bool shiftconst2 =Inst.FuInst.Shiftconst2;
				bool linkneedtosendout[4];
			  for(int i = 0;i <4;i++){linkneedtosendout[i]	= (Inst.LinkInsts[i].Dkey !=LINK_NOT_OCCUPY && Inst.LinkInsts[i].Dkey !=LINK_OCCUPY_EMPTY);}
				bool fuinstskip = Regs.ctrlregs.IIcnt < Inst.FuInst.FudelayII || Regs.ctrlregs.IIcnt >= Regs.ctrlregs.IInum + Inst.FuInst.FudelayII;
				bool linkinstskip[4];
				for(int i = 0; i<4;i++){
					linkinstskip[i] = Regs.ctrlregs.IIcnt < Inst.LinkInsts[i].DelayII || Regs.ctrlregs.IIcnt >= Regs.ctrlregs.IInum + Inst.LinkInsts[i].DelayII;
				}
				bool canexe;
				canexe =(Regs.ctrlregs.Startcyclecnt >= Regs.ctrlregs.Startcyclenum);
				/* Decode print */
#define DECODES_SIGNALS(f) \
  f(const1) f(const2) f(shiftconst1) f(linkneedtosendout[0]) f(linkneedtosendout[1]) f(linkneedtosendout[2]) f(linkneedtosendout[3]) f(fuinstskip) f(linkinstskip[0]) f(linkinstskip[1]) f(linkinstskip[2]) f(linkinstskip[3]) f(canexe)
#define PRINT_DECODE(name)\
					std::cout<<#name<<" : "<<name<<std::endl;
				std::cout << "Decode:"<<std::endl;
				DECODES_SIGNALS(PRINT_DECODE)
				std::cout <<std::endl;
#undef PRINT_DECODE
#undef DECODES_SIGNALS

				/*src1 and src2 get data and valid from linkin or other srcs ,src1mux and src2mux*/
				fusrc1 = (this->*getsrc[src1key])(0);
				fusrc2 = (this->*getsrc[src2key])(1);
				fusrc1 =Inst.FuInst.Shiftconst1 ? fusrc1 + ShiftconstMem1[Regs.ctrlregs.Shiftconstcnt1]:fusrc1;
				fusrc2 =Inst.FuInst.Shiftconst2 ? fusrc2 + ShiftconstMem2[Regs.ctrlregs.Shiftconstcnt2]:fusrc2;

				/*start cyclecnt update*/
				Regsupdate.ctrlregs.Startcyclecnt= (Regs.ctrlregs.Startcyclecnt < Regs.ctrlregs.Startcyclenum)?Regs.ctrlregs.Startcyclecnt + 1:Regs.ctrlregs.Startcyclecnt;

				/*exe update the state in CGRANode and Link*/
				if(canexe){
					std::cout << "Fu:" << std::endl;
					std::cout << "src1: data:" <<fusrc1<< std::endl;
					std::cout << "src2: data:" <<fusrc2<< std::endl;
					/*fu input*/
					auto it = config_info.execLatency.find(opt_encode_name_map[alukey]);
					int latency = it != config_info.execLatency.end() ? it->second:0;
					Opt opt;
					opt.latency = latency; opt.key = alukey;opt.src1 = fusrc1; opt.src2 = fusrc2;
					if(alukey != FU_EMPTY && (!fuinstskip)){
									pendingopts.push_back(opt);
					std::cout <<opt_encode_name_map[alukey]<<" start and will generate result "<<latency<<" cycles latter"<<std::endl;
					}
					int canexenum = 0;
					for(auto it = pendingopts.begin();it != pendingopts.end();++it){
						if((*it).latency ==  0) canexenum ++;
					}
					assert(canexenum <=1);
					for(auto it = pendingopts.begin();it != pendingopts.end();++it){
						if((*it).latency == 0){
							int fukey =(*it).key ;
							int src1 = (*it).src1;
							int src2 = (*it).src2;
				  		furesult = (this->*fuopts[fukey])(src1,src2);
							std::cout <<opt_encode_name_map[fukey]<<" generate result."<<std::endl;
							std::cout<<"furesult.data = "<<furesult.data<<" furesult.valid = "<<furesult.valid<<std::endl;
						}
					}
					if(canexenum == 0){
				  		furesult = (this->*fuopts[FU_EMPTY])(fusrc1,fusrc2);
							std::cout <<opt_encode_name_map[FU_EMPTY]<<" generate result."<<std::endl;
							std::cout<<"furesult.data = "<<furesult.data<<" furesult.valid = "<<furesult.valid<<std::endl;
					}
					pendingopts.erase(remove_if(pendingopts.begin(),pendingopts.end(),[]( Opt t){return t.latency == 0;}),pendingopts.end());
					for(auto it = pendingopts.begin();it != pendingopts.end();++it){
						if((*it).latency > 0){
							(*it).latency --;
						}
					}

					/*fuexe if delay or finish do nothing*/
					if(Regs.ctrlregs.IIcnt >= Inst.FuInst.FudelayII && Regs.ctrlregs.IIcnt < Regs.ctrlregs.IInum + Inst.FuInst.FudelayII){
									;
					}
					else if(Regs.ctrlregs.IIcnt < Inst.FuInst.FudelayII){
						std::cout<<"fu exe empty because of delay"<<std::endl;
					}else{
						std::cout<<"fu exe empty because of finish"<<std::endl;
					}
					std::cout << std::endl;

					/* update CGRANode state*/
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
					bool Kinit = (Regs.ctrlregs.K_inc>0 ? Regs.ctrlregs.K + Regs.ctrlregs.K_inc>=Regs.ctrlregs.K_thread:Regs.ctrlregs.K + Regs.ctrlregs.K_inc<=Regs.ctrlregs.K_thread);
					int Knew = Kinit ? Regs.ctrlregs.K_init : Regs.ctrlregs.K + Regs.ctrlregs.K_inc;
					Regsupdate.ctrlregs.K =Kchange ? Knew : Regs.ctrlregs.K;
					
					/*J update*/
					bool Jchange = Kinit;
					bool Jinit = (Regs.ctrlregs.J_inc>0 ? Regs.ctrlregs.J + Regs.ctrlregs.J_inc>=Regs.ctrlregs.J_thread:Regs.ctrlregs.J + Regs.ctrlregs.J_inc<=Regs.ctrlregs.J_thread);
					int Jnew = Jinit ? Regs.ctrlregs.J_init : Regs.ctrlregs.J + Regs.ctrlregs.J_inc;
					Regsupdate.ctrlregs.J = Jchange? Jnew:Regs.ctrlregs.J;

					/*I update*/
					bool Ichange = Jinit;
					bool Iinit = (Regs.ctrlregs.I_inc>0 ? Regs.ctrlregs.I + Regs.ctrlregs.I_inc>=Regs.ctrlregs.I_thread:Regs.ctrlregs.I + Regs.ctrlregs.I_inc<=Regs.ctrlregs.I_thread);
					int Inew = Iinit ? Regs.ctrlregs.I_init : Regs.ctrlregs.I + Regs.ctrlregs.I_inc;
					Regsupdate.ctrlregs.I = Ichange ? Inew:Regs.ctrlregs.I;

					/*finishreg update*/
					Regsupdate.ctrlregs.Finish = (Regs.ctrlregs.IIcnt == Regs.ctrlregs.FinishIIcnt)&&(Regs.ctrlregs.Instcnt == Regs.ctrlregs.FinishInstcnt);
					
					/*fureg update*/
					bool fureg_wen = furesult.valid;
					if(fureg_wen) {Regsupdate.fureg = furesult.data;
					}

					/* print state update*/
#define STATES(f)\
					f(ctrlregs.Instcnt) f(ctrlregs.IIcnt) f(ctrlregs.Constcnt1) f(ctrlregs.Constcnt2) f(ctrlregs.Shiftconstcnt1) f(ctrlregs.Shiftconstcnt2) f(ctrlregs.K) f(ctrlregs.J)\
					f(ctrlregs.I)	f(ctrlregs.Finish) f(fureg)
#define PRINT_STATE_UPDATE(name)\
					std::cout<<#name<<" : "<<Regs.name<<" -> "<<Regsupdate.name<<std::endl;
					std::cout<<"Update state in PE:(current state-> next state)"<<endl;
					STATES(PRINT_STATE_UPDATE)
					std::cout<<std::endl;
#undef PRINT_STATE_UPDATE
#undef STATES

					/*link data update*/
					int crossbarouts[4];
					for(int i =0;i<4;i++){
					crossbarouts[i] = (this->*getsrclink[linkkeys[i]])(0);
					}
					bool linkout_wen[4];
					std::cout<<"Send data to Link:"<<std::endl;
			  	for(int i = 0;i <4;i++){linkout_wen[i]	= linkneedtosendout[i] &&(!linkinstskip[i]);}
					for(int i = 0; i< 4; ++ i){
						if(linkout_wen[i]){
							std::cout<<"send data "<<crossbarouts[i]<<" from source"<<linkkeys[i]<<" to link direction"<<i<<std::endl;
							outLinks[i]->Regsupdate.data = crossbarouts[i];
						}
					}
				}
}
bool CGRANode::CGRANodeFinish(){
	return Regs.ctrlregs.Finish;
}
/*finsih pending opts*/
void CGRANode::CGRANodefinish_pendingopts(){
	std::cout<<"CGRANode:"<<m_id<<" finish pending opts\n";
	if(pendingopts.size() > 0){
					for(auto it = pendingopts.begin();it != pendingopts.end();++it){
						if((*it).latency == 0){
							int fukey =(*it).key ;
							int src1 = (*it).src1;
							int src2 = (*it).src2;
				  		furesult = (this->*fuopts[fukey])(src1,src2);
						}
					}
					pendingopts.erase(remove_if(pendingopts.begin(),pendingopts.end(),[]( Opt t){return t.latency == 0;}),pendingopts.end());
					for(auto it = pendingopts.begin();it != pendingopts.end();++it){
						if((*it).latency > 0){
							(*it).latency --;
						}
					}
	}
}
