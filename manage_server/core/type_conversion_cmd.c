/* type_conversion_cmd.c -  the function for data type conversion  */

/*
 * Copyright(c) 2009 OPHYLINK COMMUNICATION TECHNOLOGY LTD
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable OPHYLINK license agreement.
 */

 
/*
modification history
--------------------
2014/7/14	 Created.  
*/

#include "type_conversion_cmd.h"

void privateU32ToFloat(unsigned int uiSrcVal,float* pfDstVal,unsigned int uiPricision)
{
	float fTmpVal = 0.0;
	unsigned int uiTmpVal = 0;
	
	if((uiSrcVal&0x80000000)!=0)
	{
		uiTmpVal = 0xffffffff - uiSrcVal;
		uiTmpVal += 1;
		fTmpVal = (float)uiTmpVal;
		fTmpVal = 0 - fTmpVal;
		fTmpVal = fTmpVal / uiPricision;
	}
	else
	{
		fTmpVal = (float)uiSrcVal;
		fTmpVal = fTmpVal / uiPricision;
	}
	*pfDstVal = fTmpVal;
}

void privateFloatTou32(float fSrcVal,unsigned int* puiDstVal,unsigned int uiPricision)
{
	float fTmpVal = 0.0;
	unsigned int uiTmpVal = 0;
	
	if(fSrcVal<0)
	{
		fTmpVal = fSrcVal*uiPricision; 
		fTmpVal = 0 - fTmpVal;
		uiTmpVal = (unsigned int)fTmpVal;
		uiTmpVal = 0x80000000 - uiTmpVal;
		uiTmpVal |= 0x80000000;
	}
	else
	{
		fTmpVal = fSrcVal*uiPricision; 
		uiTmpVal = (unsigned int)fTmpVal;
	}
	*puiDstVal = uiTmpVal;
}

void privateU16ToFloat(unsigned short usSrcVal,float* pfDstVal,unsigned int uiPricision)
{
	float fTmpVal = 0.0;
	unsigned short usTmpVal = 0;
	
	if((usSrcVal&0x8000)!=0)
	{
		usTmpVal = 0xffff - usSrcVal;
		usTmpVal += 1;
		fTmpVal = (float)usTmpVal;
		fTmpVal = 0 - fTmpVal;
		fTmpVal = fTmpVal / uiPricision;
	}
	else
	{
		fTmpVal = (float)usSrcVal;
		fTmpVal = fTmpVal / uiPricision;
	}
	*pfDstVal = fTmpVal;
}

void privateFloatTou16(float fSrcVal,unsigned short* pusDstVal,unsigned int uiPricision)
{
	float fTmpVal = 0.0;
	unsigned short usTmpVal = 0;
	
	if(fSrcVal<0)
	{
		fTmpVal = fSrcVal*uiPricision; 
		fTmpVal = 0 - fTmpVal;
		usTmpVal = (unsigned int)fTmpVal;
		usTmpVal = 0x8000 - usTmpVal;
		usTmpVal |= 0x8000;
	}
	else
	{
		fTmpVal = fSrcVal*uiPricision; 
		usTmpVal = (unsigned short)fTmpVal;
	}
	*pusDstVal = usTmpVal;
}

