CXX			= g++
CXXFLAGS	= -O2 -Werror -Wall -Wextra -Wpedantic -std=c++17 \
	-I./include \
	-Wno-unused-function -Wno-unused-parameter -Wno-unused-variable \
	-Wno-format-nonliteral -Wcast-qual -Wconversion \
	-Wformat=2 -Wformat-security \
	-Wnull-dereference -Wstack-protector -Wtrampolines -Wvla \
	-Warray-bounds=2 \
	-Wshift-overflow=2 \
	-Wlogical-op -Wduplicated-cond -Wno-misleading-indentation \
	-Wformat-signedness -Wstrict-overflow=4 \
	-Wundef -Wswitch-enum \
	-Wstack-usage=10000 \
	-D_FORTIFY_SOURCE=2 

# -Wshadow 	

LDFLAGS		= -g -s
LDLIBS		= -lm 

DOC			= doc
SRC			= src
CPPW		= *.cpp
TARGET     	= pca
SRC_FILES 	= $(wildcard $(SRC)/$(CPPW)) $(wildcard$(SRC)/**/$(CPPW))
OBJECTS 	= $(SRC_FILES:%.cpp=%.o)

.SUFFIXES = 

.PHONY: all

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

.PHONY: doc
doc:
	@doxygen $(DOC)/wxtest.conf

.PHONY: cleandoc
cleandoc:
	@rm -rf $(DOC)/html
	@echo "clean doc done"

.PHONY: clean
clean:
	@rm -rf $(TARGET) $(OBJECTS)
	@echo "clean done"

.PHONY: check
check:
	@cppcheck -I./include \
		--check-config \
		--enable=all \
		--std=c++11 \
		--suppress=missingIncludeSystem \
		$(SRC)	

