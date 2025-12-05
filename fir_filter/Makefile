INC := -I../ac_math/include -I../ac_types/include

VALID_DTYPES := FP32 BF16 FP8_E5M2 FP8_E4M3
VALID_QTYPES := INT8 INT16

ifndef DTYPE
$(error DTYPE is required. Valid options: $(VALID_DTYPES). [USAGE: make DTYPE=BF16])
endif

ifneq ($(filter $(DTYPE),$(VALID_DTYPES)),$(DTYPE))
$(error Invalid DTYPE: $(DTYPE). Valid options: $(VALID_DTYPES). [USAGE: make DTYPE=BF16])
endif

ifndef QTYPE
$(error QTYPE is required. Valid options: $(VALID_QTYPES). [USAGE: make QTYPE=INT8])
endif

ifneq ($(filter $(QTYPE),$(VALID_QTYPES)),$(QTYPE))
$(error Invalid QTYPE: $(QTYPE). Valid options: $(VALID_QTYPES). [USAGE: make QTYPE=INT8])
endif

compile: main.cc main_fp.cc filter.h
	g++ main.cc $(INC) -D$(QTYPE) -O3 -o main.elf
# 	g++ main_fp.cc $(INC) -D$(DTYPE) -O3 -o main_$(DTYPE).elf