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
#include "mesh.h"


//using namespace std;

Mesh::Mesh():
    _type(triangle),
    _normals_activated(false),
    _textures_activated(false),
    _colors_activated(false),
    _is_buffered(false)
{
}


void Mesh::enableClientStates() const{

    glEnableClientState(GL_VERTEX_ARRAY);
    if(_normals_activated)
        glEnableClientState(GL_NORMAL_ARRAY);
    if(_colors_activated)
        glEnableClientState(GL_COLOR_ARRAY);
    if(_textures_activated)
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

}

void Mesh::disableClientStates() const{
    glDisableClientState(GL_VERTEX_ARRAY);
    if(_normals_activated)
        glDisableClientState(GL_NORMAL_ARRAY);
    if(_colors_activated)
        glDisableClientState(GL_COLOR_ARRAY);
    if(_textures_activated)
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Mesh::insertArrayValues() const{

    if (has_vertices()){
        const Vertex * vertices =_vertices.constData();
        int size = sizeof(vertices[0]);
        const Point3df * vertices_array = &(vertices[0]._point);
        const Point3df * normal_array   = &(vertices[0]._normal);
        const Point3df * textures_array = &(vertices[0]._texture);
        const float *    color_array    =   vertices[0]._color;

        if (_to_buffer && !_is_buffered){
            initBuffer();
        }

        if (_is_buffered){
            _vertex_buffer_object.bind();

            glVertexPointer(3,GL_FLOAT,size,0);
            if(_normals_activated)
                glNormalPointer(GL_FLOAT,size,0);
            if(_colors_activated)
                glColorPointer(4,GL_FLOAT,size,0);
            if(_textures_activated)
                glTexCoordPointer(3,GL_FLOAT,size,0);
            _vertex_buffer_object.release();
        } else {
            QOpenGLVertexArrayObject lol;
//            glEnableVertexAttribArray(0);
//            glVertexAttribPointer(0,3,GL_FLOAT,size,vertices_array);
            glVertexPointer(3,GL_FLOAT,size,vertices_array);
            if(_normals_activated)
                glNormalPointer(GL_FLOAT,size,normal_array);
            if(_colors_activated)
                glColorPointer(4,GL_FLOAT,size,color_array);
            if(_textures_activated)
                glTexCoordPointer(3,GL_FLOAT,size,textures_array);
        }
    } else {
//        qDebug()<<"Attempting to render empty mesh";
    }
}



void Mesh::initBuffer() const {
    _vertex_buffer_object = QGLBuffer(QGLBuffer::VertexBuffer);
    if (_vertex_buffer_object.create()){
        _is_buffered = true;
        _vertex_buffer_object.bind();

        const Vertex * vertices =_vertices.constData();
        int size = sizeof(vertices[0]) * _vertices.size();
        _vertex_buffer_object.allocate(vertices,size);
        _vertex_buffer_object.release();

        const unsigned short * polygons = _polygons.constData();
        for (int i = 0; i < _material_indices.size(); ++i) {
            QGLBuffer index_buffer_object(QGLBuffer::IndexBuffer);
            if (index_buffer_object.create()){
                int current_polygon_index = _material_indices.keys()[i];
                int next_polygon_index;
                if ( i < _material_indices.size()-1)
                    next_polygon_index = _material_indices.keys()[i+1];
                else
                    next_polygon_index = _polygons.size();

                int number_of_polygons = next_polygon_index - current_polygon_index;
                size = sizeof(unsigned short) * number_of_polygons;
                index_buffer_object.bind();
                const unsigned short * poly_pointer = polygons;
                poly_pointer += current_polygon_index;
                index_buffer_object.allocate(poly_pointer,size);
                index_buffer_object.release();
                _index_buffer_object_array.append(index_buffer_object);
            } else qWarning()<<"Could not create index buffer object";
        }
    } else qWarning()<<"Could not create vertex buffer object";
}

void Mesh::activateBuffering(){
    _to_buffer = true;
}

void Mesh::drawElements(const unsigned short * polygons, int number_of_polygons, int index) const{
    if (!_is_buffered){
        switch(_type){
        case triangle :
            glDrawElements(GL_TRIANGLES, number_of_polygons,GL_UNSIGNED_SHORT,polygons);
            break;
        case lines:
            glDrawElements(GL_LINES,     number_of_polygons,GL_UNSIGNED_SHORT,polygons);
            break;
        case line_strip:
            glDrawElements(GL_LINE_STRIP,number_of_polygons,GL_UNSIGNED_SHORT,polygons);
            break;
        case points:
            glDrawElements(GL_POINTS,    number_of_polygons,GL_UNSIGNED_SHORT,polygons);
            break;
        }
    } else {
        _index_buffer_object_array[index].bind();
        switch(_type){
        case triangle :
            glDrawElements(GL_TRIANGLES, number_of_polygons,GL_UNSIGNED_SHORT,0);
            break;
        case lines:
            glDrawElements(GL_LINES,     number_of_polygons,GL_UNSIGNED_SHORT,0);
            break;
        case line_strip:
            glDrawElements(GL_LINE_STRIP,number_of_polygons,GL_UNSIGNED_SHORT,0);
            break;
        case points:
            glDrawElements(GL_POINTS,    number_of_polygons,GL_UNSIGNED_SHORT,0);
            break;
        }
        _index_buffer_object_array[index].release();
    }
}

void Mesh::render() const{
    int current_polygon_index = 0;
    int next_polygon_index = 0;
    int number_of_polygons = 0;
    Material current_material;
    int current_material_index = 0;
    GLuint texture;
    const unsigned short * polygons;

    enableClientStates();
    insertArrayValues();

    if(has_materials()){
        while(current_material_index<_material_indices.size()){

            current_polygon_index = _material_indices.keys()[current_material_index];
            if (_material_indices.value(_material_indices.keys()[current_material_index]) == -1) {
                qWarning()<<"Material not found";
            } else {
                current_material = _materials[_material_indices.value(_material_indices.keys()[current_material_index])];
                texture = current_material._texture_index;

                if ( current_material_index < _material_indices.size()-1)
                    next_polygon_index = _material_indices.keys()[current_material_index+1];
                else
                    next_polygon_index = _polygons.size();

                number_of_polygons = next_polygon_index - current_polygon_index;
                polygons = &(_polygons.constData()[current_polygon_index]);

                glBindTexture(GL_TEXTURE_2D,texture);
//                glActiveTexture(GL_TEXTURE0);
                drawElements(polygons,number_of_polygons,current_material_index);
            }
            current_material_index++;
        }
    } else {
        polygons = _polygons.constData();
        drawElements(polygons,_polygons.size(),0);
    }

    disableClientStates();
}



Point3df Mesh::calculateNormal(Point3df u, Point3df v, Point3df w){
    Point3df normal;
    normal.x((u.y() * v.z()) - (u.z() * v.y()));
    normal.y((u.z() * v.x()) - (u.x() * v.z()));
    normal.z((u.x() * v.y()) - (u.y() * v.x()));
    return normal;
}

void Mesh::addPolygon(const Vertex& a,const Vertex& b,const Vertex& c){
    int size = _vertices.size();
    _vertices.append(a);
    _vertices.append(b);
    _vertices.append(c);
    for (int i = 0; i < 3; ++i) {
        _polygons.append(size++);
    }
}

void Mesh::addPolygon(int index_a, int index_b, int index_c){
        _polygons.append(index_a);
        _polygons.append(index_b);
        _polygons.append(index_c);
}


