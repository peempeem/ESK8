#include "RGBLights.h"

static const uint16_t bitmap[] PROGMEM = { 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xc419, 0xc419, 0xc018, 0xb816, 0xb013, 0xa811, 0xa011, 0x9415, 0x8c19, 0x841d, 0x807b, 0x8173, 0x826b, 0x8363, 0x8fc0, 0xafc0, 0xcfc0, 0xefa0, 0xff60, 0xff01, 0xfe82, 0xfe24, 0xfda4, 0xfd22, 0xfca1, 0xfc20, 0xfc00, 0xfc00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
};

void RGBLights::draw(TFT_eSprite* sprite) { _draw(bitmap, sprite); }