
#ifndef _TYPE_CONVERSION_H_
#define _TYPE_CONVERSION_H_


void privateU32ToFloat(unsigned int uiSrcVal,float* pfDstVal,unsigned int uiPricision);
void privateFloatTou32(float fSrcVal,unsigned int* puiDstVal,unsigned int uiPricision);
void privateU16ToFloat(unsigned short usSrcVal,float* pfDstVal,unsigned int uiPricision);
void privateFloatTou16(float fSrcVal,unsigned short* pusDstVal,unsigned int uiPricision);

#endif //_TYPE_CONVERSION_H_

