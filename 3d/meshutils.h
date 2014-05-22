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
#ifndef MESHUTILS_H
#define MESHUTILS_H

#include "mesh.h"
#include "vertex.h"
#include "bezierpath.h"
#include "utils/curve.h"

class MeshUtils
{
public:
    static void addHalfSphere (Mesh * mesh, float position_y, float radius, bool up);
    static void addHalfSphereX(Mesh * mesh, float position_x, float radius, bool up);
    static void addHalfSphereY(Mesh * mesh, float position_y, float radius, bool up);
    static void addHalfSphereZ(Mesh * mesh, float position_z, float radius, bool up);

    static void addTube(Mesh * mesh, float length, float radius);
    static void addTubeX(Mesh * mesh, float length, float radius);
    static void addTubeY(Mesh * mesh, float length, float radius);
    static void addTubeZ(Mesh * mesh, float length, float radius);

    static void addCapsuleShape(Mesh * mesh,float length, float radius);
    static void addCapsuleShapeX(Mesh * mesh,float length, float radius);
    static void addCapsuleShapeY(Mesh * mesh,float length, float radius);
    static void addCapsuleShapeZ(Mesh * mesh,float length, float radius);

    static void addCircle(Mesh *mesh, float position_y, float radius);

    static void addFlatSurface(Mesh * mesh);

    static void render(const Curve& curve, int normalization, const QColor& color, int thickness, bool points=false);
    static void render(const BezierPath& path);
    static void render(const Point3df& point);
    static void drawGrid(QRect bounds, const QColor &color, int thickness, int horizontal_progression ,int vertical_progression);

    virtual void abstract()=0;
};

#endif // MESHUTILS_H
