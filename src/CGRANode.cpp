#include "CGRANode.h"
#include "CGRALink.h"
#include "utils.h"
#include <cstring>

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
}

void CGRANode::attachOutLink(CGRALink* t_link) {
  m_outLinks.push_back(t_link);
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
				std::cout << datamem->readData(0) <<std::endl;
				std::cout << datamem->readData(1) <<std::endl;
}
