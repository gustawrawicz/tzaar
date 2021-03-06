#ifndef _DIRECTIONS_H_
#define _DIRECTIONS_H_

#include "sizes.h"

#define DIR_NUMBER 6

enum direction {up_t=0, rightup_t, right_t, down_t, leftdown_t, left_t}; /*directions clockwise*/

const int dirOffsets[DIR_NUMBER] =										/*for one-dimension array*/
			{-(int)factualSideSize, -(int)factualSideSize+1, 1, factualSideSize, factualSideSize-1, -1};

const int dirCoordOffsets[DIR_NUMBER][2] =								/*for two dimensions*/
			{ {0,-1},{1,-1},{1,0},{0,1},{-1,1},{-1,0} };

#define ITERATE_DIRS(unique_name) for(unsigned unique_name=0; unique_name<DIR_NUMBER; unique_name++)

#define ITERATE_LINES(unique_name) for(unsigned unique_name=0; unique_name<DIR_NUMBER/2; unique_name++)

#define OPPOSITE_DIR(dir) ((dir+(DIR_NUMBER/2))%DIR_NUMBER)

#endif