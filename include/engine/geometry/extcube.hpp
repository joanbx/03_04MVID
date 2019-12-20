#ifndef __EXTCUBE_H__
#define __EXTCUBE_H__ 1

#include "geometry.hpp"

class ExtendedCube final : public Geometry {
public:
	ExtendedCube() = delete;
    explicit ExtendedCube(float size, float cX=0.0f, float cY=0.0f, float cZ=0.0f);

protected:
    float _size;
	float _cX=0.0f;
	float _cY=0.0f;
	float _cZ=0.0f;
};
#endif