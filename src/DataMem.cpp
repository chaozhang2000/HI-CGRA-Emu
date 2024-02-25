#include "DataMem.h"
#include <cassert>
DataMem::DataMem(int id) {
    m_id = id;
    for (int i = 0; i < CONFIG_CGRA_DATAMEM_SIZE; ++i) {
        data[i] = 0; 
    }
}
void DataMem::writeData(int address, int data) {
	assert(address >= 0 && address < CONFIG_CGRA_DATAMEM_SIZE);
	this->data[address] = data;
}
int DataMem::readData(int address) {
	assert(address >= 0 && address < CONFIG_CGRA_DATAMEM_SIZE);
	return data[address];
}
void DataMem::writeDatas(int * src,int address, int length){
	assert(length > 0);
	assert(address>=0 && address+length -1 <CONFIG_CGRA_DATAMEM_SIZE);
	for(int i =0;i<length;i++){
		data[address + i] = src[i];
	}
}
