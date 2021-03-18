#pragma once

#include <QOpenGLWidget>
#include "ui_MyQtOpenGLWidgets.h"

#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class MyQtOpenGLWidgets : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	MyQtOpenGLWidgets(QWidget *parent = Q_NULLPTR);
	~MyQtOpenGLWidgets();

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

private:
	QOpenGLTexture *m_pTextures;
	QOpenGLShaderProgram *m_pProgram;
	QOpenGLBuffer m_vbo;
	int m_matrixUniform;
	QMatrix4x4 m_projectMat;

	bool createShaderProgram();
	void makeObject();

private:
	Ui::MyQtOpenGLWidgets ui;
};
