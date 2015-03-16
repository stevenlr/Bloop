#include "maths/Vector.h"

template class Vector<2>;
template class Vector<3>;
template class Vector<4>;

template <>
Vector<3> Vector<3>::cross(const Vector<3> &v) const
{
    Vector<3> v2;

    v2._data[0] = _data[1] * v._data[2] - _data[2] * v._data[1];
    v2._data[1] = _data[2] * v._data[0] - _data[0] * v._data[2];
    v2._data[2] = _data[0] * v._data[1] - _data[1] * v._data[0];

    return v2;
}
