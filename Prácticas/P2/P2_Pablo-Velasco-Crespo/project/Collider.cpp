#include "Collider.h"
#include <utility>
#include "math.h"
#include <stdio.h>

bool Collider::checkCircleCircle(const Vec2& pos1, float radius1, const Vec2& pos2, float radius2) const
{
	float radiusSum = radius1 + radius2;
	float dist = pos1.distance(pos2);
	return (dist < radiusSum);
}

bool Collider::checkCircleRect(const Vec2& circlePos, float circleRadius, const Vec2& rectPos, const Vec2& rectSize) const
{
	Vec2 halfRectSize(rectSize/2.f);
	Vec2 circleDistance(fabs(circlePos.getX() - rectPos.getX()), fabs(circlePos.getY() - rectPos.getY()));

	if (circleDistance.getX() > (halfRectSize.getX() + circleRadius))
	{
		return false;
	}
	if (circleDistance.getY() > (halfRectSize.getY() + circleRadius))
	{
		return false;
	}

	if (circleDistance.getX() <= halfRectSize.getX())
	{
		return true;
	}

	if (circleDistance.getY() <= halfRectSize.getY())
	{
		return true;
	}

	return (circleDistance.distance(halfRectSize) < circleRadius);
}

bool Collider::checkRectRect(const Vec2& rectPos1, const Vec2& rectSize1, const Vec2& rectPos2, const Vec2& rectSize2) const
{
	Vec2 halfRectSize1(rectSize1 / 2.f);
	Vec2 halfRectSize2(rectSize2 / 2.f);

	float xDist = fabs(rectPos1.getX() - rectPos2.getX());
	if (xDist < (halfRectSize1.getX() + halfRectSize2.getX()))
	{
		float yDist = fabs(rectPos1.getY() - rectPos2.getY());

		return yDist < (halfRectSize1.getY() + halfRectSize2.getY());
	}
	return false;
}

bool Collider::checkCirclePixels(const Vec2& circlePos, float circleRadius, const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const
{
	// Verificar si el cículo y el sprite se solapan
	if (!checkCircleRect(circlePos, circleRadius, pixelsPos, pixelsSize))
	{
		return false;
	}

	// En caso de que se solapen obtenemos las esquinas de la textura del píxel y los límites laterales y verticales del círculo
	int x1_min = (int)(pixelsPos.getX() - pixelsSize.getX() / 2);
	int x1_max = (int)(pixelsPos.getX() + pixelsSize.getX() / 2);
	int y1_min = (int)(pixelsPos.getY() - pixelsSize.getY() / 2);
	int y1_max = (int)(pixelsPos.getY() + pixelsSize.getY() / 2);

	int x2_min = (int)(circlePos.getX() - circleRadius);
	int x2_max = (int)(circlePos.getX() + circleRadius);
	int y2_min = (int)(circlePos.getY() - circleRadius);
	int y2_max = (int)(circlePos.getY() + circleRadius);

	// Ahora obtenemos las esquinas de la parte común
	int x_inter_min = std::max(x1_min, x2_min);
	int x_inter_max = std::min(x1_max, x2_max);
	int y_inter_min = std::max(y1_min, y2_min);
	int y_inter_max = std::min(y1_max, y2_max);

	// Recorremos los píxeles que hay entre las esquinas de la parte común hasta encontrar que uno no sea transparente, es decir que el canal alpha sea distinto de 0
	for (int y = y_inter_min; y < y_inter_max; ++y)
	{
		for (int x = x_inter_min; x < x_inter_max; ++x)
		{
			// Obtenemos las coordenadas locales en cada píxel
			int localX = x - x1_min;
			int localY = y - y1_min;

			// Comprobamos que el punto que estamos analizando entra dentro de los límites del círculo
			Vec2 pointPosition(pixelsPos + Vec2(localX, localY));
			if (pointPosition.distance(circlePos) < (circleRadius * circleRadius))
			{
				continue;
			}

			// Calculamos los índices, uint_8* es un array en 1D, para operar en 2D debemos multiplicar la Y por la anchura y sumarle la X, 
			// después multiplicamos por cuatro ya que los valores de los píxeles se organizan como: RGBA, donde sólo nos interesa el canal alpha
			int index = (localY * (int)pixelsSize.getX() + localX) * 4;

			// Por último comprobamos la cuarta posición a partir del índice calculado, que es el canal alpha, si es mayor que cero, 
			// significa que no es transparente, es decir que hay colisión
			if (pixels[index + 3] != 0.f)
			{
				return true;
			}
		}
	}
	return false;
}


bool Collider::checkPixelsPixels(const Vec2& pixelsPos1, const Vec2& pixelsSize1, const uint8_t* pixels1, const Vec2& pixelsPos2, const Vec2& pixelsSize2, const uint8_t* pixels2) const
{
	// Verificar si los rectángulos que contienen los píxeles se solapan.
	if (!checkRectRect(pixelsPos1, pixelsSize1, pixelsPos2, pixelsSize2))
	{
		return false;
	}

	// En caso de que se solapen obtenemos las esquinas
	int x1_min = (int)(pixelsPos1.getX() - pixelsSize1.getX() / 2);
	int x1_max = (int)(pixelsPos1.getX() + pixelsSize1.getX() / 2);
	int y1_min = (int)(pixelsPos1.getY() - pixelsSize1.getY() / 2);
	int y1_max = (int)(pixelsPos1.getY() + pixelsSize1.getY() / 2);

	int x2_min = (int)(pixelsPos2.getX() - pixelsSize2.getX() / 2);
	int x2_max = (int)(pixelsPos2.getX() + pixelsSize2.getX() / 2);
	int y2_min = (int)(pixelsPos2.getY() - pixelsSize2.getY() / 2);
	int y2_max = (int)(pixelsPos2.getY() + pixelsSize2.getY() / 2);

	// Ahora obtenemos las esquinas de la parte común
	int x_inter_min = std::max(x1_min, x2_min);
	int x_inter_max = std::min(x1_max, x2_max);
	int y_inter_min = std::max(y1_min, y2_min);
	int y_inter_max = std::min(y1_max, y2_max);

	// Recorremos los píxeles que hay entre las esquinas de la parte común hasta encontrar que uno no sea transparente, es decir que el canal alpha sea distinto de 0
	for (int y = y_inter_min; y < y_inter_max; ++y)
	{
		for (int x = x_inter_min; x < x_inter_max; ++x)
		{
			// Obtenemos las coordenadas locales en cada píxel
			int localX1 = x - x1_min;
			int localX2 = x - x2_min;

			int localY1 = y - y1_min;
			int localY2 = y - y2_min;

			// Calculamos los índices, uint_8* es un array en 1D, para operar en 2D debemos multiplicar la Y por la anchura y sumarle la X, 
			// después multiplicamos por cuatro ya que los valores de los píxeles se organizan como: RGBA, donde sólo nos interesa el canal alpha
			int index1 = (localY1 * (int)pixelsSize1.getX() + localX1) * 4;
			int index2 = (localY2 * (int)pixelsSize2.getX() + localX2) * 4;

			// Por último comprobamos la cuarta posición a partir del índice calculado, que es el canal alpha, si es mayor que cero, 
			// significa que no es transparente si ambas son mayores que cero hay colisión
			if (pixels1[index1 + 3] != 0.f && pixels2[index2 + 3] != 0.f)
			{
				return true;
			}
		}
	}
	return false;
}

bool Collider::checkPixelsRect(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels, const Vec2& rectPos, const Vec2& rectSize) const
{

	// Verificar si los rectángulos se solapan.
	if (!checkRectRect(pixelsPos, pixelsSize, rectPos, rectSize))
	{
		return false;
	}

	// En caso de que se solapen obtenemos las esquinas
	int x1_min = (int)(pixelsPos.getX() - pixelsSize.getX() / 2);
	int x1_max = (int)(pixelsPos.getX() + pixelsSize.getX() / 2);
	int y1_min = (int)(pixelsPos.getY() - pixelsSize.getY() / 2);
	int y1_max = (int)(pixelsPos.getY() + pixelsSize.getY() / 2);

	int x2_min = (int)(rectPos.getX() - rectSize.getX() / 2);
	int x2_max = (int)(rectPos.getX() + rectSize.getX() / 2);
	int y2_min = (int)(rectPos.getY() - rectSize.getY() / 2);
	int y2_max = (int)(rectPos.getY() + rectSize.getY() / 2);

	// Ahora obtenemos las esquinas de la parte común
	int x_inter_min = std::max(x1_min, x2_min);
	int x_inter_max = std::min(x1_max, x2_max);
	int y_inter_min = std::max(y1_min, y2_min);
	int y_inter_max = std::min(y1_max, y2_max);

	// Recorremos los píxeles que hay entre las esquinas de la parte común hasta encontrar que uno no sea transparente, es decir que el canal alpha sea distinto de 0
	for (int y = y_inter_min; y < y_inter_max; ++y)
	{
		for (int x = x_inter_min; x < x_inter_max; ++x)
		{
			// Obtenemos las coordenadas locales en cada píxel
			int localX = x - x1_min;
			int localY = y - y1_min;

			// Calculamos los índices, uint_8* es un array en 1D, para operar en 2D debemos multiplicar la Y por la anchura y sumarle la X, 
			// después multiplicamos por cuatro ya que los valores de los píxeles se organizan como: RGBA, donde sólo nos interesa el canal alpha
			int index = (localY * (int)pixelsSize.getX() + localX) * 4;

			// Por último comprobamos la cuarta posición a partir del índice calculado, que es el canal alpha, si es mayor que cero, 
			// significa que no es transparente, es decir que hay colisión
			if (pixels[index + 3] != 0.f)
			{
				return true;
			}
		}
	}
	return false;
}
