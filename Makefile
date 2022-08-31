all: recvFromQ send2Q sockSvr consuming_cpu consuming_mem

CC=gcc -m64 
WARNING_FLAG = -Wconversion -Wfloat-equal \
				-Wdouble-promotion -Wall -Wextra -Wshadow \
				-Waggregate-return -Wbad-function-cast -Wcast-align \
				-Wformat=2 -Winit-self -Wjump-misses-init \
				-Wlogical-op -Wold-style-definition \
				-Wunused-macros -Wno-format-truncation \
				-Wvector-operation-performance -Wunsafe-loop-optimizations

LIB_GCOV=-lgcov
BIN = .
SEC_FLAG=-fsanitize=address -fsanitize-recover=all
CCFLAGS = -g  -std=gnu18 $(WARNING_FLAG) $(SEC_FLAG) -I${INCPATH} ${DEF}

LIBS = -pthread -lm -lrt -fsanitize=address -fsanitize-recover=all -lasan

SRCS = recvFromQ.c send2Q.c sockSvr.c consuming_mem.c consuming_cpu.c

OBJS =	$(SRCS:.c=.o)

%.o:%.c
	${CC} -c ${CCFLAGS} -I$(INCPATHLLM) $< -o $@

recvFromQ: recvFromQ.o
	${CC} -o ${BIN}/$@ $^ ${LIBS} 

send2Q: send2Q.o
	${CC} -o ${BIN}/$@ $^ ${LIBS}

sockSvr: sockSvr.o
	${CC} -o ${BIN}/$@ $^ ${LIBS}

consuming_cpu: consuming_cpu.o
	${CC} -o ${BIN}/$@ $^ ${LIBS}

consuming_mem: consuming_mem.o
	${CC} -o ${BIN}/$@ $^ ${LIBS}

#clang_analyze: 
	#clang --analyze -Xanalyzer -analyzer-output=text  ${CCFLAGS} -I$(INCPATHLLM) $(SRCS)

#cppcheck:
#	cppcheck --enable=all $(SRCS)

#install:
#	cp -p ${BIN}/SBltSvr ${HOME}/ts.bin

clean:
	@rm -f *.o ${BIN}/recvFromQ ${BIN}/send2Q ${BIN}/sockSvr
