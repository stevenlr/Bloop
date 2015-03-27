#include "maths/TransformPipeline.h"

#include "maths/MatrixFactory.h"

using namespace std;

TransformPipeline::TransformPipeline()
{
	_model = new Matrix4();
}

TransformPipeline::~TransformPipeline()
{
	while (!_modelStack.empty()) {
		delete _modelStack.top();
		_modelStack.pop();
	}

	delete _model;
}

void TransformPipeline::orthographicProjection(float left, float right, float bottom, float top, float near, float far)
{
	_projection = MatrixFactory::orthographicProjection4x4(left, right, bottom, top, near, far);
	_isDirty = true;
}

void TransformPipeline::perspectiveProjection(float fov, float width, float height, float near, float far)
{
	_projection = MatrixFactory::perspectiveProjection4x4(fov, width, height, near, far);
	_isDirty = true;
}

void TransformPipeline::lookAt(const Vector3 &position, const Vector3 &target, const Vector3 &up)
{
	_view = MatrixFactory::lookAt4x4(position, target, up);
	_isDirty = true;
}

void TransformPipeline::lookAt(const Camera &camera)
{
	_view = camera.toLookAtMatrix();
	_isDirty = true;
}

void TransformPipeline::identity()
{
	_model->identity();
	_isDirty = true;
}

void TransformPipeline::translation(const Vector3 &offset)
{
	translation(offset[0], offset[1], offset[2]);
}

void TransformPipeline::translation(float x, float y, float z)
{
	*_model *= MatrixFactory::translation4x4(x, y, z);
	_isDirty = true;
}

void TransformPipeline::scale(const Vector3 &s)
{
	scale(s[0], s[1], s[2]);
}

void TransformPipeline::scale(float a)
{
	scale(a, a, a);
}

void TransformPipeline::scale(float x, float y, float z)
{
	*_model *= MatrixFactory::scale4x4(x, y, z);
	_isDirty = true;
}

void TransformPipeline::rotationX(float a)
{
	*_model *= MatrixFactory::rotationX4x4(a);
	_isDirty = true;
}

void TransformPipeline::rotationY(float a)
{
	*_model *= MatrixFactory::rotationY4x4(a);
	_isDirty = true;
}

void TransformPipeline::rotationZ(float a)
{
	*_model *= MatrixFactory::rotationZ4x4(a);
	_isDirty = true;
}

void TransformPipeline::rotation(const Vector3 &axis, float angle)
{
	rotation(axis[0], axis[1], axis[2], angle);
}

void TransformPipeline::rotation(float ux, float uy, float uz, float angle)
{
	*_model *= MatrixFactory::rotation4x4(ux, uy, uz, angle);
	_isDirty = true;
}

void TransformPipeline::rotation(const Quaternion &q)
{
	*_model *= MatrixFactory::rotation4x4(q);
	_isDirty = true;
}

void TransformPipeline::saveModel()
{
	Matrix4 *newModel = new Matrix4(*_model);

	_modelStack.push(_model);
	_model = newModel;
	_isDirty = true;
}

void TransformPipeline::restoreModel()
{
	if (_modelStack.empty())
		return;

	delete _model;
	_model = _modelStack.top();
	_modelStack.pop();
	_isDirty = true;
}

const Matrix4 &TransformPipeline::getProjectionMatrix() const
{
	return _projection;
}

const Matrix4 &TransformPipeline::getViewMatrix() const
{
	return _view;
}

const Matrix4 &TransformPipeline::getModelMatrix() const
{
	return *_model;
}

const Matrix4 &TransformPipeline::getPVMMatrix()
{
	if (_isDirty)
		computeCache();

	return _pvmCache;
}

const Matrix4 &TransformPipeline::getViewModelMatrix()
{
	if (_isDirty)
		computeCache();

	return _viewModelCache;
}

const Matrix3 &TransformPipeline::getNormalMatrix()
{
	if (_isDirty)
		computeCache();

	return _normalCache;
}

const Matrix4 &TransformPipeline::getInverseViewMatrix()
{
	if (_isDirty)
		computeCache();

	return _inverseViewCache;
}

void TransformPipeline::computeCache()
{
	_viewModelCache = _view * *_model;
	_pvmCache = _projection * _viewModelCache;
	
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			_normalCache(i, j) = _viewModelCache(i, j);
		}
	}

	_normalCache.invert();
	_normalCache.transpose();

	_inverseViewCache = _view;
	_inverseViewCache.invert();

	_isDirty = false;
}
