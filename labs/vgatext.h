#include "util/io.h"

using namespace mmio;
namespace vgatext{

   static inline void writechar(int loc, uint8_t c, uint8_t bg, uint8_t fg, addr_t base){
     //your code goes here
     //loc<<1: as we have color also; bgfg c as little endian
     write16(base,loc<<1,((uint16_t)bg<<12)|((uint16_t)fg<<8)|(uint16_t)c);
   }

}//namespace vgatext
