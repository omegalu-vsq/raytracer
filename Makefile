CC = g++

SKEL_CPP_FILES = image.cpp point.cpp vector.cpp texture.cpp shape.cpp light.cpp camera.cpp
SKEL_HH_FILES  = image.hh point.hh vector.hh texture.hh shape.hh scene.hh light.hh camera.hh

CPP_FILES =  $(SKEL_CPP_FILES)
H_FILES =  $(SKEL_HH_FILES)
OBJ_FILES = $(CPP_FILES:.cpp=.o)

CXX_FLAGS += -Wall -Wextra -O3 -g -std=c++20
CXX_FLAGS += -lm
CXX_FLAGS += -march=native
#CXX_FLAGS += -fopt-info-vec-optimized -fopt-info-vec-missed -ftree-vectorize
LDXX_FLAGS =

#For gcc 4.9
#CXXFLAGS+=-fdiagnostics-color=auto
export GCC_COLORS=1

define color
    if test -n "${TERM}" ; then\
	if test `tput colors` -gt 0 ; then \
	    tput setaf $(1); \
        fi;\
    fi
endef

define default_color
    if test -n "${TERM}" ; then\
	if test `tput colors` -gt 0 ; then  tput sgr0 ; fi; \
    fi
endef


all: post-build

pre-build:
	@$(call color,4)
	@echo "******** Starting Compilation ************"
	@$(call default_color)

post-build:
	@make --no-print-directory main-build ; \
	sta=$$?;	  \
	$(call color,4); \
	echo "*********** End Compilation **************"; \
	$(call default_color); \
	exit $$sta;

main-build: pre-build build

build: $(OBJ_FILES)
	$(CC) tp1.cpp -o tp1 $(OBJ_FILES) $(CXX_FLAGS) $(LDXX_FLAGS)
	#$(CC) tp2.cpp -o tp2 $(OBJ_FILES) $(CXX_FLAGS) $(LDXX_FLAGS)

%.o: %.cpp %.hh
	@$(call color,2)
	@echo "[$@] $(CXX_FLAGS)"
	@$(call default_color)
	@$(CC) -c -o $@ $< $(CXX_FLAGS) ; \
	sta=$$?;	  \
	if [ $$sta -eq 0 ]; then  \
	  $(call color,2) ; \
	  echo "[$@ succes]" ; \
	  $(call default_color) ; \
	else  \
	  $(call color,1) ; \
	  echo "[$@ failure]" ; \
	  $(call default_color) ; \
	fi ;\
	exit $$sta

.PHONY: all clean pre-build post-build main-build build

clean:
	rm -f $(OBJ_FILES) tp1

