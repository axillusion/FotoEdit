#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

typedef UInt32 Color;

/*------------------------------------------------------------------------------
 * Color_GetRGB
 *----------------------------------------------------------------------------*/
/** 
    Extracts red, green and blue values from a 24 bits color code of format 
    0xRRGGBB.

@param [in] color
    24 bits color code with format 0xRRGGBB
@param [out] red
	value of color red
@param [out] green
	value of color green
@param [out] blue
	value of color blue
 ******************************************************************************/

void Color_GetRGB ( 
	IN const Color color, 
	OUT UInt8* red, 
	OUT UInt8* green, 
	OUT UInt8* blue ); 

/*------------------------------------------------------------------------------
 * Color_GetYUV
 *----------------------------------------------------------------------------*/
/** 
    Extracts red, green and blue values from a 24 bits color code of format 
    0xRRGGBB and converts them to Y, U, V values.

@param [in] color
    24 bits color code with format 0xRRGGBB
@param [out] Y
	value of color Y
@param [out] U
	value of color U
@param [out] V
	value of color V
 ******************************************************************************/

void Color_GetYUV ( 
	IN const Color color, 
	OUT UInt8* Y, 
	OUT UInt8* U, 
	OUT UInt8* V );

/*------------------------------------------------------------------------------
 * Color_GetGray
 *----------------------------------------------------------------------------*/
/** 
    Extracts red, green and blue values from a 24 bits color code of format 
    0xRRGGBB and converts them to a grey value.

@param [in] color
    24 bits color code with format 0xRRGGBB
@param [out] gray 
	value of color gray
 ******************************************************************************/

void Color_GetGray ( 
	IN const Color color, 
	OUT UInt8* gray );

/*------------------------------------------------------------------------------
 * Color_RGBToYUV
 *----------------------------------------------------------------------------*/
/** 
    converts R,G,B values to Y,U,V values.

@param [in] R
	value of color red
@param [in] G
	value of color green
@param [in] B
	value of color blue
@param [out] Y
	value of color Y
@param [out] U
	value of color U
@param [out] V
	value of color V
 ******************************************************************************/

void Color_RGBToYUV ( 
    IN const UInt8 R,
    IN const UInt8 G,
    IN const UInt8 B,
    OUT UInt8* Y,
    OUT UInt8* U,
    OUT UInt8* V );

/*------------------------------------------------------------------------------
 * Color_YUVToRGB
 *----------------------------------------------------------------------------*/
/** 
    converts Y,U,V values to R,G,B values.

@param [in] Y
	value of color Y
@param [in] U
	value of color U
@param [in] V
	value of color V
@param [out] R
	value of color red
@param [out] G
	value of color green
@param [out] B
	value of color blue

 ******************************************************************************/

void Color_YUVToRGB ( 
    IN const UInt8 Y,
    IN const UInt8 U,
    IN const UInt8 V,
    OUT UInt8* R,
    OUT UInt8* G,
    OUT UInt8* B );

#endif // COLOR_H_INCLUDED
