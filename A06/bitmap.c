/*----------------------------------------------
 * Author: Oghenefejiro Anigboro 
 * Date: 10/05/2024
 * Description: program that converts a 64bit unsigned int 
 * into an 8x8 1bpp sprite 
 ---------------------------------------------*/
#include <stdio.h>

int main() {
  unsigned long img;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);
  unsigned long leftmask = 0x1ul << 63;
  
  for(int i = 63; i >= 0; --i){
    if((leftmask & img) != 0){
      printf("@ ");
    } else printf("  ");
    if(i % 8 == 0) printf("\n");
    leftmask = 0x1ul << (i-1);
  }
  return 0;
}
