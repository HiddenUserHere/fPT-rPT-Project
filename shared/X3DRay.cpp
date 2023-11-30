#include "stdafx.h"
#include "X3DRay.h"

namespace X3D
{

Ray::Ray() {
}

Ray::Ray(const Vector3 &location, const Vector3 &direction): location(location), direction(direction) {
}

Ray::Ray(const Ray &c): location(c.location), direction(c.direction) {
}

}