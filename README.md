# README

## Quick start

Ubuntu22.04

1. install dependency
```
sudo apt-get install make build-essential bison flex libncurses-dev
```

2. add EMU\_HOME to ~/.bashrc  
	1. vim ~/.bashrc then add at the last line,for example "export CGRA\_EMU\_HOME=/home/user/HIT-CGRA-Mapper"
	2. source ~/.bashrc

3. config HIT-CGRA-Emu and compile
```
make menuconfig
make
```

4. run demo
```
make defconfig
make run
```
