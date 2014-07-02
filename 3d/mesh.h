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
#ifndef MESH_H
#define MESH_H

//#include <iostream>
//#include <fstream>
#include <QVector>
#include <QtOpenGL>
#include <QGLWidget>
#include <QFile>
#include <QTextStream>

#include "vertex.h"
#include "material.h"

class Mesh
{
    friend class OBJLoader;
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

    //! Renders the mesh using the opengl system
    void render() const;
//    void loadFromFile(QString filepath, int filetype = obj);
    //! Adds a polygon to the list, with the vertices given in parameter
    /*!
        Adds a polygon to the list, with the vertices given in parameter.\n
        The vertices are added to the vertices list, so if you want to define a polygon with already defined vertices, refer to ...
        \n Status  1 : not implemented
        \param a,b,c : the vertices defining the polygon to add. \n
    */
    void addPolygon(const Vertex &a, const Vertex &b, const Vertex &c);
    //! adds a polygon with indices of the vertices in parameter
    void addPolygon(int index_a, int index_b, int index_c);


    Point3df calculateNormal(Point3df u, Point3df v, Point3df w);

    void clear(){
        _polygons.clear();
        _vertices.clear();
        _materials.clear();
        _material_indices.clear();
        _normals_activated = false;
        _textures_activated = false;
        _colors_activated = false;
        _type = triangle;
    }

    QVector<unsigned short>& get_polygons() {return _polygons;}
    QVector<Vertex>& get_vertices() {return _vertices;}
    bool has_materials () const {return (_materials.size()>0);}
    bool has_vertices () const {return (_vertices.size()>0);}
    polygontype get_type() const {return _type;}
    void set_type(const polygontype& type){_type = type;}
    void set_texture_activated(bool texture_activated){ _textures_activated = texture_activated;}
    void set_color_activated(bool color_activated) {_colors_activated = color_activated;}
    void set_normal_activated(bool normal_activated) { _normals_activated = normal_activated;}

    void initBuffer() const;

    void activateBuffering();

protected:
    void enableClientStates() const;
    void disableClientStates() const;
    void insertArrayValues() const;
    void drawElements(const unsigned short *polygons, int number_of_polygons) const;
    GLuint loadTexture(const QString &textureName);
//    void parseMaterials(const QString& material_path);
//    int findMaterialIndex(const QString& name);
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

    Mesh(Mesh& triangle);

//    void parseOBJ(QString filepath);

    QVector<unsigned short> _polygons;
    QVector<Vertex> _vertices;

    polygontype _type;
    QMap<int,int>  _material_indices; // first polygon <> material index
    QList<Material> _materials;

    bool _normals_activated;
    bool _textures_activated;
    bool _colors_activated;
    mutable QGLBuffer _vertex_buffer_object;
    mutable QGLBuffer _index_buffer_object;
    mutable bool _is_buffered;
    bool _to_buffer;

};
typedef QSharedPointer<Mesh> MeshPointer;

#endif // MESH_H
