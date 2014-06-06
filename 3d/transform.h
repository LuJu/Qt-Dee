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
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "utils/geometry.h"
#include "utils/typedefinitions.h"
#include <QQuaternion>
#include <QMatrix4x4>

class Transform
{
public:
    Transform();
    virtual ~Transform(){}
    const QQuaternion& get_rotate() const { return _rotate; }
    void set_rotate(const QQuaternion& rotate){_rotate = rotate;}
    const Point3df& get_position() const {return _position;}
    void set_position(Point3df position){_position = position;}
    const Point3df& get_scale() const {return _scale;}
    void set_scale(Point3df scale){_scale = scale;}
    void set_scale(float scale){_scale = Point3df(scale,scale,scale);}
    QMatrix4x4 get_matrix(){
        QMatrix4x4 matrix;
        matrix.setToIdentity();
        matrix.translate(_position.x(),_position.y(),_position.z());
        matrix.scale(_scale.x(),_scale.y(),_scale.z());
        matrix.rotate(_rotate);
        return matrix;
    }

    Transform & mult(const Transform& other){
        Transform ret;
        ret.set_position(this->_position + other._position);
        ret.set_scale(this->_scale + other._scale);
        ret.set_rotate(this->_rotate * other._rotate);

    }

private :
    QQuaternion _rotate;
    Point3df _position;
    Point3df _scale;

};

#endif // TRANSFORM_H
