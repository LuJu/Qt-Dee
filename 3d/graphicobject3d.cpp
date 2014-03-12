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
#include "3d/graphicobject3d.h"

GraphicObject3D::GraphicObject3D():
    _visible(true),
    _moving(false),
    _mesh(NULL){
}

GraphicObject3D::~GraphicObject3D(){
}

QMatrix4x4 GraphicObject3D::get_matrix(){
    computeTransformations();
    return _matrix;
}

void GraphicObject3D::render(){
    if(_mesh==NULL) qWarning()<<"Mesh not defined";
    else _mesh->render();
}

void GraphicObject3D::computeTransformations(){
    _matrix.setToIdentity();
    _matrix.translate(_transform.get_position().x(),_transform.get_position().y(),_transform.get_position().z());
    _matrix.scale(_transform.get_scale().x(),_transform.get_scale().y(),_transform.get_scale().z());
    _matrix.rotate(_transform.get_rotate());
}

void GraphicObject3D::resetPosition(){
    _transform.set_position(Point3df(0.0f,0.0f,0.0f));
}

