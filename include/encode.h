#ifndef encode_H
#define encode_H
#include <string>
using namespace std;
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
#define OPTS(f) \
  f(mul) f(add) f(getelementptr) f(load) f(store) f(shl) f(or) f(sext) f(and) f(sub) f(ashr)
#define _OPT_NAME(k) FU_##k,

enum {  FU_EMPTY = 0,
				OPTS(_OPT_NAME)
};

extern string opt_encode_name_map[30];
void init_optcode_name_map();
#endif
