#include "GLWidget.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <cmath>


GLWidget::GLWidget()
	: QGLWidget()
	, mSplitG( false )
	, mShowR( true )
	, mShowGR( true )
	, mShowGB( true )
	, mShowB( true )
	, mPlaneR( nullptr )
{
}


GLWidget::~GLWidget()
{
}


GLWidget::Plane::Plane()
{
	verticesCount = 4 * ( 100 + 1 ) * ( 75 + 1 );
	vertices = new Vertex[verticesCount];

	for ( uint32_t i = 0; i < verticesCount; i++ ) {
		Vertex* vertex = &vertices[i];
		vertex->x = 0.0f;
		vertex->y = 0.0f;
		vertex->z = 0.0f;
		vertex->color[0] = vertex->color[1] = vertex->color[2] = vertex->color[3] = 1.0f;
	}

	Vertex* vertex = vertices;
	for ( uint32_t y = 0; y < 76; y++ ) {
		for ( uint32_t x = 0; x < 101; x++ ) {
			vertex->x = (float)x * 51.0f / 100.0f - 52.0f / 2.0f;
			vertex->y = (float)y * 38.0f / 75.0f - 39.0f / 2.0f;
			vertex++;
			vertex->x = std::min( 51.0f, (float)( x + 1 ) * 51.0f / 100.0f ) - 52.0f / 2.0f;
			vertex->y = (float)y * 38.0f / 75.0f - 39.0f / 2.0f;
			vertex++;
			vertex->x = (float)x * 51.0f / 100.0f - 52.0f / 2.0f;
			vertex->y = (float)y * 38.0f / 75.0f - 39.0f / 2.0f;
			vertex++;
			vertex->x = (float)x * 51.0f / 100.0f - 52.0f / 2.0f;
			vertex->y = std::min( 38.0f, (float)( y + 1 ) * 38.0f / 75.0f ) - 39.0f / 2.0f;
			vertex++;
		}
	}
}


GLWidget::Plane::~Plane()
{
}


void GLWidget::initializeGL()
{
	mZoom = 0.0f;
	mRotH = -M_PI / 2.0f;
	mRotV = -M_PI / 2.0f;

	mPlaneR = new Plane();
	mPlaneGR = new Plane();
	mPlaneGB = new Plane();
	mPlaneB = new Plane();
}


float interpolate( uint16_t channel[52*39], float x, float y )
{
	int32_t X0 = (int32_t)std::max( 0.0f, std::min( 51.0f, std::floor( x ) ) );
	int32_t Y0 = 39 - 1 - (int32_t)std::max( 0.0f, std::min( 38.0f, std::floor( y ) ) );
	int32_t X1 = (int32_t)std::max( 0.0f, std::min( 51.0f, std::ceil( x ) ) );
	int32_t Y1 = 39 - 1 - (int32_t)std::max( 0.0f, std::min( 38.0f, std::floor( y ) ) );
	int32_t X2 = (int32_t)std::max( 0.0f, std::min( 51.0f, std::floor( x ) ) );
	int32_t Y2 = 39 - 1 - (int32_t)std::max( 0.0f, std::min( 38.0f, std::ceil( y ) ) );
	int32_t X3 = (int32_t)std::max( 0.0f, std::min( 51.0f, std::ceil( x ) ) );
	int32_t Y3 = 39 - 1 - (int32_t)std::max( 0.0f, std::min( 38.0f, std::ceil( y ) ) );

	float itmp;
	float fx = std::modf( x, &itmp );
	float fy = std::modf( y, &itmp );

	float f0 = ( 1.0f - fx ) * ( 1.0f - fy );
	float f1 = fx * ( 1.0f - fy );
	float f2 = ( 1.0f - fx ) * fy;
	float f3 = fx * fy;

	float value0 = channel[Y0 * 52 + X0];
	float value1 = channel[Y1 * 52 + X1];
	float value2 = channel[Y2 * 52 + X2];
	float value3 = channel[Y3 * 52 + X3];
	return value0 * f0 + value1 * f1 + value2 * f2 + value3 * f3;
}


void GLWidget::RepaintShader( uint16_t channels[4][2028], bool separate_gr_gb, bool enable_r, bool enable_gr, bool enable_gb, bool enable_b )
{
	mSplitG = separate_gr_gb;
	mShowR = enable_r;
	mShowGR = enable_gr;
	mShowGB = enable_gb;
	mShowB = enable_b;

	if ( not mPlaneR ) {
		return;
	}

	float max = 0.0f;
	float offset = 0.0f;

	for ( uint32_t i = 0; i < mPlaneR->verticesCount; i++ ) {
		Plane::Vertex* vertex = &mPlaneR->vertices[i];
		float gain = interpolate( channels[0], vertex->x + 52.0f / 2.0f, vertex->y + 39.0f / 2.0f );
		vertex->z = 32.0f * gain / 255.0f - 16.0f;
		vertex->color[1] = vertex->color[2] = ( vertex->z + 10.0f ) / 10.0f;
		vertex->z += 16.0f;
		max = std::max( vertex->z, max );
	}

	if ( separate_gr_gb ) {
		for ( uint32_t i = 0; i < mPlaneGR->verticesCount; i++ ) {
			Plane::Vertex* vertex = &mPlaneGR->vertices[i];
			float gain = interpolate( channels[1], vertex->x + 52.0f / 2.0f, vertex->y + 39.0f / 2.0f );
			vertex->z = 32.0f * gain / 255.0f - 16.0f;
			vertex->color[2] = ( vertex->z + 10.0f ) / 10.0f;
			vertex->color[0] = std::max( vertex->color[2], 0.75f );
			vertex->z += 16.0f;
			max = std::max( vertex->z, max );
		}
		for ( uint32_t i = 0; i < mPlaneGB->verticesCount; i++ ) {
			Plane::Vertex* vertex = &mPlaneGB->vertices[i];
			float gain = interpolate( channels[2], vertex->x + 52.0f / 2.0f, vertex->y + 39.0f / 2.0f );
			vertex->z = 32.0f * gain / 255.0f - 16.0f;
			vertex->color[0] = ( vertex->z + 10.0f ) / 10.0f;
			vertex->color[2] = std::max( vertex->color[0], 0.75f );
			vertex->z += 16.0f;
			max = std::max( vertex->z, max );
		}
	} else {
		for ( uint32_t i = 0; i < mPlaneGR->verticesCount; i++ ) {
			Plane::Vertex* vertex = &mPlaneGR->vertices[i];
			float gain1 = interpolate( channels[1], vertex->x + 52.0f / 2.0f, vertex->y + 39.0f / 2.0f );
			float gain2 = interpolate( channels[2], vertex->x + 52.0f / 2.0f, vertex->y + 39.0f / 2.0f );
			vertex->z = 32.0f * ( ( gain1 + gain2 ) / 2.0f ) / 255.0f - 16.0f;
			vertex->color[0] = vertex->color[2] = ( vertex->z + 10.0f ) / 10.0f;
			vertex->z += 16.0f;
			max = std::max( vertex->z, max );
		}
	}

	for ( uint32_t i = 0; i < mPlaneB->verticesCount; i++ ) {
		Plane::Vertex* vertex = &mPlaneB->vertices[i];
			float gain = interpolate( channels[3], vertex->x + 52.0f / 2.0f, vertex->y + 39.0f / 2.0f );
		vertex->z = 32.0f * gain / 255.0f - 16.0f;
		vertex->color[0] = vertex->color[1] = ( vertex->z + 10.0f ) / 10.0f;
		vertex->z += 16.0f;
		max = std::max( vertex->z, max );
	}

	offset = max / 2.0f;

	for ( uint32_t i = 0; i < mPlaneR->verticesCount; i++ ) {
		mPlaneR->vertices[i].z -= offset;
		mPlaneGR->vertices[i].z -= offset;
		mPlaneGB->vertices[i].z -= offset;
		mPlaneB->vertices[i].z -= offset;
	}
}


void GLWidget::paintGL()
{
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef( 0.0f, 0.0f, -50.0f );
	glTranslatef( 0.0, 0.0f, -mZoom );
	glRotatef( -mRotV, 1.0f, 0.0f, 0.0f );
	glRotatef( mRotH, 0.0f, 0.0f, 1.0f );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glBegin( GL_LINES );

	if ( mShowR )  {
		for ( uint32_t i = 0; i < mPlaneR->verticesCount; i++ ) {
			glColor4f( mPlaneR->vertices[i].color[0], mPlaneR->vertices[i].color[1], mPlaneR->vertices[i].color[2], mPlaneR->vertices[i].color[3] );
			glVertex3f( mPlaneR->vertices[i].x, mPlaneR->vertices[i].y, mPlaneR->vertices[i].z );
		}
	}
	if ( mShowGR )  {
		for ( uint32_t i = 0; i < mPlaneGR->verticesCount; i++ ) {
			glColor4f( mPlaneGR->vertices[i].color[0], mPlaneGR->vertices[i].color[1], mPlaneGR->vertices[i].color[2], mPlaneGR->vertices[i].color[3] );
			glVertex3f( mPlaneGR->vertices[i].x, mPlaneGR->vertices[i].y, mPlaneGR->vertices[i].z );
		}
	}
	if ( mSplitG and mShowGB )  {
		for ( uint32_t i = 0; i < mPlaneGB->verticesCount; i++ ) {
			glColor4f( mPlaneGB->vertices[i].color[0], mPlaneGB->vertices[i].color[1], mPlaneGB->vertices[i].color[2], mPlaneGB->vertices[i].color[3] );
			glVertex3f( mPlaneGB->vertices[i].x, mPlaneGB->vertices[i].y, mPlaneGB->vertices[i].z );
		}
	}
	if ( mShowB )  {
		for ( uint32_t i = 0; i < mPlaneB->verticesCount; i++ ) {
			glColor4f( mPlaneB->vertices[i].color[0], mPlaneB->vertices[i].color[1], mPlaneB->vertices[i].color[2], mPlaneB->vertices[i].color[3] );
			glVertex3f( mPlaneB->vertices[i].x, mPlaneB->vertices[i].y, mPlaneB->vertices[i].z );
		}
	}

	glEnd();
}


void GLWidget::resizeGL( int width, int height )
{
	glViewport( 0, 0, width, height );

	glMatrixMode( GL_PROJECTION );
	float h = 1.0f / std::tan( 45.0f * M_PI / 360.0f );
	float neg_depth = 0.1f - 1000.0f;
	float m[16];
	m[0] = h / ( (float)width / (float)height );
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	m[4] = 0.0f;
	m[5] = h;
	m[6] = 0.0f;
	m[7] = 0.0f;
	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = ( 1000.0f + 0.1f ) / neg_depth;
	m[11] = -1.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 2.0f * ( 0.1f * 1000.0f ) / neg_depth;
	m[15] = 0.0f;
	glLoadMatrixf( m );
}


void GLWidget::mousePressEvent( QMouseEvent* event )
{
	mCursorPos = event->localPos();
	if ( event->buttons() & Qt::LeftButton ) {
		setCursor( Qt::BlankCursor );
	}
}


void GLWidget::mouseReleaseEvent( QMouseEvent* event )
{
	setCursor( Qt::ArrowCursor );
}


void GLWidget::mouseMoveEvent( QMouseEvent* event )
{
	if ( event->buttons() & Qt::LeftButton ) {
		QPointF warp;
		warp.setX( ( event->localPos().x() - mCursorPos.x() ) / (float)width() );
		warp.setY( ( event->localPos().y() - mCursorPos.y() ) / (float)height() );
		mRotH -= warp.x() * -180.0f;
		mRotV += warp.y() * ( -180.0f * (float)height() / (float)width() );
		repaint();
		mCursorPos = event->localPos();
	}
}


void GLWidget::wheelEvent( QWheelEvent* event )
{
	QPoint numDegrees = event->angleDelta() / 8;
	mZoom -= (float)numDegrees.y() / 10.0f;
	repaint();
}
