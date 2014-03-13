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
#ifndef GRAPHICOBJECT3D_H
#define GRAPHICOBJECT3D_H

#include <QVector>
#include <QMatrix4x4>

#include "utils/geometry.h"

#include "mesh.h"
#include "transform.h"

//! Class representing a 3-dimensionnal object.
//! It is visually defined by a 3D mesh, wich can be accessed with the get_mesh and set_mesh methods
class GraphicObject3D
{
public:
    GraphicObject3D();
    ~GraphicObject3D();

    //! Load the file given in path
    void loadFromAsset(QString path);

    //! Renders the mesh of the object
    void render();
    void resetPosition();

    bool get_visible() const {return _visible;}
    void set_visible(bool visible){_visible = visible;}

    bool is_moving() const {return _moving;}
    void set_moving(bool moving){_moving = moving;}

    MeshPointer get_mesh() {return _mesh;}
    void set_mesh(MeshPointer mesh){_mesh = mesh;}

    int get_type() const {return _type;}
    void set_type(int type){_type = type;}

    Transform& get_transform() {return _transform;}
    void set_transform(Transform& transform){_transform = transform;}
    QMatrix4x4 get_matrix();

protected:
    void computeTransformations();
    int _type;
    MeshPointer _mesh;
    bool _visible;
    bool _moving;
    Transform _transform;
    QMatrix4x4 _matrix;
private:
};

#endif // GRAPHICOBJECT3D_H
