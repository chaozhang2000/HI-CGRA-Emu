#include "DataMem.h"
#include <cassert>
#include <iostream>
#include "config.h"
DataMem::DataMem(int id) {
    m_id = id;
		data = new int[config_info.datamemsize];
    for (int i = 0; i < config_info.datamemsize; ++i) {
        data[i] = 0; 
    }
}
void DataMem::writeData(int address, int data) {
	assert(address >= 0 && address < config_info.datamemsize);
	this->data[address] = data;
	std::cout<<"storew data "<<data <<" to address "<<address<<std::endl;
}
int DataMem::readData(int address) {
	assert(address >= 0 && address < config_info.datamemsize);
	return data[address];
}
void DataMem::writeDatas(int * src,int address, int length){
	assert(length > 0);
	assert(address>=0 && address+length -1 <config_info.datamemsize);
	for(int i =0;i<length;i++){
		data[address + i] = src[i];
	}
}
Src DataMem::fureadData(int address) {
	assert(address >= 0 && address < config_info.datamemsize);
	return {data[address],true};
}

DataMem::~DataMem(){
	delete [] data;
}
