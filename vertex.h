#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>

class Vertex
{
	public:
		Vertex(const QVector3D& position, const QVector3D& color);

		const QVector3D& position() const;
		void setPosition(const QVector3D& position);
		const QVector3D& color() const;
		void setColor(const QVector3D& color);

		static int positionOffset();
		static int colorOffset();

	public:
		static const int s_positionTupleSize = 3;
		static const int s_colorTupleSize = 3;

	protected:
		QVector3D m_position;
		QVector3D m_color;
};

#endif // VERTEX_H
