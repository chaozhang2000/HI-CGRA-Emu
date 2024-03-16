#ifndef data_H
#define data_H
struct Src{
	int data;
	bool valid;
};

struct Link{
	int data;
	bool valid;
	bool ready;
};
#endif
