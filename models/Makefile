OBJS=$(wildcard *.obj)
COBJS=$(patsubst %.obj, %.cobj, $(OBJS))

all: $(COBJS)

%.cobj: %.obj
	python ../scripts/compile_obj.py $< $@

