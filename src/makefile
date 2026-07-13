FLAGS=-g -Wall -std=c99 

ray3: structs.h lin_alg.h stb_image_write.h main.c 
	gcc ${FLAGS} -o ray3 main.c -lm

.PHONY: clean
clean:
	rm -f sphere.png ray3 vgcore.*

.PHONY: gprof
gprof:
	gcc -pg ${FLAGS} -o ray3 main.c -lm
	./ray3
	gprof ray3 gmon.out > prof.out

.PHONY: clean_gprof
clean_gprof:
	rm -f prof.out 
	rm -f gmon.out

.PHONY: submission
submission:
	cp main.c ../turnins/3/main.c
	cp lin_alg.h ../turnins/3/lin_alg.h
	cp stb_image_write.h ../turnins/3/stb_image_write.h
	cp structs.h ../turnins/3/structs.h
	cp makefile ../turnins/3/makefile
	tar -czvf prog3.tgz ../3prog/
	



