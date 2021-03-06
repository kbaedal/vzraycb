#include <cmath>

#include "isecaux.h"
#include "cylinder.h"

#include "onb.h"
#include "randomc/randomc.h"

const double Cylinder::kpi = 3.14159265358979323846;

Cylinder::Cylinder(Point a_bottom, Point a_top, double a_radius, Material *a_material)
{
		std::clog << "Creando cilindro..." << std::endl;
		// Nuestro cilindro estará definido por dos puntos y un radio,
		// siendo los dos puntos el centro de cada una de sus caras.
		// En su espacio, estos puntos serán:
		// p0 = <0, 0, -1>
		// p1 = <0, 0,  1>
		// Y su radio será siempre 1.
		// Aplicaremos las transformaciones necesarias para obtener el
		// cilindro indicado en el espacio real.

		// Creamos la matriz de transformación
		trans = new Transform;

		// Orden de operaciones para la transformación desde el cilindro
		// base al cilindro indicado:
		// 	- Trasladar nuestro cilindro 1 unidad en direccion z, así lo
		//		tendremos con su base en el origen del mundo real.
		//	- Escalarlo en x e y el valor indicado por a_dRadius, y en z
		//		primero lo escalamos a la mitad (tenemos un cilindro de
		//		dos unidades de longitud en z, lo necesitamos de 1 unidad)
		//		y después el valor resultante de la diferencia del punto
		//		incial con el punto final (a_v3P1 - a_v3P0).
		//	- Como ya tenemos la base en el origen, solo nos queda girar
		//		el cilindro el ángulo correcto para que al trasladarlo
		//		hasta el punto inicial indicado, el punto final quede
		//		en la posicion correcta. Para ello:
		//			1. Calcularemos el vector que une los extremos del
		//				cilindro (a_v3P1 - a_v3P0) y lo normalizamos.
		//			2. Calculamos el eje de giro, producto vectorial del
		//				anterior vector con el vector z.
		//			3. Ahora solo queda calcular el angulo de giro. Para
		//				ello utilizaremos el producto escalar, que nos
		//				dará el coseno del ángulo formado por los dos
		//				vectores.
		//	- Ahora solo queda trasladar el cilindro hasta a_v3P0.

		/*
		 *
		 * PRIMERO LOS ESCALES Y ROTATES Y POR ULTIMO EL TRANSLATE
		 *
		 */
		//~ // Trasladamos nuestro cilindro que tiene por puntos base el
		//~ // (-1, -1, -1) y (1, 1, 1) para que el (-1, -1, -1) coincida
		//~ // con el origen.
		//~ m_pTrans->translate(0.f, 0.f, 1.f);
		//~ // Lo escalamos a la mitad en z, con lo que tenemos un cilindro
		//~ // de una unidad de largo en ese eje.
		//~ m_pTrans->scale(1.f, 1.f, .5); --- ERROR --
		//  AQUI EL CILINDRO AL SER REDUCIDO PASA A TENER LA BASE EN Z=.5
		//  TODOS LOS CALCULOS A PARTIR DE ESTE PUNTO SON ERRONEOS.
		//~
		//~ // Ahora lo escalamos para que tenga el tamaño indicado, que
		//~ // será la distancia entre los puntos que lo definen, y el radio
		//~ // adecuado.
		//~ Vec3 v3Dist;
		//~ v3Dist.set(a_p3P1 - a_p3P0);
		//~ m_pTrans->scale(a_dRadius, a_dRadius, v3Dist.length());
		//~
		//~ std::clog << "Matrix: " << std::endl << m_pTrans->m_mtxM << std::endl;
		//~
		//~ std::clog << "Aplicado scale: <" << a_dRadius << ", " << a_dRadius << ", " << v3Dist.length() << ">" << std::endl;
		//~
		//~ Vec3 v3CilAxis, v3RotateAxis;
		//~
		//~ v3CilAxis.set(a_p3P1 - a_p3P0);
		//~ v3CilAxis.normalize();
		//~
		//~ std::clog << "Eje del cilindro: " << v3CilAxis << std::endl;
		//~
		//~ v3RotateAxis = cross(v3CilAxis, Vec3(0.f, 0.f, 1.f));
		//~ v3RotateAxis.normalize();
		//~
		//~ std::clog << "Eje de giro: " << v3RotateAxis << std::endl;
		//~
		//~ // Ambos son vectores normalizados, con lo que nos ahorramos
		//~ // la división por la multiplicacion de los modulos.
		//~ double dCosAng = dot(v3CilAxis, Vec3(0.f, 0.f, 1.f));
		//~ // El angulo esta en radianes, y lo necesitamos en grados sexagesimales.
		//~ double dAngRad 	= acos(dCosAng);
		//~ double dAng 		= dAngRad * (180.f / PI);
		//~
		//~ std::clog << "(Coseno, Angulo_Rad, Ang): (" << dCosAng << ", " << dAngRad << ", " << dAng << ")" << std::endl;
		//~ // Rotamos.
		//~ m_pTrans->rotate(dAng, v3RotateAxis);
		//~ std::clog << "Matrix: " << std::endl << m_pTrans->m_mtxM << std::endl;

		// Escalamos el cilindro en x e y al radio indicado, y en z
		// lo escalaremos a la distancia qua hay entre p0 y p1.
		Vec3 	cyl_axis,		// Eje del cilindro, y auxiliar para distancias
				rotate_axis;	// Eje de giro.
		Ray		aux;			// Calculos auxilares.
		float	length,		    // Longitud del cilindro en z.
				cos_ang, 	    // Coseno formado por los ejes del cilindro y de giro
				ang_rad,		// Angulo en radianes entre los ejes
				angle;			// Angulo en grados sexagesiamales.

		cyl_axis    = top - bottom;
		length      = cyl_axis.length();

		aux.set(bottom, versor(cyl_axis));

		// Escalamos a la mitad en z, porque nuestro cilindro base ya mide
		// dos unidades en esa dirección.
		trans->scale(a_radius, a_radius, length/2.0f);
		std::clog << "\nEscalado a : (" << a_radius << ", " << a_radius << ", " << length << ")\n";

		// Rotaremos el cilindro al eje que marque p1 - p0, para que al
		// trasladarlo hasta su lugar, los puntos coincidan.
		cyl_axis.normalize();

		// El eje de rotación será el resultado del producto vectorial
		// del eje del cilindro con el eje z, que es donde está orientado
		// el cilindro base.
		// Comprobamos que el angulo que forman los vectores es mayor que 0,
		// es decir, que no son paralelos.

		std::clog << "\nEje del cilindro: " << cyl_axis << "\n";

		cos_ang = dot(cyl_axis, Vec3(0.0f, 0.0f, 1.0f));
		std::clog << "\nCoseno: " << cos_ang << "\n";
		if(fabs(cos_ang) < 1.0f)
		{
			rotate_axis = cross(cyl_axis, Vec3(0.0f, 0.0f, 1.0f));
			rotate_axis.normalize();

			std::clog << "\nEje de rotacion: " << rotate_axis << "\n";

			// Calculamos ahora el ángulo que forman el eje del cilindro
			// con el eje z. Esto nos dirá cuanto girar el cilindro
			// sobre el eje de rotación calculado.
			// Ambos son vectores normalizados, con lo que nos ahorramos
			// la división por la multiplicacion de los modulos.

			// El angulo esta en radianes, y lo necesitamos en grados sexagesimales.
			ang_rad 	= acos(cos_ang);
			angle 		= ang_rad * (180.f / kpi);

			std::clog << "Angulo de rotacion (Sexagesimales): " << angle << "\n";
			std::clog << "Angulo de rotacion (Radianes): " << ang_rad << "\n";

			// Rotamos.
			trans->rotate(angle, rotate_axis);
		}
		else {
			std::clog << "Los ejes son paralelos. No es necesaria la rotacion.\n";
		}

		// Trasladamos el cilindro una vez girado al punto base indicado.
		// Para ello, trasladaremos nuestro cilindro que tiene su centro
		// en el origen al centro que marcan las coordenadas p0 y p1.
		trans->translate(aux.get_point(length/2.0f));

		std::clog << "Trasladamos a: " << aux.get_point(length/2.0f) << "\n";

		// Iniciamos nuestra bbox
		aabb.set(Point(-1.0f), Point(1.0f));

		// Y le aplicamos la transformación.
		aabb = trans->update_AABB(aabb);

		bottom		= Point(0.0f, 0.0f, -1.0f);
		top 		= Point(0.0f, 0.0f, 1.0f);
		radius	    = 1.0f;
		material    = a_material;
		shadow		= true;
		bounds 	    = true;
}

bool Cylinder::hit(const Ray &r, double min_dist, double max_dist, HitRecord &hit) const
{
	// Transformar el rayo
	Ray r_obj_space = trans->scene_to_object(r);

	double dist;

	if(isecaux::test_ray_cylinder(r_obj_space, min_dist, max_dist, dist)) {
		hit.dist 	= dist;
		// Para la normal, tomaremos el punto de intersección, y pondremos
		// a 0.f su coordenada z (el cilindro esta centrado en el eje Z).
		Point intersection(r_obj_space.get_point(dist));

		hit.normal      = versor(trans->normal_to_scene(Vec3(intersection.x(), intersection.y(), 0.0f)));

		hit.material	= material;

		return true;
	}

	return false;
}

bool Cylinder::shadow_hit(const Ray &r, double min_dist, double max_dist) const
{
	if(shadow) {
		// Transformar el rayo
		Ray r_obj_space = trans->scene_to_object(r);

		double dist;

		return isecaux::test_ray_cylinder(r_obj_space, min_dist, max_dist, dist);
	}

	return false;
}

bool Cylinder::get_random_point(const Point &view_pos, CRandomMersenne *rng, Point &light_pos) const
{
	return false;
}
