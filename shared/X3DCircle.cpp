#include "stdafx.h"
#include "X3DCircle.h"

namespace X3D
{

Circle::Circle() {
}

Circle::Circle(const Vector3 &center, const float radius, const Vector3 &normal): center(center), radius(radius), normal(normal) {
}

Circle::Circle(const Circle &c): center(c.center), radius(c.radius), normal(c.normal) {
}

}