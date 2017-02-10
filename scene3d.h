#ifndef SCENE3D_H
#define SCENE3D_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class Scene3D : public QOpenGLWidget, public QOpenGLFunctions
{
	public:
		Scene3D(QWidget* parent = 0);
		~Scene3D();

	protected:
			virtual void initializeGL() override;
			virtual void resizeGL(int w, int h) override;
			virtual void paintGL() override;

	private:
		void setupVertexAttributes();

	protected:
		QOpenGLShaderProgram* m_shaderProgram = nullptr;
		QOpenGLShader* m_vertexShader = nullptr;
		QOpenGLShader* m_fragmentShader = nullptr;
		int m_positionId;
		int m_colorId;
		int m_projectionId;
		int m_modelviewId;
		QOpenGLBuffer m_buffer;
		QOpenGLVertexArrayObject m_vertexArrayObject;
};

#endif // SCENE3D_H
