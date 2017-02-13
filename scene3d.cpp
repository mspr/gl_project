#include "scene3d.h"
#include "vertex.h"

static QVector<Vertex> vertices = { Vertex(QVector3D(0.5, 0.0, -1.0), QVector3D(1.0, 0.0, 0.0)),
																		Vertex(QVector3D(0.0, 0.5, -1.0), QVector3D(0.0, 1.0, 0.0)),
																		Vertex(QVector3D(-0.5, 0.0, -1.0), QVector3D(0.0, 0.0, 1.0)) };

Scene3D::Scene3D(QWidget* parent)
	: QOpenGLWidget(parent)
{
	setMinimumSize(300, 250);
}

Scene3D::~Scene3D()
{
	makeCurrent();
	delete m_shaderProgram;
	delete m_vertexShader;
	delete m_fragmentShader;
	m_buffer.destroy();
	doneCurrent();
}

void Scene3D::initializeGL()
{
	initializeOpenGLFunctions();

	m_vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
	const char *vsrc =
			"#version 150\n"
			"in vec3 in_vertex;\n"
			"uniform mat4 modelview;\n"
			"uniform mat4 projection;\n"
			"in vec3 in_color;\n"
			"out vec4 color;\n"
			"void main()\n"
			"{\n"
			"    gl_Position = projection * modelview * vec4(in_vertex, 1.0);\n"
			"    color = vec4(in_color, 1.0);\n"
			"}\n";
	if (!m_vertexShader->compileSourceCode(vsrc))
	{
		qDebug() << m_vertexShader->log();
		close();
	}

	m_fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
	const char *fsrc =
			"#version 150\n"
			"in vec4 color;\n"
			"out vec4 out_color;\n"
			"void main(void)\n"
			"{\n"
			"    out_color = color;\n"
			"}\n";
	if (!m_fragmentShader->compileSourceCode(fsrc))
	{
		qDebug() << m_fragmentShader->log();
		close();
	}

	m_shaderProgram = new QOpenGLShaderProgram;

	if (!m_shaderProgram->addShader(m_vertexShader))
		close();
	if(!m_shaderProgram->addShader(m_fragmentShader))
		close();

	if (!m_shaderProgram->link())
	{
		qDebug() << m_shaderProgram->log();
		close();
	}

	m_positionId = m_shaderProgram->attributeLocation("in_vertex");
	m_colorId = m_shaderProgram->attributeLocation("in_color");
	m_modelviewId = m_shaderProgram->uniformLocation("modelview");
	m_projectionId = m_shaderProgram->uniformLocation("projection");

	m_buffer.create();
	m_buffer.bind();
	m_buffer.allocate(vertices.constData(), vertices.size()*sizeof(Vertex));
	m_buffer.release();

	QOpenGLVertexArrayObject::Binder vertexArrayObjectBinder(&m_vertexArrayObject);
	if (m_vertexArrayObject.isCreated())
			setupVertexAttributes();
}

void Scene3D::setupVertexAttributes()
{
		m_buffer.bind();
		m_shaderProgram->setAttributeBuffer(m_positionId, GL_FLOAT, Vertex::positionOffset(), Vertex::s_positionTupleSize, sizeof(Vertex));
		m_shaderProgram->setAttributeBuffer(m_colorId, GL_FLOAT, Vertex::colorOffset(), Vertex::s_colorTupleSize, sizeof(Vertex));
		m_shaderProgram->enableAttributeArray(m_positionId);
		m_shaderProgram->enableAttributeArray(m_colorId);
		m_buffer.release();
}

void Scene3D::resizeGL(int w, int h)
{
	Q_UNUSED(w);
	Q_UNUSED(h);
}

void Scene3D::paintGL()
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!m_vertexArrayObject.isCreated())
		close();

	m_shaderProgram->bind();

	QMatrix4x4 modelview;
	modelview.translate(0.2f, 0.0f, 0.0f);
	modelview.rotate(45.0, 0.0f, 0.0f, 1.0f);

	QMatrix4x4 projection;
//	projection.perspective(70.0, (double) width() / height(), 1.0, 100.0);

	m_shaderProgram->setUniformValue(m_modelviewId, modelview);
	m_shaderProgram->setUniformValue(m_projectionId, projection);

	m_vertexArrayObject.bind();
	glDrawArrays(GL_TRIANGLES, 0, 3);
	m_vertexArrayObject.release();

	m_shaderProgram->release();
}
