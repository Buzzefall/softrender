#pragma once

#include <cassert>
#include <cmath>
#include <memory>

template <int N> struct Vec {
	double data[N] = {0};

	Vec() = default;
	template <int M> Vec(const Vec<M>& original, double fill = 1.0);

	double & operator [](const int idx)			{ assert(idx >= 0 && idx < N); return data[idx]; }
	double	 operator [](const int idx) const	{ assert(idx >= 0 && idx < N); return data[idx]; }
	double norm2() const { return	*this * *this;	 }
	double norm()  const { return std::sqrt(norm2()); }
	Vec& normalize()	 { *this = *this / norm(); return *this; }
};


template <int N> template <int M> Vec<N>::Vec(const Vec<M>& original, const double fill) {
	for (int i = 0; i < N; i++) { data[i] = i < M ? original[i] : fill; }
}


template<int N> Vec<N> operator -(const Vec<N>& vec) {
	Vec<N> res = vec;
	for (int i = 0; i < N; i++) { res[i] = -vec[i]; }
	return res;
}


template<int N> Vec<N> operator *(double scalar, const Vec<N>& right) {
	Vec<N> res = right;
	for (int i = 0; i < N; i++) { res[i] *= scalar; }
	return res;
}


template<int N> Vec<N> operator *(const Vec<N>& left, double scalar) {
	Vec<N> res = left;
	for (int i = 0; i < N; i++) { res[i] *= scalar; }
	return res;
}


template<int N> Vec<N> operator /(double scalar, const Vec<N>& right) {
	Vec<N> res = right;
	for (int i = 0; i < N; i++) { res[i] /= scalar; }
	return res;
}


template<int N> Vec<N> operator /(const Vec<N>& left, double scalar) {
	Vec<N> res = left;
	for (int i = 0; i < N; i++) { res[i] /= scalar; }
	return res;
}


template<int N> double operator *(const Vec<N>& left, const Vec<N>& right) {
	double res = 0.0;
	for (int i = 0; i < N; i++) { res += left[i] * right[i]; }
	return res;
}


template<int N> Vec<N> operator +(const Vec<N>& left, const Vec<N>& right) {
	Vec<N> res;
	for (int i = 0; i < N; i++) { res[i] = left[i] + right[i]; }
	return res;
}


template<int N> Vec<N> operator -(const Vec<N>& left, const Vec<N>& right) {
	Vec<N> res;
	for (int i = 0; i < N; i++) { res[i] = left[i] - right[i]; }
	return res;
}




template <> struct Vec<2> {
	double x{}, y{};

	Vec() = default;
	Vec(double x, double y) : x(x), y(y) { }
	template <int M> Vec(const Vec<M>& original, double fill = 1.0) {
		for (int i = 0; i < 2; i++) { (*this)[i] = i < M ? original[i] : fill; }
	}

	double & operator [](const int idx)			{ assert(idx >= 0 && idx < 2); return idx ? y : x; }
	double	 operator [](const int idx) const	{ assert(idx >= 0 && idx < 2); return idx ? y : x; }
	double norm2() { return	*this * *this;		}
	double norm()  { return std::sqrt(norm2());	}
	Vec& normalize() { *this = *this / norm(); return *this; }
};




template <> struct Vec<3> {
	double x{}, y{}, z{};

	Vec() = default;
	Vec(double x, double y, double z) : x(x), y(y), z(z) { }
	template <int M> Vec(const Vec<M>& original, double fill = 1.0) {
		for (int i = 0; i < 3; i++) { (*this)[i] = i < M ? original[i] : fill; }
	}

	Vec operator ^(const Vec& right) const {
		return {
			y * right.z - z * right.y,
			z * right.x - x * right.z,
			x * right.y - y * right.x
		};
	}
	double & operator [](const int idx)			{ assert(idx >= 0 && idx < 3); return idx ? (idx==1 ? y : z) : x; }
	double	 operator [](const int idx) const	{ assert(idx >= 0 && idx < 3); return idx ? (idx==1 ? y : z) : x; }
	double norm2() { return	*this * *this;		}
	double norm()  { return std::sqrt(norm2());	}
	Vec& normalize() { *this = *this / norm(); return *this; }
	Vec& reverse()	 { std::swap(x, z); return *this; }
};




template <> struct Vec<4> {
	double x{}, y{}, z{}, w{};

	Vec() = default;
	Vec(double x, double y, double z, double wval) : x(x), y(y), z(z), w(wval) { }
	template <int M> Vec(const Vec<M>& original, double fill = 1.0) {
		for (int i = 0; i < 4; i++) { (*this)[i] = i < M ? original[i] : fill; }
	}
	
	double & operator [](const int idx)			{ assert(idx >= 0 && idx < 4); return idx ? (idx==1 ? y : (idx==2 ? z : w)) : x; }
	double	 operator [](const int idx) const	{ assert(idx >= 0 && idx < 4); return idx ? (idx==1 ? y : (idx==2 ? z : w)) : x; }
	double norm2() { return	*this * *this;		}
	double norm()  { return std::sqrt(norm2());	}
	Vec& normalize() { *this = *this / norm(); return *this; }
};




////////////////////////////////////////////////////

typedef Vec<2> Vec2;
typedef Vec<3> Vec3;
typedef Vec<4> Vec4;

////////////////////////////////////////////////////


template<int N, int M = N>
struct Mat {
	Vec<M> rows[N] = {{}};
	double _det = 0.0;

	static Mat<N> identity();
	static Mat<N> translation(const Vec<3>& offset);

		  Vec<M>& operator[] (const int idx)       { assert(idx >= 0 && idx < N); return rows[idx]; }
	const Vec<M>& operator[] (const int idx) const { assert(idx >= 0 && idx < N); return rows[idx]; }

	Mat operator -() const;
	Mat operator +(const Mat<N, M>& right) const;
	Mat operator *(double scalar) const;
	template <int K> Mat<N, K> operator *(const Mat<M, K>& right) const;

	Mat<N> faddeev_leverrier();

	double det();
	Mat<N> inverse();
	Mat<M, N> transpose() const;
	const Mat& set_row(Vec<M> row, int i);
	const Mat& set_col(Vec<N> col, int j);
};


template <int N, int M>
Mat<N, M> Mat<N, M>::operator -() const {
	auto res = Mat<N, M>();
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			res[i][j] = -rows[i][j];
		}
	}
	return res;
}


template <int N, int M>
Mat<N, M> Mat<N, M>::operator +(const Mat<N, M>& right) const {
	auto res = Mat<N, M>();
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			res[i][j] = rows[i][j] + right[i][j];
		}
	}
	return res;
}



template <int N, int M> template <int K>
Mat<N, K> Mat<N, M>::operator *(const Mat<M, K>& right) const {
	auto res = Mat<N, K>();
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			for (int k = 0; k < K; k++) {
				res[i][k] += rows[i][j] * right[j][k];
			}
		}
	}
	return res;
}


template <int N, int M>
Vec<N> operator *(const Mat<N, M>& left, const Vec<M>& right) {
	Vec<N> res;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			res[i] += left[i][j] * right[j];
		}
	}
	return res;
}


template <int N, int M>
Vec<M> operator *(const Vec<N>& left, const Mat<N, M>& right) {
	Vec<M> res;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			res[j] += left[i] * right[i][j];
		}
	}
	return res;
}


template <int N, int M>
Mat<N, M> Mat<N, M>::operator *(double scalar) const {
	auto res = Mat<M, N>();
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			res[i][j] = scalar * rows[i][j];
		}
	}
	return res;
}


template <int N, int M>
Mat<N> Mat<N, M>::identity() {
	assert (N==M);
	auto res = Mat<N>();
	for (int i = 0; i < N; i++) { res[i][i] = 1.0; }
	return res;
}

template <>
inline Mat<4> Mat<4>::translation(const Vec<3>& offset) {
	Mat<4> res = identity();
	res[0][3] = offset[0];
	res[1][3] = offset[1];
	res[2][3] = offset[2];
	return res;
}


template <int N, int M>
Mat<N> Mat<N,M>::faddeev_leverrier() {
	auto alpha_current = 0.0;
	auto L_current = Mat<N,M>::identity();
	auto L_prev = Mat<N,M>::identity();

	for (auto i = 1; i <= N; i++) {
		L_prev = L_current;
		L_current = (*this) * L_current;

		alpha_current = 0.0;
		for (int k = 0; k < N; k++) { alpha_current += L_current[k][k]; }
		alpha_current /= i;

		for (int k = 0; k < N; k++) { L_current[k][k] -= alpha_current; }
	}

	_det = (N % 2 ? -1 : 1) * alpha_current;
	if (std::abs(_det) > 1e-5) {
		return L_prev * pow(alpha_current, -1);
	}

	return Mat<N>();
}


template <int N, int M>
double Mat<N, M>::det() {
	assert (N==M);
	faddeev_leverrier();
	return _det;
}


template <int N, int M>
Mat<N> Mat<N, M>::inverse() {
	assert (N==M);
	return faddeev_leverrier();
}


template <int N, int M>
Mat<M, N> Mat<N, M>::transpose() const {
	auto res = Mat<M, N>();
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			res[i][j] = rows[j][i];
		}
	}
	return res;
}


template <int N, int M>
const Mat<N, M>& Mat<N, M>::set_row(Vec<M> row, int i) {
	this[i] = row;
	return *this;
}


template <int N, int M>
const Mat<N, M>& Mat<N, M>::set_col(Vec<N> col, int j) {
	for (int i = 0; i < N; i++) { rows[i][j] = col[i]; }
	return *this;
}


////////////////////////////////////////////////////

typedef Mat<2> Mat2;
typedef Mat<3> Mat3;
typedef Mat<4> Mat4;

////////////////////////////////////////////////////