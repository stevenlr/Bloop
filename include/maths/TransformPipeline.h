#ifndef MATHS_TRANSFORMPIPELINE_H
#define MATHS_TRANSFORMPIPELINE_H

#include <stack>

#include "maths/Vector.h"
#include "maths/Matrix.h"
#include "maths/Quaternion.h"

#include "graphics/Camera.h"

class TransformPipeline {
public:
	TransformPipeline();
	~TransformPipeline();
	TransformPipeline(const TransformPipeline &tp) = delete;
	TransformPipeline &operator=(const TransformPipeline &tp) = delete;

	void orthographicProjection(float left, float right, float bottom, float top, float near, float far);
	void perspectiveProjection(float fov, float width, float height, float near, float far);
	void lookAt(const Vector3 &position, const Vector3 &target, const Vector3 &up);
	void lookAt(const Camera &camera);

	void identity();

	void translation(const Vector3 &offset);
	void translation(float x, float y, float z);

	void scale(const Vector3 &scale);
	void scale(float a);
	void scale(float x, float y, float z);

	void rotationX(float a);
	void rotationY(float a);
	void rotationZ(float a);

	void rotation(const Vector3 &axis, float angle);
	void rotation(float ux, float uy, float uz, float angle);
	void rotation(const Quaternion &q);

	void saveModel();
	void restoreModel();

	const Matrix4 &getProjectionMatrix() const;
	const Matrix4 &getViewMatrix() const;
	const Matrix4 &getModelMatrix() const;
	const Matrix4 &getPVMMatrix();
	const Matrix4 &getViewModelMatrix();
	const Matrix3 &getNormalMatrix();
	const Matrix4 &getInverseViewMatrix();

private:
	void computeCache();

	Matrix4 _projection;
	Matrix4 _view;
	Matrix4 *_model;
	std::stack<Matrix4 *> _modelStack;
	bool _isDirty = true;
	Matrix4 _pvmCache;
	Matrix4 _viewModelCache;
	Matrix3 _normalCache;
	Matrix4 _inverseViewCache;
};

#endif
