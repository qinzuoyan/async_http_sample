CXX=g++
OPT=-O2
INCPATH=-I. -Ilibashttp -Ilibashttp/ashttp -Itemplog
CXXFLAGS += $(OPT) -std=c++11 -Wno-narrowing -D_GNU_SOURCE -D__STDC_LIMIT_MACROS $(INCPATH)
LDFLAGS += -L. -lashttp -ltemplog -lboost_system -lcrypto -lssl -lpthread -lrt

ASHTTP_LIB=libashttp.a
ASHTTP_SRC=$(wildcard libashttp/ashttp/*.cpp libashttp/ashttp/client/*.cpp)
ASHTTP_OBJ=$(patsubst %.cpp,%.o,$(ASHTTP_SRC))

TEMPLOG_LIB=libtemplog.a
TEMPLOG_SRC=$(wildcard templog/imp/*.cpp)
TEMPLOG_OBJ=$(patsubst %.cpp,%.o,$(TEMPLOG_SRC))

TEST_BIN=http_client_test
TEST_SRC=$(wildcard libashttp/test/*.cpp)
TEST_OBJ=$(patsubst %.cpp,%.o,$(TEST_SRC))

all: $(TEST_BIN)

.PHONY: clean

clean:
	@rm -f $(ASHTTP_OBJ) $(ASHTTP_LIB) $(TEMPLOG_OBJ) $(TEMPLOG_LIB) $(TEST_OBJ) $(TEST_BIN)

rebuild: clean all

libashttp.a: $(ASHTTP_OBJ)
	ar rcs $@ $^

libtemplog.a: $(TEMPLOG_OBJ)
	ar rcs $@ $^

http_client_test: $(TEST_OBJ) $(ASHTTP_LIB) $(TEMPLOG_LIB)
	$(CXX) $(TEST_OBJ) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

