INC := -I./ac_math/include -I./ac_types/include

compile: main.cc main_16.cc main_fp.cc matrix.h
	g++ main.cc $(INC) -O3 -o main.elf
	g++ main_16.cc $(INC) -O3 -o main_16.elf
	g++ main_fp.cc $(INC) -D$(DTYPE) -O3 -o main_$(DTYPE).elf