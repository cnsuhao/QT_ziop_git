/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/utilities/src/NullOstream.h $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * <description>
  *
  */
#ifndef NULL_OSTREAM_H
#define NULL_OSTREAM_H

#include <iostream>
//#include <streambuf>
using std::streambuf;

using std::ostream;

namespace TA_Base_Core
{
	class NullStreamBuf: public streambuf
	{
	public:
		NullStreamBuf(): streambuf(){}
		virtual ~NullStreamBuf(){}

		void	put_buffer(void){};
		void	put_char(int){};
	};


	class NullOstream: public ostream
	{
	public:
		NullOstream(): ostream(new NullStreamBuf()){}
		virtual ~NullOstream(){}
	};
};

#endif // NULL_OSTREAM_H
