#include "config.h"
#include "json.h"
#include <fstream>
#include "utils.h"
using namespace std;
using json = nlohmann::json;

CONFIG_INFO config_info;
bool getconfig(CONFIG_INFO* config_info,std::string& filename)
{

			bool hasJson= false;
      ifstream param(filename);
			if(param.good())hasJson= true;

      if (!hasJson) {
        OUTS("=============================================================\n",ANSI_FG_RED);
				OUTS("Please provide a valid <param.json> in current directory.",ANSI_FG_RED);
				OUTS("A set of default parameters is leveraged.",ANSI_FG_RED);
        OUTS("=============================================================\n",ANSI_FG_RED);
				return false;
      }else{
        json configs;
				param >> configs;
	config_info->kernel= configs["kernel"];
	config_info->loopargnum = configs["loopargnum"];
	config_info->rows = configs["rows"];
	config_info->cols = configs["cols"];
	config_info->instmemsize = configs["instmemsize"];
	config_info->constmemsize = configs["constmemsize"];
	config_info->shiftconstmemsize = configs["shiftconstmemsize"];
	config_info->datamemsize = configs["datamemsize"];
	config_info->mrrgsize = configs["mrrgsize"];
	config_info->loop0start = configs["loop0start"];
	config_info->loop0inc = configs["loop0inc"];
	config_info->loop0end = configs["loop0end"];
	config_info->loop1start = configs["loop1start"];
	config_info->loop1inc = configs["loop1inc"];
	config_info->loop1end = configs["loop1end"];
	config_info->loop2start = configs["loop2start"];
	config_info->loop2inc = configs["loop2inc"];
	config_info->loop2end = configs["loop2end"];
	config_info->maxsimcycle= configs["maxsimcycle"];
				return true;
			}
}
