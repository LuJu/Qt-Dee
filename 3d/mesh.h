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
#ifndef TRIANGLE_MESH3D_H
#define TRIANGLE_MESH3D_H

#include <iostream>
#include <fstream>
#include <QVector>
#include <QtOpenGL>
#include <QGLWidget>
#include "qgl.h"
#include "core/core.h"
#include "vertex.h"
#include "utils/curve.h"
class Curve;
class Mesh
{
public:
    enum filetype{
        obj
    };
    enum polygontype{
        triangle,
        lines,
        line_strip,
        points
    };

    Mesh();
    void render() const;

    void loadFromFile(QString filepath, int filetype = obj);
    GLuint get_texture() const {return _texture;}
    void set_texture(GLuint texture){_texture = texture;}

    polygontype get_type() const {return _type;}
    void set_type(const polygontype& type){_type = type;}

    QVector<unsigned short>& get_polygons() {return _polygons;}
    QVector<Vertex>& get_vertices() {return _vertices;}

    static void render(const Curve& curve, const QColor& color, int thickness, bool points=false);

    static void drawGrid(QRect bounds, const QColor &color, int thickness, int horizontal_progression ,int vertical_progression);
protected:
    GLuint _texture;
private:
    void fillVertices(const QVector<Point3df>& _temp_vertices,
                      const QVector<Point3df>& _temp_normals,
                      const QVector<Point3dus>& _temp_polygons,
                      const QVector<Point3dus>& _temp_texture_polygons,
                      const QVector<Point3dus>& _temp_normal_polygons);
    void loadFromOBJ(QString filepath);
    void simpleShape();

    QVector<unsigned short> _polygons;
    QVector<Vertex> _vertices;

    polygontype _type;
    Mesh(Mesh& triangle);
};

#endif // TRIANGLE_MESH3D_H
