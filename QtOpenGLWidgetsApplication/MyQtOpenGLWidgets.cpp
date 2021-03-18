#include "MyQtOpenGLWidgets.h"

MyQtOpenGLWidgets::MyQtOpenGLWidgets(QWidget *parent)
	: QOpenGLWidget(parent),
	m_vbo(QOpenGLBuffer::VertexBuffer),
	m_matrixUniform(0),
	m_projectMat()
{
	ui.setupUi(this);
}

MyQtOpenGLWidgets::~MyQtOpenGLWidgets()
{
	m_pTextures->release();
	delete m_pTextures;
	delete m_pProgram;

	makeCurrent();
	m_vbo.destroy();
}

void MyQtOpenGLWidgets::initializeGL()
{
	initializeOpenGLFunctions();

	createShaderProgram();

	makeObject();
}

void MyQtOpenGLWidgets::resizeGL(int w, int h)
{
	float aspect = float(w) / float(h ? h : 1);
	float fov = 45.0f, zNear = 0.1f, zFar = 100.0f;
	m_projectMat.setToIdentity();
	m_projectMat.perspective(fov, aspect, zNear, zFar);
}

void MyQtOpenGLWidgets::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);

	m_pProgram->bind();
	QMatrix4x4 mvMat;
	mvMat.translate(0.0f, 0.0f, -5.0f);
	m_pProgram->setUniformValue(m_matrixUniform, m_projectMat * mvMat);

	m_pTextures->bind();
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	m_pTextures->release();

	m_pProgram->release();
}

bool MyQtOpenGLWidgets::createShaderProgram()
{
	bool suc(false);
	m_pProgram = new QOpenGLShaderProgram(this);
	suc = m_pProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "vshader.glsl");
	suc = m_pProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "fshader.glsl");
	suc = m_pProgram->link();
	suc = m_pProgram->bind();

	if (!suc)
	{
		close();
		return suc;
	}

	m_matrixUniform = m_pProgram->uniformLocation("mvpMat");

	return suc;
}

void MyQtOpenGLWidgets::makeObject()
{
	m_vbo.create();

	m_pTextures = new QOpenGLTexture(QImage("01.png").mirrored());
	m_pTextures->setMinificationFilter(QOpenGLTexture::Nearest);
	m_pTextures->setMagnificationFilter(QOpenGLTexture::Linear);
	m_pTextures->setWrapMode(QOpenGLTexture::Repeat);

	//float arrVertex[]{
	//	// position           texture
	//	0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
	//	0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	//	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
	//	-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	//};//1
	//float arrVertex[]{
	//	// position           texture
	//	1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	//	1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	//	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	//	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f
	//};//2
	float arrVertex[]{
		// position           texture
		1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, 0.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, 0.0f, -1.0f, 1.0f
	};//3
	//float arrVertex[]{
	//	// position           texture
	//	1.0f,  1.0f, 0.0f, 2.0f, 2.0f,
	//	1.0f, -1.0f, 0.0f, 2.0f, 0.0f,
	//	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	//	-1.0f,  1.0f, 0.0f, 0.0f, 2.0f
	//};//4 equal 3
	//float arrVertex[]{
	//	// position           texture
	//	1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	//	1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
	//	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	//	-1.0f,  1.0f, 0.0f, 1.0f, 0.0f
	//};//5 rotate & mirror

	m_vbo.bind();
	m_vbo.allocate(arrVertex, sizeof(arrVertex));

	int attr = -1;
	attr = m_pProgram->attributeLocation("attrPos");
	m_pProgram->setAttributeBuffer(attr, GL_FLOAT, 0,
		3, sizeof(float) * 5);
	m_pProgram->enableAttributeArray(attr);

	attr = m_pProgram->attributeLocation("attrTexCoord");
	m_pProgram->setAttributeBuffer(attr, GL_FLOAT, 3 * sizeof(float),
		2, sizeof(float) * 5);
	m_pProgram->enableAttributeArray(attr);

	m_vbo.release();
}