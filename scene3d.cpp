#include "scene3d.h"

static GLfloat vertices[] = {
	0.5, 0.0, -1.0,
	0.0, 0.5, -1.0,
	-0.5, 0.0, -1.0
};

static GLfloat colors[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

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
			"in vec3 in_color;\n"
			"out vec4 color;\n"
			"void main()\n"
			"{\n"
			"    gl_Position = vec4(in_vertex, 1.0);\n"
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
	//m_modelviewId = m_shaderProgram->uniformLocation("matrix");

	m_buffer.create();
	m_buffer.bind();
	m_buffer.allocate(sizeof(vertices) + sizeof(colors));
	m_buffer.write(0, vertices, sizeof(vertices));
	m_buffer.write(sizeof(vertices), colors, sizeof(colors));
	m_buffer.release();

	QOpenGLVertexArrayObject::Binder vertexArrayObjectBinder(&m_vertexArrayObject);
	if (m_vertexArrayObject.isCreated())
			setupVertexAttributes();
}

void Scene3D::setupVertexAttributes()
{
		m_buffer.bind();
		m_shaderProgram->setAttributeBuffer(m_positionId, GL_FLOAT, 0, 3);
		m_shaderProgram->setAttributeBuffer(m_colorId, GL_FLOAT, sizeof(vertices), 3);
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

	if (!m_shaderProgram->bind())
		close();

	//QMatrix4x4 modelview;
	//matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	//modelview.translate(0.0f, 0.0f, -2.0f);
	//matrix.rotate(m_angle, 0.0f, 1.0f, 0.0f);

//	QMatrix4x4 projection;
//	projection.perspective(70.0, (double) width() / height(), 1.0, 100.0);

//	m_shaderProgram->setUniformValue(m_modelviewId, modelview);
//	m_shaderProgram->setUniformValue(m_projectionId, projection);

	m_vertexArrayObject.bind();
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//m_shaderProgram->disableAttributeArray(m_positionId);
	//m_shaderProgram->disableAttributeArray(m_colorId);
	m_vertexArrayObject.release();

	m_shaderProgram->release();
}
