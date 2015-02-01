#ifndef MATHS_MULTIPLYCONST_H
#define MATHS_MULTIPLYCONST_H

template <typename T>
class MultiplyConst {
public:
	MultiplyConst(T f) : _f(f) {}
	T operator()(T f) { return f * _f; };

private:
	T _f;
};

#endif
