CC=gcc


.PHONY: all mandatory extra

all: MMULT.c row_calc.c row_calc.h test_case_constants.h
	$(CC) -D TEST_CASES_COMMON_OPERATIONS -o MMULT $^ -lpthread -lrt

mandatory: MMULT.c row_calc.c row_calc.h test_case_constants.h
	$(CC) -o MMULT $^ -lpthread -lrt

extra: MMULT.c row_calc.c row_calc.h test_case_constants.h
	$(CC) -D DEACTIVATE_MANDATORY_TEST -o MMULT $^ -lpthread -lrt

