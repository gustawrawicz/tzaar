#ifndef _FAST_COMPUTATIONS_H_
#define _FAST_COMPUTATIONS_H_

class FastComputations{

public:

	static float invSqrt(float x){
		union {
			float f;
			int i;
		} tmp;
		tmp.f = x;
		tmp.i = 0x5f3759df - (tmp.i >> 1);
		float y = tmp.f;
		return y * (1.5f - 0.5f * x * y * y);
	}

};

#endif