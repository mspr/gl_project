#include "vertex.h"

Vertex::Vertex(const QVector3D& position, const QVector3D& color)
	: m_position(position)
	, m_color(color)
{
}

const QVector3D& Vertex::position() const
{
	return m_position;
}

void Vertex::setPosition(const QVector3D& position)
{
	m_position = position;
}

const QVector3D& Vertex::color() const
{
	return m_color;
}

void Vertex::setColor(const QVector3D& color)
{
	m_color = color;
}

int Vertex::positionOffset()
{
	return offsetof(Vertex, m_position);
}

int Vertex::colorOffset()
{
	return offsetof(Vertex, m_color);
}
