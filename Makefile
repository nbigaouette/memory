#################################################################
# Main makefile
# Type "make help" for usage
#################################################################

# Project options
LIB             := memory
BIN              = $(LIB)_test
SRCDIRS          = src
TESTDIRS         = unit_testing
SRCEXT           = cpp
HEADEXT          = hpp
HEADERS          = $(wildcard $(addsuffix *.$(HEADEXT),$(addsuffix /, $(SRCDIRS)) ) )
LANGUAGE         = CPP

# Include the generic rules
include makefiles/Makefile.rules

#################################################################
# Project specific options

LIB_OBJ          = $(OBJ)

# Project is a library. Include the makefile for build and install.
include makefiles/Makefile.library

$(eval $(call Flags_template,stdcout,StdCout.hpp,ssh://optimusprime.selfip.net/git/nicolas/stdcout.git))

############ End of file ########################################
