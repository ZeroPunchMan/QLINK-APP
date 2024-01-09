
CurDir := $(shell pwd)
SrcDir := $(CurDir)/src/
IncDir := $(CurDir)/inc/

Headers := $(shell ls ./inc)
Sources := $(shell ls ./src)
Ojects := $(shell for src in $(Sources); do echo -e "$${src/%.c/.o} \c"; done;)
LibName := libcl
LibLink := cl

HeadPath := $(shell for src in $(shell ls ./inc); do echo -e $(IncDir)/$$src; done;)
SrcPath := $(shell for src in $(shell ls ./src); do echo -e $(SrcDir)/$$src; done;)
#ClibObj := $(shell for src in $(SrcPath); do echo -e "$${src/%.c/.o} \c"; done;)

OutputDir := ./build/debug/

# add test case here
TestCases := queue_test pool_test list_test event_system_test
# TestCases := event_system_test
TestCaseDir := $(CurDir)/testcase/

DBG ?= -g

# test:
# 	@echo $(Ojects)
# 	@echo $(SrcPath)
# 	@echo $(HeadPath)

all: testcase
	

genslib: $(HeadPath) $(SrcPath)
	@echo generating static library
	@mkdir -vp $(OutputDir)
	@for src in $(Sources); do\
		gcc -g -std=c99 -I $(IncDir) -c $(SrcDir)/$$src -o $(OutputDir)/$${src/%.c/.o};\
	done  
	@rm -f $(OutputDir)/*.a
	@for obj in $(Ojects); do\
		ar -r $(OutputDir)/$(LibName).a $(OutputDir)/$${obj}; \
	done

testcase: FORCE genslib 
	@for tc in $(TestCases); do\
		echo ----start $${tc}----; \
		gcc -g -std=c99 -I $(IncDir) -c $(TestCaseDir)/$${tc}.c -o  $(OutputDir)/$${tc}.o; \
		gcc -g $(OutputDir)/$${tc}.o -std=c99 -I $(IncDir) -L$(OutputDir) -l$(LibLink)  -o $(OutputDir)/$${tc}.exe; \
		$(OutputDir)/$${tc}.exe; \
	done

.PHONY: FORCE
FORCE:

.PHONY: clean
clean:
	@rm -rf ./build/*

