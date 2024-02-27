enum {  SRC_NOT_OCCUPY,
				SRC_OCCUPY_FROM_FU,
				SRC_OCCUPY_FROM_CONST_MEM,
				SRC_OCCUPY_FROM_N, 
				SRC_OCCUPY_FROM_S,
				SRC_OCCUPY_FROM_W,
				SRC_OCCUPY_FROM_E,
				SRC_OCCUPY_FROM_LOOP0,
				SRC_OCCUPY_FROM_LOOP1,
				SRC_OCCUPY_FROM_LOOP2};

enum {  LINK_NOT_OCCUPY,
				LINK_OCCUPY_EMPTY,
				LINK_OCCUPY_FROM_N, 
				LINK_OCCUPY_FROM_S,
				LINK_OCCUPY_FROM_W,
				LINK_OCCUPY_FROM_E,
				LINK_OCCUPY_FROM_FU,
				LINK_OCCUPY_FROM_FUREG};

enum {  FU_EMPTY,
				FU_MUL,
				FU_ADD,
				FU_GETELEMENTPTR, 
				FU_LOAD,
				FU_STORE};
