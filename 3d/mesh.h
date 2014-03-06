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

#include "vertex.h"
#include "material.h"

#include "utils/curve.h"

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

    static void render(const Curve& curve, int normalization, const QColor& color, int thickness, bool points=false);

    static void drawGrid(QRect bounds, const QColor &color, int thickness, int horizontal_progression ,int vertical_progression);
    GLuint _texture;

    void fromCapsuleShape(float length, float radius);
    void addCircle(float position_y, float radius);
    void addHalfSphere(float position_y, float radius, bool up);
    void addTube(float length, float radius);

    Point3df calculateNormal(Point3df u, Point3df v);
    Point3df addPolygon(Vertex &a, Vertex &b, Vertex &c);

    void parseMaterials(const QString& material_path);
    int findMaterialIndex(const QString& name);
    GLuint loadTexture(const QString &textureName);

    bool has_materials () const {return (_materials.size()>0);}
    bool has_vertices () const {return (_vertices.size()>0);}
    void set_texture_activated(bool texture_activated){ _textures_activated = texture_activated;}
    void set_color_activated(bool color_activated) {_colors_activated = color_activated;}
    void set_normal_activated(bool normal_activated) { _normals_activated = normal_activated;}

protected:
    void enableClientStates() const;
    void disableClientStates() const;
    void insertArrayValues() const;
    void drawTextures(const unsigned short *polygons, int number_of_polygons) const;
private:
    void fillVertice(
                    const QVector<Point3df>& _temp_vertices,
                    const QVector<Point3df>& _temp_normals,
                    const QVector<Point3df>& _temp_textures,
                    const Point3dus& temp_polygon,
                    const Point3dus& temp_normal_polygon,
                    const Point3dus& temp_texture_polygon,
                    bool insert_normal=true,
                    bool insert_texture=true);
    void loadFromOBJ(QString filepath);
    void simpleShape();

    QVector<unsigned short> _polygons;
    QVector<Vertex> _vertices;

    polygontype _type;
    Mesh(Mesh& triangle);

    bool _normals_activated;
    bool _textures_activated;
    bool _colors_activated;

    QMap<int,int>  _material_indices; // first polygon <> material index
    QList<Material> _materials;
};

#endif // TRIANGLE_MESH3D_H
