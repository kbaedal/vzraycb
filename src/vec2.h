#ifndef __VEC2_H__
#define __VEC2_H__ 1

#include <math.h>
#include <iostream>

/**
 * Clase de vectores de 2 dimensiones.
 * 
 * Lo utilizaremos, sobre todo, en la aplicacion de texturas.
 */
class Vec2
{
	public:
		// Constructores
		Vec2(float a, float b)
		{
			this->set(a, b);			
		}
		Vec2() { e[0] = e[1] = 0.f; }
		Vec2(const Vec2 &v) { this->set(v); }
		
		// Operadores aritmeticos.
		const Vec2 &operator+() const { return *this; }
		Vec2 operator-() const { return Vec2(-e[0], -e[1]); }
		friend Vec2 operator+(const Vec2 &v1, const Vec2 &v2);
		friend Vec2 operator-(const Vec2 &v1, const Vec2 &v2);
		friend Vec2 operator*(const Vec2 &v, float f);
		friend Vec2 operator*(float f, const Vec2 &v);
		friend Vec2 operator/(const Vec2 &v, float f);
		friend Vec2 operator/(float f, const Vec2 &v);
		
		// Operadores logicos.
		friend bool operator==(const Vec2 &v1, const Vec2 &v2);
		friend bool operator!=(const Vec2 &v1, const Vec2 &v2);
				
		// Otras operaciones.
		float length() { return sqrt(e[0]*e[0] + e[1]*e[1]); }
		float sqLength() { return (e[0]*e[0] + e[1]*e[1]); }
		void normalize() { *this = *this * (1 / this->length()); }
		void set(float a, float b)
		{
			e[0] = a;
			e[1] = b;
		}
		void set(const Vec2 &v)
		{
			e[0] = v.x();
			e[1] = v.y();
		}
		
		// Acceso rapido a los componentes.
		float x() const { return e[0]; }
		float y() const { return e[1]; }
		
		/**
		 * Devuelve el versor asociado al parametro indicado.
		 * @param v Vector del que calcular su versor
		 * @return El versor correspondiente
		 */			
		friend Vec2 versor(const Vec2 &v);
		
		// Manejo del operador << para ostream
		friend std::ostream& operator<<(std::ostream &os, const Vec2 &v);
		
		// Componentes
		float e[2];
};

#endif // __VEC2_H__