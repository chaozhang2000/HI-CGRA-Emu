#ifndef DataMem_H
#define DataMem_H

#include "generated/autoconf.h"
class DataMem
{
  private:
    int m_id;

		int data[CONFIG_CGRA_DATAMEM_SIZE];
		
  public:

		DataMem(int id);
		int readData(int address);
		void writeData(int address,int data);

		void writeDatas(int * src,int address, int length);

};
#endif
