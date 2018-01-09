# The source code in this file is the property of
# Ripple Systems and is not for redistribution
# in any form.
#
# Source:  $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/utilities/Makefile $ 
# Author:  Ripple
# Version: $Revision: #1 $
#
# Last modification: $DateTime: 2015/10/09 15:42:35 $
# Last modified by:  $Author: CM $
#
PROJ_ROOT_DIR	= ../../../..
PROJ_CODE_DIR	= transactive/core/utilities
PROJ_NAME	= TA_Utilities
PROJ_TYPE	= lib
PROJ_SOURCE	= DebugUtil.cpp \
		  DebugUtilInit.cpp \
		  DebugSETranslator.cpp \
		  TA_String.cpp \
		  TAAssert.cpp \
		  ILogType.cpp GenericLogType.cpp \
		  PreformattedLogType.cpp Logger.cpp \
	          RunParams.cpp Timer.cpp FunctionEntry.cpp \
		  Hostname.cpp MatrixException.cpp \
		  AlarmClock.cpp \
		  DateFormat.cpp \
		  StdPubUtility.cpp \
		  DataConversion.cpp
		  
		  
PROJ_INC_DIR	= ./src $(PROJ_ROOT_DIR)/code/transactive
PROJ_OUT_DIR	= $(PROJ_ROOT_DIR)/sdk/$(OPERATING_SYSTEM)_$(STRING_FLAG)$(RELEASE_FLAG)

##############################################################
# The main include file that has all the wiz bang stuff for neato compiling
##############################################################
include $(PROJ_ROOT_DIR)/code/tools/build/CommonMakefile.mak


install:
	#Do Nothing
