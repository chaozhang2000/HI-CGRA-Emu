#ifndef CGRANode_H
#define CGRANode_H

#include <list>
#include <string>
#include <set>
#include "BitStream.h"
#include "DataMem.h"
#include "data.h"

using namespace std;
class CGRALink;

typedef CGRAInstruction CGRANodeInst;
struct CGRANodeRegs{
	CtrlRegs ctrlregs;
	int fureg;
};
class CGRANode{

  private:
    int m_id;
    int m_x;
    int m_y;
		/**true main disable the CGRANode
		 */
    bool m_disabled;
		/**if this PE has a DataMem if it has DataMem, it can load or store.
		 */
		bool m_hasDataMem;
		/**the list to record input CGRALinks of this CGRANode
		 */
    list <CGRALink*> m_inLinks;
		CGRALink* inLinks[4];
		/**the list to record output CGRALinks of this CGRANode
		 */
    list <CGRALink*> m_outLinks;
		CGRALink* outLinks[4];

		/**the set to record all opname that this CGRANode is support
		 */
		set<string> m_supportOpts;

		/**the list to record all neighbor CGRANode
		 */
		list<CGRANode*>* m_neighbors;

		CGRANodeRegs Regs;
		CGRANodeRegs Regsupdate;
		CGRANodeInst InstMem[CONFIG_CGRA_INSTMEM_SIZE];
		int ConstMem1[CONFIG_CGRA_CONSTMEM_SIZE];
		int ConstMem2[CONFIG_CGRA_CONSTMEM_SIZE];
		int ShiftconstMem1[CONFIG_CGRA_SHIFTCONSTMEM_SIZE];
		int ShiftconstMem2[CONFIG_CGRA_SHIFTCONSTMEM_SIZE];

		typedef Src(CGRANode::*getSrcPtr)(int key);
		getSrcPtr getsrc[10];
		Src getsrcnull(int key);
		Src getsrcfromFureg(int key);
		Src getsrcformconstmem(int key);
		Src getsrcfromNlink(int key);
		Src getsrcfromSlink(int key);
		Src getsrcfromWlink(int key);
		Src getsrcfromElink(int key);
		Src getsrcfromloop0reg(int key);
		Src getsrcfromloop1reg(int key);
		Src getsrcfromloop2reg(int key);

		typedef Src(CGRANode::*fuExecPtr)(Src src1,Src src2);
		fuExecPtr fuopts[7];
		Src emptyopt(Src src1, Src src2);
		Src addopt(Src src1, Src src2);
		Src mulopt(Src src1, Src src2);
		Src loadopt(Src src1, Src src2);
		Src storeopt(Src src1, Src src2);
		Src shlopt(Src src1,Src src2);

		getSrcPtr getsrclink[8];
		Src furesult;
		Src getsrcfromFu(int key);

  public:
		DataMem* datamem;
		void CGRANodeReset();
		void CGRANodeUpdate();
		void CGRANodeLoadBitStream(BitStreamInfoPE* PEbitstream);
		void CGRANodeExecOnecycle();
		bool CGRANodeFinish();
		/**The constructor function of class CGRANode
		 * this function init CGRANode's ID,x and y according the params,other var is init by default value.
		 * all opts like load store add,mul,shift and so on is turned on by default
		 * @param t_id : the id of the CGRANode
		 * @param t_x : the x of the CGRANode
		 * @param t_y : the y of the CGRANode 
		 */
		CGRANode(int t_id, int t_x, int t_y);
		~CGRANode();

		/**add the CGRALink to the list of this CGRANode's InCGRALinks
		 * @param t_link : the pointer of the in CGRALink
		 */
		void attachInLink(CGRALink* t_link);

		/**add the CGRALink to the list of this CGRANode's outCGRALinks
		 * @param t_link : the pointer of the out CGRALink
		 */
		void attachOutLink(CGRALink* t_link);


		int getID();
		/**return m_x
		 */
		int getx();
		/**return m_y
		 */
		int gety();
		/**return m_hasDataMem
		 */
		bool hasDataMem();
		/**return m_disabled
		 */
		bool isdisable();

		/**judge if this CGRANode can support Opt which opcodeName is t_optsname
		 * @param t_optsname: the name of the Opt
		 */
		bool canSupport(string t_optsname);

		/**get the neighbors CGRANode of this Node
		 * return m_neighbors,if m_neighbors == NULL will find neighbors first
		 */
		list<CGRANode*>* getNeighbors();

		/**get the out CGRALink form this CGRANode to neighbor Node t_neighbor
		 * if not found return NULL
		 */
		CGRALink* getOutLinkto(CGRANode* t_neighbor);
};
#endif
