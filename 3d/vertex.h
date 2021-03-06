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
#ifndef VERTEX_H
#define VERTEX_H

#include "utils/geometry.h"
#include "utils/typedefinitions.h"

//! Vertex class made to be used with opengl
/*!
        Represents a vertex defined by its coordinates, its color, its normal, and the coordinates of the texture
    */
class Vertex
{
public:
    Vertex();
    Vertex(float x, float y, float z);
    Vertex(Point3d<float> point);

    bool operator=(const Vertex &b);
    void set_color(float r, float g, float b, float a);

    void colorFromPoint(){
        int i=0;
        Point3df point = _point;
        point.normalize();
        _color[i++] = point.x();
        _color[i++] = point.y();
        _color[i++] = point.z();
        _color[i++] = 1.0f;
    }
    void colorFromNormal(){
        int i=0;
        _color[i++] = _normal.x();
        _color[i++] = _normal.y();
        _color[i++] = _normal.z();
        _color[i++] = 1.0f;
    }

    void normalFromPoint(){
        int i=0;
        _normal[i++] = _point.x();
        _normal[i++] = _point.y();
        _normal[i++] = _point.z();
    }

    Point3df _point;
    Point3df _normal;
    Point3df _texture;
    float _color[4];
};
bool operator==(const Vertex& a, const Vertex& b);
#endif // VERTEX_H
