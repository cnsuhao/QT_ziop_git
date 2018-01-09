# The source code in this file is the property of
# Ripple Systems and is not for redistribution
# in any form.
#
# Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/pa_4669/pa_agent/Makefile $
# @author:  Ripple
# @version: $Revision: #1 $
#
# Last modification: $DateTime: 2015/10/09 15:42:35 $
# Last modified by:  $Author: CM $
#

PROJ_ROOT_DIR	= ../../../../..
PROJ_CODE_DIR	= transactive/bus/pa_4669/pa_agent
PROJ_NAME	= TA_PaAgentLibrary
PROJ_SOURCE	= TrainPaAgentLibrary.cpp \
		  PaAgentEntityNotFoundException.cpp \
		  TrainPaAgentLibrary.cpp
PROJ_TYPE	= lib
PROJ_INC_DIR	= ./src $(PROJ_ROOT_DIR)/code/transactive
PROJ_OUT_DIR	= $(PROJ_ROOT_DIR)/sdk/$(OPERATING_SYSTEM)_$(STRING_FLAG)$(RELEASE_FLAG)

DEPENDENT_LIBRARIES = $(PROJ_ROOT_DIR)/code/transactive/core/message/IDL \
		      $(PROJ_ROOT_DIR)/code/transactive/bus/pa_4669/pa_agent/IDL

include $(PROJ_ROOT_DIR)/code/tools/build/CommonMakefile.mak

install:
	#Do Nothing
