// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#ifndef SCANTAILOR_FOUNDATION_VECNT_H_
#define SCANTAILOR_FOUNDATION_VECNT_H_

#include <QPointF>
#include <cstddef>

template <size_t N, typename T>
class VecNT;

using Vec1f = VecNT<1, float>;
using Vec1d = VecNT<1, double>;
using Vec2f = VecNT<2, float>;
using Vec2d = VecNT<2, double>;
using Vec3f = VecNT<3, float>;
using Vec3d = VecNT<3, double>;
using Vec4f = VecNT<4, float>;
using Vec4d = VecNT<4, double>;

template <size_t N, typename T>
class VecNT {
 public:
  using type = T;
  enum { SIZE = static_cast<int>(N) };

  /**
   * \brief Initializes vector elements to T().
   */
  VecNT();

  /**
   * \brief Construction from an array of elements of possibly different type.
   *
   * Conversion is done by static casts.
   */
  template <typename OT>
  VecNT(const OT* data);

  /**
   * \brief Construction from a vector of same dimension but another type.
   *
   * Conversion is done by static casts.
   */
  template <typename OT>
  VecNT(const VecNT<N, OT>& other);

  /**
   * \brief Construction from a one-less dimensional
   *        vector and the last element value.
   */
  template <typename OT>
  VecNT(const VecNT<N - 1, OT>& lesser, T last);

  /**
   * \brief 1D vector constructor.
   *
   * Will not compile for different dimensions.
   */
  explicit VecNT(T x);

  /**
   * \brief 2D vector constructor.
   *
   * Will not compile for different dimensions.
   */
  VecNT(T x, T y);

  /**
   * \brief 3D vector constructor.
   *
   * Will not compile for different dimensions.
   */
  VecNT(T x, T y, T z);

  /**
   * \brief 4D vector constructor.
   *
   * Will not compile for different dimensions.
   */
  VecNT(T x, T y, T z, T w);

  /**
   * \brief Construction from a QPointF.
   *
   * Will not compile for N != 2.  Will compile for any T's that
   * are convertable from qreal by a static cast.
   */
  VecNT(const QPointF& pt);

  /**
   * \brief Implicit conversion to QPointF.
   *
   * Will not compile for N != 2.  Will compile for any T's that
   * are convertable to qreal by a static cast.
   */
  operator QPointF() const;

  /**
   * \brief Assignment from a vector of same dimension but another type.
   *
   * Conversion is done by static casts.
   */
  template <typename OT>
  VecNT& operator=(const VecNT<N, OT>& other);

  T& operator[](size_t idx) { return m_data[idx]; }

  const T& operator[](size_t idx) const { return m_data[idx]; }

  VecNT& operator+=(T scalar);

  VecNT& operator+=(const VecNT& other);

  VecNT& operator-=(T scalar);

  VecNT& operator-=(const VecNT& other);

  VecNT& operator*=(T scalar);

  VecNT& operator/=(T scalar);

  const T* data() const { return m_data; }

  T* data() { return m_data; }

  /**
   * \brief Sums elements in the vector.
   */
  T sum() const;

  T dot(const VecNT& other) const;

  T squaredNorm() const { return dot(*this); }

 private:
  T m_data[N];
};


namespace vecnt {
template <size_t N, typename T>
struct SizeSpecific;

template <typename T>
struct SizeSpecific<1, T> {
  static void assign(T* data, T x) { data[0] = x; }
};

template <typename T>
struct SizeSpecific<2, T> {
  static void assign(T* data, T x, T y) {
    data[0] = x;
    data[1] = y;
  }

  static void assign(T* data, const QPointF& pt) {
    data[0] = static_cast<T>(pt.x());
    data[1] = static_cast<T>(pt.y());
  }

  static QPointF toQPointF(const T* data) { return QPointF(static_cast<qreal>(data[0]), static_cast<qreal>(data[1])); }
};

template <typename T>
struct SizeSpecific<3, T> {
  static void assign(T* data, T x, T y, T z) {
    data[0] = x;
    data[1] = y;
    data[2] = z;
  }
};

template <typename T>
struct SizeSpecific<4, T> {
  static void assign(T* data, T x, T y, T z, T w) {
    data[0] = x;
    data[1] = y;
    data[2] = z;
    data[3] = w;
  }
};
}  // namespace vecnt

template <size_t N, typename T>
VecNT<N, T>::VecNT() {
  for (size_t i = 0; i < N; ++i) {
    m_data[i] = T();
  }
}

template <size_t N, typename T>
template <typename OT>
VecNT<N, T>::VecNT(const OT* data) {
  for (size_t i = 0; i < N; ++i) {
    m_data[i] = static_cast<T>(data[i]);
  }
}

template <size_t N, typename T>
template <typename OT>
VecNT<N, T>::VecNT(const VecNT<N, OT>& other) {
  for (size_t i = 0; i < N; ++i) {
    m_data[i] = static_cast<T>(other[i]);
  }
}

template <size_t N, typename T>
template <typename OT>
VecNT<N, T>::VecNT(const VecNT<N - 1, OT>& lesser, T last) {
  for (size_t i = 0; i < N - 1; ++i) {
    m_data[i] = static_cast<T>(lesser[i]);
  }
  m_data[N - 1] = last;
}

template <size_t N, typename T>
VecNT<N, T>::VecNT(T x) {
  vecnt::SizeSpecific<N, T>::assign(m_data, x);
}

template <size_t N, typename T>
VecNT<N, T>::VecNT(T x, T y) {
  vecnt::SizeSpecific<N, T>::assign(m_data, x, y);
}

template <size_t N, typename T>
VecNT<N, T>::VecNT(T x, T y, T z) {
  vecnt::SizeSpecific<N, T>::assign(m_data, x, y, z);
}

template <size_t N, typename T>
VecNT<N, T>::VecNT(T x, T y, T z, T w) {
  vecnt::SizeSpecific<N, T>::assign(m_data, x, y, z, w);
}

template <size_t N, typename T>
VecNT<N, T>::VecNT(const QPointF& pt) {
  vecnt::SizeSpecific<N, T>::assign(m_data, pt);
}

template <size_t N, typename T>
VecNT<N, T>::operator QPointF() const {
  return vecnt::SizeSpecific<N, T>::toQPointF(m_data);
}

template <size_t N, typename T>
template <typename OT>
VecNT<N, T>& VecNT<N, T>::operator=(const VecNT<N, OT>& other) {
  for (size_t i = 0; i < N; ++i) {
    m_data[i] = static_cast<T>(other[i]);
  }
  return *this;
}

template <size_t N, typename T>
VecNT<N, T>& VecNT<N, T>::operator+=(T scalar) {
  for (size_t i = 0; i < N; ++i) {
    m_data[i] += scalar;
  }
  return *this;
}

template <size_t N, typename T>
VecNT<N, T>& VecNT<N, T>::operator+=(const VecNT& other) {
  for (size_t i = 0; i < N; ++i) {
    m_data[i] += other[i];
  }
  return *this;
}

template <size_t N, typename T>
VecNT<N, T>& VecNT<N, T>::operator-=(T scalar) {
  for (size_t i = 0; i < N; ++i) {
    m_data[i] -= scalar;
  }
  return *this;
}

template <size_t N, typename T>
VecNT<N, T>& VecNT<N, T>::operator-=(const VecNT<N, T>& other) {
  for (size_t i = 0; i < N; ++i) {
    m_data[i] -= other[i];
  }
  return *this;
}

template <size_t N, typename T>
VecNT<N, T>& VecNT<N, T>::operator*=(T scalar) {
  for (size_t i = 0; i < N; ++i) {
    m_data[i] *= scalar;
  }
  return *this;
}

template <size_t N, typename T>
VecNT<N, T>& VecNT<N, T>::operator/=(T scalar) {
  return *this *= (T(1) / scalar);
}

template <size_t N, typename T>
T VecNT<N, T>::sum() const {
  T sum = T();
  for (size_t i = 0; i < N; ++i) {
    sum += m_data[i];
  }
  return sum;
}

template <size_t N, typename T>
T VecNT<N, T>::dot(const VecNT& other) const {
  T sum = T();
  for (size_t i = 0; i < N; ++i) {
    sum += m_data[i] * other[i];
  }
  return sum;
}

template <size_t N, typename T>
VecNT<N, T> operator+(const VecNT<N, T>& lhs, const VecNT<N, T>& rhs) {
  VecNT<N, T> res(lhs);
  res += rhs;
  return res;
}

template <size_t N, typename T>
VecNT<N, T> operator-(const VecNT<N, T>& lhs, const VecNT<N, T>& rhs) {
  VecNT<N, T> res(lhs);
  res -= rhs;
  return res;
}

template <size_t N, typename T>
VecNT<N, T> operator-(const VecNT<N, T>& vec) {
  VecNT<N, T> res(vec);
  for (size_t i = 0; i < N; ++i) {
    res[i] = -res[i];
  }
  return res;
}

template <size_t N, typename T>
VecNT<N, T> operator*(const VecNT<N, T>& vec, T scalar) {
  VecNT<N, T> res(vec);
  res *= scalar;
  return res;
}

template <size_t N, typename T>
VecNT<N, T> operator*(T scalar, const VecNT<N, T>& vec) {
  VecNT<N, T> res(vec);
  res *= scalar;
  return res;
}

template<size_t N, typename T>
VecNT<N, T> operator/(VecNT<N, T> const& vec, T scalar)
{
  VecNT<N, T> res(vec);
  res /= scalar;
  return res;
}

template<size_t N, typename T>
VecNT<N, T> operator/(T scalar, VecNT<N, T> const& vec)
{
  VecNT<N, T> res(vec);
  res /= scalar;
  return res;
}

#endif  // ifndef SCANTAILOR_FOUNDATION_VECNT_H_
