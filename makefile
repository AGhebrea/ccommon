# todo: it has issues. Like not having cleaned the bin folders before starting another build.

INPUT_FILES=$(wildcard ./src/$@/*.c)
INCLUDE_FILES=$(wildcard ./src/$@/include/*.h)

BIN_PATH=./src/$@/bin

OUTPUT_FILES=$(wildcard ./src/$@/bin/*.o)
OUTPUT=./bin/lib$@.a

INSTALL_LIB_DIR:=/usr/lib/ccommon
INSTALL_INCLUDE_DIR:=/usr/include/ccommon
FLAGS:=-I./include -ggdb3 -Wall -lm

TEST_MAIN_FILE:=./test/main.c
TEST_FILES:=$(wildcard ./test/*.c)
TEST_FILES:=$(filter-out ${TEST_MAIN_FILE}, $(TEST_FILES))
TEST_OUTPUT:=./test/bin/testrun
TEST_FLAGS:=-L./bin/ -I./test/include -lcclog -lccstd -Wall -ggdb3 -lm

# idk
define RULE_TEXT
	@if [ ! -d ${BIN_PATH} ]; then mkdir ${BIN_PATH}; fi
	$(foreach file,${INPUT_FILES},gcc ${file} -c -o ${BIN_PATH}/$(basename $(notdir ${file})).o ${FLAGS};)
	ar rcs ${OUTPUT} ${OUTPUT_FILES}
	@if [ ! -d ${INSTALL_LIB_DIR} ]; then sudo mkdir ${INSTALL_LIB_DIR}; fi
	sudo cp ${OUTPUT} ${INSTALL_LIB_DIR}/
	@if [ ! -d ${INSTALL_INCLUDE_DIR} ]; then sudo mkdir ${INSTALL_INCLUDE_DIR}; fi
	sudo cp ${INCLUDE_FILES} ${INSTALL_INCLUDE_DIR}/
endef

all: 
	$(MAKE) cclog
	$(MAKE) ccstd
	$(MAKE) ccdebug
	$(MAKE) test

.PHONY: cclog
cclog:
	${RULE_TEXT}

.PHONY: ccstd
ccstd:
	${RULE_TEXT}

.PHONY: ccdebug
ccdebug:
	${RULE_TEXT}

.PHONY: test
test:
	gcc ${TEST_MAIN_FILE} ${TEST_FILES} ${BUILD_FILES} -o ${TEST_OUTPUT} ${TEST_FLAGS}
	${TEST_OUTPUT}

clean:
	rm ./bin/lib*.a
	rm ./test/bin/testrun