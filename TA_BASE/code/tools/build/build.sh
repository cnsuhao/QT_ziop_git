
#The source code in this file is the property of
#Ripple Systems and is not for redistribution
#in any form.
#
#Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/tools/build/build.sh $ 
#Author:  Grace Koh
#Version: $Revision: #1 $
#
#Last modification: $DateTime: 2015/10/09 15:42:35 $
#Last modified by:  $Author: CM $

#!/bin/bash

# Modify these as required.
export ROOT_BASE="/disk2/tabuild1/Nightly_Build/DDBase_Build/Base"

# Prepare the environment.
source /disk2/tabuild1/.profile

# Execute the base build.
cd $ROOT_BASE
ant clean_all p4checkoutCode version build stage
