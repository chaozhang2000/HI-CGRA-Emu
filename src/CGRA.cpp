#include "CGRA.h"
#include "utils.h"
#include "generated/autoconf.h"
#include "DataMem.h"
#include "config.h"

/**
 * TODO: now the cgra is connect in a specific way,and every Node can support all opts, the conection and the opts should be defined by users.
 *
 * the default CGRA
 * |---| |---| |---| |---|
 * |	 | |	 | |	 | |	 | row3
 * |---| |---| |---| |---|
 *
 * |---| |---| |---| |---|
 * |	 | |	 | |	 | |	 | row2
 * |---| |---| |---| |---|
 *
 * |---| |---| |---| |---|
 * | 4 | |...| |	 | |	 | row1   ^ y
 * |---| |---| |---| |---|        |
 *                                |
 * |---| |---| |---| |---|        |
 * | 0 | | 1 | | 2 | | 3 | row0		|
 * |---| |---| |---| |---|        |
 * col0  col1  col2  col3					|
 * --------------------------------> x
 * What is in this Function:
 * 1. init some var,like m_rows、m_columns、m_FUCount and so on,new nodes and links.
 * 2. connect the CGRANode and CGRALink to Generate the CGRA.
 * 3. print CGRA information if needed.
 */
CGRA::CGRA(int t_rows,int t_columns){
	
 	//1. init some var,like m_rows、m_columns、m_FUCount and so on,new nodes and links.
  m_rows = t_rows;
  m_columns = t_columns;
  m_FUCount = t_rows * t_columns;
  nodes = new CGRANode**[t_rows];
  int node_id = 0;
  for (int i=0; i<t_rows; ++i) {
  	nodes[i] = new CGRANode*[t_columns];
    for (int j=0; j<t_columns; ++j) {
    	nodes[i][j] = new CGRANode(node_id++, j, i);
    }
  }
  m_LinkCount = 2 * (t_rows * (t_columns-1) + (t_rows-1) * t_columns);
  links = new CGRALink*[m_LinkCount];

  //2. connect the CGRANode and CGRALink to Generate the CGRA.
  int link_id = 0;
  for (int i=0; i<t_rows; ++i) {
  	for (int j=0; j<t_columns; ++j) {
			//to N
			if (i < t_rows - 1) {
      	links[link_id] = new CGRALink(link_id,LINK_DIRECTION_TO_N);
        nodes[i][j]->attachOutLink(links[link_id]);
        nodes[i+1][j]->attachInLink(links[link_id]);
        links[link_id]->connect(nodes[i][j], nodes[i+1][j]);
        ++link_id;
			}
			//to S
      if (i > 0) {
      	links[link_id] = new CGRALink(link_id,LINK_DIRECTION_TO_S);
        nodes[i][j]->attachOutLink(links[link_id]);
        nodes[i-1][j]->attachInLink(links[link_id]);
        links[link_id]->connect(nodes[i][j], nodes[i-1][j]);
        ++link_id;
      }
			//to E
      if (j < t_columns - 1) {
        links[link_id] = new CGRALink(link_id,LINK_DIRECTION_TO_E);
        nodes[i][j]->attachOutLink(links[link_id]);
        nodes[i][j+1]->attachInLink(links[link_id]);
        links[link_id]->connect(nodes[i][j], nodes[i][j+1]);
        ++link_id;
      }
			//to W
      if (j > 0) {
        links[link_id] = new CGRALink(link_id,LINK_DIRECTION_TO_W);
        nodes[i][j]->attachOutLink(links[link_id]);
        nodes[i][j-1]->attachInLink(links[link_id]);
        links[link_id]->connect(nodes[i][j], nodes[i][j-1]);
        ++link_id;
			}
    }
  }
	//3. add datamems
	datamems =new DataMem* [config_info.datamemnum];
	for(int i = 0; i< config_info.datamemnum ;i ++){
		 datamems[i] = new DataMem(i);
		 for(unsigned long j = 0;j< config_info.datamemaccess[i].size();j++){
			int row = config_info.datamemaccess[i][j] / t_columns;
			int col = config_info.datamemaccess[i][j] % t_columns;
			nodes[row][col]->datamem = datamems[i];
		 }
	}
 	//3. print CGRA information if needed.
#ifdef CONFIG_CGRA_INFO
	//dump CGRA
  OUTS("\nCGRA Info",ANSI_FG_BLUE);
	OUTS("==================================",ANSI_FG_CYAN); 
  OUTS("[CGRA Node and links count]",ANSI_FG_CYAN);
	std::cout<<"CGRArows:"<<m_rows<<"\n";
	std::cout<<"CGRAcolumns:"<<m_columns<<"\n";
	std::cout<<"CGRANode:"<<m_FUCount<<"\n";
	std::cout<<"CGRALink:"<<m_LinkCount<<"\n";

	OUTS("==================================",ANSI_FG_CYAN); 
  OUTS("[CGRA Node and links information]",ANSI_FG_CYAN);
  for (int i=0; i<m_rows; ++i) {
    for (int j=0; j<m_columns; ++j) {
			std::cout<< "Node("<<nodes[i][j]->getx()<<","<<nodes[i][j]->gety()<<"); ";
			std::cout<< "ID:"<<nodes[i][j]->getID()<<"; ";
			std::cout<< "hasDataMem:"<<nodes[i][j]->hasDataMem()<<"\n";
    }
  }
	for (int i=0; i<m_LinkCount;i++){
		std::cout<<"Link"<<links[i]->getID()<<":from Node"<<links[i]->getsrc()->getID()<<"->Node"<<links[i]->getdst()->getID()<<" direction:"<<links[i]->getdirection()<<"\n";
	}
#endif
}

int CGRA::getNodeCount(){
	return m_FUCount;
}

int CGRA::getLinkCount(){
	return m_LinkCount;
}

int CGRA::getrows(){
	return m_rows;
}

int CGRA::getcolumns(){
	return m_columns;
}

CGRALink* CGRA::getLinkfrom(CGRANode* t_src,CGRANode* t_dst){
	for(int i = 0;i<m_LinkCount;i++){
		if(links[i]->getsrc()== t_src and links[i]->getdst() == t_dst)
			return links[i];
	}
	return NULL;
}

CGRA::~CGRA(){
	for (int i=0; i<m_LinkCount;i++){
					delete links[i];
	}
  for (int i=0; i<m_rows; ++i) {
    for (int j=0; j<m_columns; ++j) {
			delete nodes[i][j];
		}
	}
	for (int i=0;i<m_rows;i++){
		delete[] nodes[i];
	}
	for(int i = 0; i< config_info.datamemnum ;i ++){
		 delete datamems[i];
	}
	delete[] datamems;
	delete[] nodes;
	delete[] links;
}

void CGRA::CGRAReset(){
  for (int i=0; i<m_rows; ++i) {
    for (int j=0; j<m_columns; ++j) {
			nodes[i][j]->CGRANodeReset();
    }
  }
	for (int i=0; i<m_LinkCount;i++){
		links[i]->CGRALinkReset();
	}
}

void CGRA::CGRALoadBitStream(BitStreamInfo* bitstream){
  for (int i=0; i<m_rows; ++i) {
    for (int j=0; j<m_columns; ++j) {
			nodes[i][j]->CGRANodeLoadBitStream(&(bitstream->BitstreaminfoOfPE[m_rows*i+j]));
    }
  }
}

void CGRA::CGRAExecOnecycle(){
  for (int i=0; i<m_rows; ++i) {
    for (int j=0; j<m_columns; ++j) {
			nodes[i][j]->CGRANodeExecOnecycle();
    }
	}
  for (int i=0; i<m_rows; ++i) {
    for (int j=0; j<m_columns; ++j) {
			nodes[i][j]->CGRANodeUpdate();
    }
	}
	for(int i = 0;i<m_LinkCount;i++){
		links[i]->CGRALinkUpdate();
	}
}

bool CGRA::IfFinished(){
  for (int i=0; i<m_rows; ++i) {
    for (int j=0; j<m_columns; ++j) {
			if(nodes[i][j]->CGRANodeFinish() == false) return false;
    }
	}
	return true;
}
