INC := -I../ac_math/include -I../ac_types/include

VALID_DTYPES := FP32 BF16 FP8_E5M2 FP8_E4M3

ifndef DTYPE
$(error DTYPE is required. Valid options: $(VALID_DTYPES). [USAGE: make DTYPE=BF16])
endif

ifneq ($(filter $(DTYPE),$(VALID_DTYPES)),$(DTYPE))
$(error Invalid DTYPE: $(DTYPE). Valid options: $(VALID_DTYPES). [USAGE: make DTYPE=BF16])
endif

compile: main.cc main_16.cc main_fp.cc matrix.h
	g++ main.cc $(INC) -O3 -o main.elf
	g++ main_16.cc $(INC) -O3 -o main_16.elf
	g++ main_fp.cc $(INC) -D$(DTYPE) -O3 -o main_$(DTYPE).elf