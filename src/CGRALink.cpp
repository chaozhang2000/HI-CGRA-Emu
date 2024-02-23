#include "CGRALink.h"
#include <cstring>

CGRALink::CGRALink(int t_linkId, int t_direction) {
	m_id = t_linkId;
	m_direction = t_direction;
	CGRALinkReset();
}

void CGRALink::connect(CGRANode* t_src, CGRANode* t_dst) {
  m_src = t_src;
  m_dst = t_dst;
}

int CGRALink::getID(){
	return m_id;
}

CGRANode* CGRALink::getsrc(){
	return m_src;
}

CGRANode* CGRALink::getdst(){
	return m_dst;
}

int CGRALink::getdirection(){
	return m_direction;
}

void CGRALink::CGRALinkReset(){
	memset(&Regs,0,sizeof(CGRALinkRegs));
	memset(&Regsupdate,0,sizeof(CGRALinkRegs));
}

void CGRALink::CGRALinkUpdate(){
	memcpy(&Regs,&Regsupdate,sizeof(CGRALinkRegs));
}
