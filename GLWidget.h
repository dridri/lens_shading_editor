#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

class GLWidget : public QGLWidget
{
public:
	GLWidget();
	virtual ~GLWidget();

	void RepaintShader( uint16_t channels[4][52 * 39], bool separate_gr_gb, bool enable_r, bool enable_gr, bool enable_gb, bool enable_b );

protected:
	void initializeGL();
	void paintGL();
	void resizeGL( int width, int height );
	void mousePressEvent( QMouseEvent* event );
	void mouseReleaseEvent( QMouseEvent* event );
	void mouseMoveEvent( QMouseEvent* event );
	void wheelEvent( QWheelEvent* event );

private:
	class Plane {
	public:
		Plane();
		~Plane();
		typedef struct {
			float color[4];
			float x, y, z;
		} Vertex;
		Vertex* vertices;
		uint32_t verticesCount;
	};

	bool mSplitG;
	bool mShowR;
	bool mShowGR;
	bool mShowGB;
	bool mShowB;

	Plane* mPlaneR;
	Plane* mPlaneGR;
	Plane* mPlaneGB;
	Plane* mPlaneB;

	float mZoom;
	float mRotH;
	float mRotV;
	QPointF mCursorPos;
};

#endif // GLWIDGET_H
