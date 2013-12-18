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
#ifndef EXTENDEDTRIPLET_H
#define EXTENDEDTRIPLET_H

#include "triplet.h"

//! Class that extends the Triplet class and adds the +. -. +=, -= operators.
template<class T> class ExtendedTriplet : public Triplet<T>
{
public:
    inline ExtendedTriplet();
    inline ExtendedTriplet(T x, T y, T z);
    inline ExtendedTriplet(const ExtendedTriplet<T>& v);

    inline ExtendedTriplet operator+() const;
    inline ExtendedTriplet operator-() const;
    inline ExtendedTriplet& operator+=(const ExtendedTriplet<T>& a);
    inline ExtendedTriplet& operator-=(const ExtendedTriplet<T>& a);
};

template<class T> ExtendedTriplet<T>::ExtendedTriplet()
{
}

template<class T> ExtendedTriplet<T>::ExtendedTriplet(const ExtendedTriplet<T>& v) :
    Triplet<T>(v)
{
}

template<class T> ExtendedTriplet<T>::ExtendedTriplet(T x, T y, T z) :
    Triplet<T>(x,y,z)
{
}

template<class T> ExtendedTriplet<T>& ExtendedTriplet<T>::operator+=(const ExtendedTriplet<T>& a)
{
    Triplet<T>::_x += a.x(); Triplet<T>::_y += a.y(); Triplet<T>::_z += a.z();
    return *this;
}

template<class T> ExtendedTriplet<T>& ExtendedTriplet<T>::operator-=(const ExtendedTriplet<T>& a)
{
    Triplet<T>::_x -= a.x(); Triplet<T>::_y -= a.y(); Triplet<T>::_z -= a.z();
    return *this;
}

template<class T> ExtendedTriplet<T> ExtendedTriplet<T>::operator-() const
{
    return ExtendedTriplet<T>(-Triplet<T>::_x, -Triplet<T>::_y, -Triplet<T>::_z);
}

template<class T> ExtendedTriplet<T> ExtendedTriplet<T>::operator+() const
{
    return *this;
}


template<class T> ExtendedTriplet<T> operator+(const ExtendedTriplet<T>& a, const ExtendedTriplet<T>& b)
{
    return ExtendedTriplet<T>(a.x() + b.x(), a.y() + b.y(), a.z() + b.z());
}

template<class T> ExtendedTriplet<T> operator-(const ExtendedTriplet<T>& a, const ExtendedTriplet<T>& b)
{
    return ExtendedTriplet<T>(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
}



#endif // EXTENDEDTRIPLET_H
