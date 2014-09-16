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
#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QSharedPointer>
#include "mesh.h"

class OBJLoader
{
public:
    OBJLoader();
    ~OBJLoader(){
        _mesh.clear();
    }

    QSharedPointer<Mesh> get_mesh(){return _mesh;}
    void parseOBJ(QString filepath);

private:
        GLuint loadTexture(const QString &textureName);
    QSharedPointer<Mesh> _mesh;
    void parseMaterials(const QString& material_path);
    void fillVertice(
                      const QVector<Point3df>& _temp_vertices,
                      const QVector<Point3df>& _temp_normals,
                      const QVector<Point3df>& _temp_textures,
                      const Point3dus& temp_polygon,
                      const Point3dus& temp_normal_polygon,
                      const Point3dus& temp_texture_polygon,
                      bool insert_normal, bool insert_texture);

    int findMaterialIndex(const QString& name);

};

#endif // OBJLOADER_H
