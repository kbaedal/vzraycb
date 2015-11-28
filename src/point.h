#ifndef __POINT_H__
#define __POINT_H__ 1

#include <iostream>

class Vec3;
/**
 * Manejo de puntos de 3 coordenadas.
 */
class Point
{
	public:
		// Constructores
		Point(double a, double b, double c) { this->set(a, b, c); }
		Point() { e[0] = e[1] = e[2] = 0.0f; }
		Point(double a) { this->set(a); }
		Point(const Point &p) { this->set(p); }
		Point(const Vec3 &v);

		// Operadores aritmeticos unarios
		const Point &operator+() const { return *this; }
		Point operator-() const { return Point(-e[0], -e[1], -e[2]); }
		Point &operator+=(Point const &p);

		// Operadores aritmeticos binarios
		friend Point operator+(Point p1, const Point &p2);
		friend Point operator-(Point p1, const Point &p2);
		friend Point operator*(Point p, double f);
		friend Point operator*(double f, Point p);
		friend Point operator/(Point p, double f);

		// Operadores logicos
		friend bool operator==(const Point &p1, const Point &p2);
		friend bool operator!=(const Point &p1, const Point &p2);

		// Otras operaciones
		void set(double a, double b, double c)
		{
			e[0] = a;
			e[1] = b;
			e[2] = c;
		}
		void set(const Point &p)
		{
			e[0] = p.x();
			e[1] = p.y();
			e[2] = p.z();
		}
		void set(double a)
		{
			e[0] = e[1] = e[2] = a;
		}
		void set(double *a)
		{
		    e[0] = a[0];
		    e[1] = a[1];
		    e[2] = a[2];
		}
		void set(const Vec3 &v);

		// Acceso rapido a los componentes
		double x() const { return e[0]; }
		double y() const { return e[1]; }
		double z() const { return e[2]; }

		// Manejo del operador << para ostream
		friend std::ostream &operator<<(std::ostream &os, const Point &p);

		// Componentes
		double e[3];
};

#endif // __POINT_H__
