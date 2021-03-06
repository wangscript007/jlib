/* -*- mode: C++ c-basic-offset: 4 -*-
 * 
 * Copyright (c) 1999 Joey Yandle <dragon@dancingdragon.net>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */

#ifndef JLIB_CRYPT_CURVE_HH
#define JLIB_CRYPT_CURVE_HH

#include <ostream>

#include <sodium.h>
#include <sodium/crypto_core_ristretto255.h>

#include <jlib/util/util.hh>

namespace jlib {
namespace crypt {
namespace curve {

class BasePoint;
class Point;
class Scalar;
class Commitment;

template<int N>
class Hash {
public:
    Hash();

    void update(const Point& p);
    void update(const Scalar& x);
    void update(const Commitment& c);
    void update(const unsigned char* data, std::size_t n);
    void finalize();
    
    static Hash generic(const Point& p);
    static Hash generic(const Scalar& x);
    static Hash generic(const Commitment& x);
    static Hash generic(const unsigned char* data, std::size_t n);
 
    friend class Point; 
    friend class Scalar;
  
protected:
    unsigned char m_data[crypto_generichash_BYTES];
    crypto_generichash_state m_state;
};

template<int N, typename... Args>
Hash<N> hash(Args&&... args);

template<int N, typename T, typename... Args>
Hash<N> do_hash(Hash<N>& hasher, const T& t, Args&&... args);

template<int N, typename T>
Hash<N> do_hash(Hash<N>& hasher, const T& t);

class Scalar {
public:
    static const int HASHSIZE = crypto_core_ristretto255_HASHBYTES;
    static const int SIZE = crypto_core_ristretto255_SCALARBYTES;

    struct Power {
        static Scalar pow(const Scalar& x, const int& y) {
            return (x^y);
        }
    };
    
    Scalar();
    Scalar(std::size_t x);
    Scalar(const Hash<HASHSIZE>& hash);

    Scalar operator+(const Scalar& x) const;
    Scalar operator-(const Scalar& x) const;
    Scalar operator*(const Scalar& x) const;
    Scalar operator^(int k) const;
    Point operator*(const Point& x) const;
    Point operator*(const BasePoint& x) const;
    Scalar operator-() const;
    Scalar& operator+=(const Scalar& x);
    Scalar& operator-=(const Scalar& x);
    Scalar& operator*=(const Scalar& x);

    const unsigned char* data() const;
    unsigned char* data();

    static Scalar random();
    static Scalar zero();
    static Scalar one();
    
    template<int N>
    friend class Hash;
    friend class Point;
    friend class BasePoint;

    friend std::ostream& operator<<(std::ostream& out, const Scalar& d);
    friend bool operator==(const Scalar& x, const Scalar& y);
    
protected:
    unsigned char m_data[crypto_core_ristretto255_SCALARBYTES];
};
    
class Point {
public:
    static const int HASHSIZE = crypto_core_ristretto255_HASHBYTES;
    static const int SIZE = crypto_core_ristretto255_BYTES;

    Point();
    Point(const Scalar& scalar);
    Point(const Hash<HASHSIZE>& hash);

    Point operator+(const Point& x) const;
    Point operator-(const Point& x) const;
    Point operator*(const Scalar& x) const;
    Point& operator+=(const Point& x);
    Point& operator*=(const Scalar& x);
    
    const unsigned char* data() const;
    unsigned char* data();

    static Point zero();
    static Point random();
    static Point from(const Scalar& x);

    friend std::ostream& operator<<(std::ostream& out, const Point& d);
    friend bool operator==(const Point& x, const Point& y);

    template<int N>
    friend class Hash;
    friend class BasePoint;
    friend class Commitment;
    
protected:
    unsigned char m_data[crypto_core_ristretto255_BYTES];
};

class BasePoint : public Point {
public:
    BasePoint();

    Point operator*(const Scalar& x) const;
};

    
std::ostream& operator<<(std::ostream& out, const Scalar& d);
std::ostream& operator<<(std::ostream& out, const Point& d);

bool operator==(const Scalar& x, const Scalar& y);
bool operator!=(const Scalar& x, const Scalar& y);
bool operator==(const Point& x, const Point& y);
bool operator!=(const Point& x, const Point& y);

class Commitment : public Point {
public:
    Commitment();
    Commitment(const Scalar& value);
    Commitment(const Scalar& value, const Scalar& blind);
    
    const Scalar& value() const;
    const Scalar& blind() const;

    static BasePoint G;
    static Point H;

protected:
    Scalar m_value;
    Scalar m_blind;
};
    
template<int N>
Hash<N>::Hash() {
    crypto_generichash_init(&m_state, 0, 0, N);
}

template<int N>
void Hash<N>::update(const Point& p) {
    crypto_generichash_update(&m_state, p.data(), Point::SIZE);
}
    
template<int N>
void Hash<N>::update(const Scalar& x) {
    crypto_generichash_update(&m_state, x.data(), Scalar::SIZE);
}
    
template<int N>
void Hash<N>::update(const Commitment& c) {
    crypto_generichash_update(&m_state, c.data(), Point::SIZE);
}
    
template<int N>
void Hash<N>::update(const unsigned char* data, std::size_t n) {
    crypto_generichash_update(&m_state, data, n);
}
    
template<int N>
void Hash<N>::finalize() {
    crypto_generichash_final(&m_state, reinterpret_cast<unsigned char*>(&m_data), N);
}
    
template<int N>
Hash<N> Hash<N>::generic(const Point& p) {
    return Hash<N>::generic(reinterpret_cast<const unsigned char*>(&p.m_data), Point::SIZE);
}

template<int N>
Hash<N> Hash<N>::generic(const Scalar& x) {
    return Hash<N>::generic(reinterpret_cast<const unsigned char*>(&x.m_data), Scalar::SIZE);
}

template<int N>
Hash<N> Hash<N>::generic(const Commitment& c) {
    return Hash<N>::generic(reinterpret_cast<const unsigned char*>(c.data()), Point::SIZE);
}

template<int N>
Hash<N> Hash<N>::generic(const unsigned char* data, std::size_t n) {
    Hash<N> result;
    
    crypto_generichash(reinterpret_cast<unsigned char*>(&result.m_data), crypto_generichash_BYTES, data, n, NULL, 0);

    return result;
}
    
template<int N, typename... Args>
Hash<N> hash(Args&&... args) {
    Hash<N> hasher;

    do_hash(hasher, args...);
    hasher.finalize();
    
    return hasher;
}

template<int N, typename T, typename... Args>
Hash<N> do_hash(Hash<N>& hasher, const T& t, Args&&... args) {
    hasher.update(t.data(), T::SIZE);
    do_hash(hasher, args...);
}

template<int N, typename T>
Hash<N> do_hash(Hash<N>& hasher, const T& t) {
    hasher.update(t.data(), T::SIZE);
}

}
}
}

#endif
