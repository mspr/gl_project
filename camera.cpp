#include "camera.h"

Camera::Camera()
{

}

void Camera::translate(const QVector3D& dt)
{
	m_translation += dt;
}

void Camera::setRotation(QQuaternion& dr)
{
	m_rotation = dr * m_rotation;
}

QMatrix4x4 Camera::toMatrix() const
{
	QMatrix4x4 world;
	world.setToIdentity();
	world.rotate(m_rotation.conjugated());
	world.translate(-m_translation);

	return world;
}
