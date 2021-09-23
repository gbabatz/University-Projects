SUFFIX = .exe

all: bin/basic_a.exe bin/basic_b.exe bin/basic_c.exe bin/wbuffer.exe 

bin/basic_a.exe : src/full_loop_fusion_a.c
	gcc -o $@ $? 

bin/basic_b.exe : src/full_loop_fusion_b.c
	gcc -o $@ $? 

bin/basic_c.exe : src/full_loop_fusion_c.c
	gcc -o $@ $? 

bin/wbuffer.exe : src/bonus_full_loop_fusion.c
	gcc -o $@ $? 

