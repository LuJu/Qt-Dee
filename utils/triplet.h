/*
Copyright (c) 2013, Lucas Juliéron
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef TRIPLET_H
#define TRIPLET_H

#include <QDebug>

//! Container of 3 objects of the same type.
//! The different values can be accessed with the [] operator or with the x,y and z methods
template<class T> class Triplet
{
public:
    inline Triplet();
    inline Triplet(T x, T y, T z);
    inline Triplet(const Triplet<T>& t);

    inline T& operator[](int) const;

    inline T x()const{return _x;}
    inline T y()const{return _y;}
    inline T z()const{return _z;}

    inline void x(const T x){_x=x;}
    inline void y(const T y){_y=y;}
    inline void z(const T z){_z=z;}

protected:
    T _x, _y, _z;
};


template<class T> Triplet<T>::Triplet()
{
}

template<class T> Triplet<T>::Triplet(const Triplet<T>& t) :
    _x(t.x()), _y(t.y()), _z(t.z())
{
}

template<class T> Triplet<T>::Triplet(T x, T y, T z) : _x(x), _y(y), _z(z)
{
}

template<class T> T& Triplet<T>::operator[](int n) const
{
    return ((T*) this)[n];
}

template<class T> bool operator==(const Triplet<T>& a, const Triplet<T>& b)
{
    return a.x() == b.x() && a.y() == b.y() && a.z() == b.z();
}

template<class T> bool operator!=(const Triplet<T>& a, const Triplet<T>& b)
{
    return a.x() != b.x() || a.y() != b.y() || a.z() != b.z();
}


#endif // TRIPLET_H
