CXX = c++
CXX_FLAGS = -O2 -std=c++11 -pipe -pthread

EXEC = bbq_test
SRCS = test.cc
HDRS = bbq.h

${EXEC} : ${SRCS} ${HDRS}
	${CXX} ${SRCS} ${CXX_FLAGS} -o ${EXEC}

.PHONY: test
test : ${EXEC}
	./${EXEC}

.PHONY: clean 
clean :
	rm ${EXEC}