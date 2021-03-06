#include <vector>

#include "matrix/matrix.h"

Matrix4x4::Matrix4x4()
{
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(i==j)
				e[i][j] = 1.0;
            else
				e[i][j] = 0.0;
		}
    }
}

Matrix4x4::Matrix4x4(const Matrix4x4 &m)
{
    *this = m;
}

void Matrix4x4::set(double m[4][4])
{
	for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
			e[i][j] = m[i][j];
}

void Matrix4x4::set(double m[16])
{
	for(int i = 0, k = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
			e[i][j] = m[k++];
}

Matrix4x4 Matrix4x4::operator-() const
{
    Matrix4x4 m;

    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            if(m.e[i][j] != 0.0f)
                m.e[i][j] = -e[i][j];

    return m;
}

Matrix4x4 &Matrix4x4::operator+=(Matrix4x4 const &m)
{
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            e[i][j] += m.e[i][j];

    return *this;
}

Matrix4x4 &Matrix4x4::operator-=(Matrix4x4 const &m)
{
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            e[i][j] -= m.e[i][j];

    return *this;
}

Matrix4x4 &Matrix4x4::operator*=(Matrix4x4 const &m)
{
    Matrix4x4 mt(*this);

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            double sum = 0.0;
            for(int k = 0; k < 4; k++)
				sum += e[i][k] * m.e[k][j];

            mt.e[i][j] = sum;
        }
    }

    *this = mt;

    return *this;
}

Matrix4x4 &Matrix4x4::operator*=(double v)
{
    for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			e[i][j] *= v;

    return *this;
}

Matrix4x4 &Matrix4x4::operator/=(const Matrix4x4 &m)
{
    /*
    Matrix4x4 mres, mtemp;

    mtemp = m2;
    mtemp.invert();

    mres = m1 * mtemp;

    return mres;
    */

    Matrix4x4 mt {m};

    mt.invert();

    *this *= mt;

    return *this;
}

Matrix4x4 &Matrix4x4::operator/=(double v)
{
    // Si intentamos dividir por 0.0, devolvemos la matriz identidad.
    if( v != 0.0f) {
        double inv = 1.0 / v;

        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                e[i][j] *= inv;
    }
    else {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                if(i==j)
                    e[i][j] = 1.0;
                else
                    e[i][j] = 0.0;
            }
        }

        std::clog << "----\nMatriz: Division por 0.\n---\n" << std::endl;
    }

    return *this;
}

Matrix4x4 Matrix4x4::get_inv()
{
	// Aplicaremos el método de cálculo por determinates:
	// Sea una matriz A, se define I(A) como su matriz inversa:
	//
	//                  T(C(A))
	//			I(A) = ----------
	//                    |A|
	//
	// Donde C(A) es la matriz de cofactores de A, T(A) es la transpuesta
	// de A y |A| es el determinante de A.
	// Luego A tiene inversa si y solo si su determinante es distinto de 0.

	Matrix4x4 mtemp;

	// Calculamos el determinante. Si es cero, devolvemos la matriz identidad.
	double det = determinant();

	if(det != 0.0f) {
		// Calculamos la matriz de cofactores
		mtemp = get_cofactor();

		// La transponemos
		mtemp.transpose();

		// Y la dividimos por el determinante.
		mtemp = mtemp / det;
	}
	else {
		std::clog << "----\nMatriz SIN inversa posible.\n---\n" << std::endl;
		std::clog << *this << std::endl;
		std::clog << "Determinante: " << det << std::endl;
	}

	return mtemp;
}

Matrix4x4 Matrix4x4::get_trans()
{
	Matrix4x4 mtemp;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            mtemp.e[i][j] = e[j][i];

    return mtemp;
}

Matrix4x4 Matrix4x4::get_cofactor()
{
	Matrix4x4 mtemp;

	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			mtemp.e[i][j] = cofactor(i, j);

    return mtemp;
}

Matrix4x4 operator+(Matrix4x4 m1, const Matrix4x4 &m2)
{
    return m1 += m2;
}

Matrix4x4 operator-(Matrix4x4 m1, const Matrix4x4 &m2)
{
	return m1 -= m2;
}

Matrix4x4 operator*(Matrix4x4 m1, const Matrix4x4 &m2)
{
    return m1 *= m2;
}

Matrix4x4 operator/(Matrix4x4 m1, const Matrix4x4 &m2)
{
    /*
    Matrix4x4 mres, mtemp;

    mtemp = m2;
    mtemp.invert();

    mres = m1 * mtemp;

    return mres;
    */
    return m1 /= m2;
}

Matrix4x4 operator*(Matrix4x4 m, double v)
{
    Matrix4x4 mtemp;

	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			mtemp.e[i][j] = m.e[i][j] * v;

	return mtemp;
	//return m *= v;
}

Matrix4x4 operator/(Matrix4x4 m, double v)
{
	Matrix4x4 mtemp;

	if(v != 0.0) {
        double inv = 1.0f / v;

		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				mtemp.e[i][j] = m.e[i][j] * inv;
	}

	return mtemp;
	//return m /= v;
}

double Matrix4x4::determinant()
{
	// La funcion CofactorXY ya devuelve el signo adecuado para la
	// posición calculada, con lo que simplemente tenemos que sumar.
	return  e[0][0] * cofactor(0, 0) +
            e[0][1] * cofactor(0, 1) +
            e[0][2] * cofactor(0, 2) +
            e[0][3] * cofactor(0, 3);
}

double Matrix4x4::det3x3(
	double a11, double a12, double a13,
	double a21, double a22, double a23,
	double a31, double a32, double a33
)
{
	// Aplicamos la regla de Sarrus
	return  a11 * a22 * a33 +
            a12 * a23 * a31 +
            a13 * a21 * a32 -
            a13 * a22 * a31 -
            a12 * a21 * a33 -
            a11 * a23 * a32;
}

double Matrix4x4::cofactor(int x, int y)
{
	std::vector<float> m3;

	// Almacenamos por orden los 9 numeros resultantes de eliminar
	// de la matriz de 4x4 la fila X y la columna Y.
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			if((i != x) && (j != y))
				m3.push_back(e[i][j]);


	// Calculamos el signo que corresponde a este cofactor
	int sign = (((x+y) % 2) == 0) ? 1 : -1;

	// Calculamos el determinante
	double det = det3x3(
					m3[0], m3[1], m3[2],
					m3[3], m3[4], m3[5],
					m3[6], m3[7], m3[8]
				);

	// Evitamos el cero negativo
	if((det < 0.0f) || (det > 0.0f))
		det = sign * det;
	else
		det = 0.0f;

	return det;
}

void Matrix4x4::transform(double *v)
{
    // Multiplicamos una matriz de 4 filas y 1 columna
    // por una matriz de 4 filas y 4 columnas.
    for(int j = 0; j < 4; ++j) {
        double sum = 0.0f;

        for(int i = 0; i < 4; ++i) {
            sum += e[j][i] * v[i];
        }
        v[j] = sum;
    }
}

Vec3 Matrix4x4::transform_pos(const Vec3 &pos)
{
    double  v[4];

    v[0] = pos.x();
    v[1] = pos.y();
    v[2] = pos.z();
    v[3] = 1.0f;

    transform(v);

    return Vec3(v[0], v[1], v[2]);
}

Vec3 Matrix4x4::transform_vec(const Vec3 &vec)
{
    double v[4];

    v[0] = vec.x();
    v[1] = vec.y();
    v[2] = vec.z();
    v[3] = 0.0f;

    transform(v);

    return Vec3(v[0], v[1], v[2]);
}
