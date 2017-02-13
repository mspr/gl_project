#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

class Camera
{
	public:
		Camera();

		void translate(const QVector3D& dt);
		void setRotation(QQuaternion& dr);

		QMatrix4x4 toMatrix() const;

	private:
		QVector3D m_translation;
		QQuaternion m_rotation;
};

#endif // CAMERA_H
