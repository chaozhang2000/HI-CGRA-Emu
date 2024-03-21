#include "encode.h"

string opt_encode_name_map[30];

#define _OPT_INIT_MAP(k) opt_encode_name_map[FU_##k] = #k;
void init_optcode_name_map(){
	opt_encode_name_map[0] = "empty";
	OPTS(_OPT_INIT_MAP)
	
}
