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
    _colors_activated(false)
{
}

GLuint Mesh::loadTexture(const QString &textureName)
{
    QImage qim_Texture;
    QImage qim_TempTexture;
    GLuint texture_index;
    if(qim_TempTexture.load(textureName)){
        qim_Texture = QGLWidget::convertToGLFormat( qim_TempTexture );
        glGenTextures( 1, &texture_index);
        glBindTexture( GL_TEXTURE_2D, texture_index);
        glTexImage2D( GL_TEXTURE_2D, 0, 3, qim_Texture.width(), qim_Texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qim_Texture.bits() );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        return texture_index;
    } else {
        qCritical()<<"failed to load textue :"<<textureName;
    }
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

        glVertexPointer(3,GL_FLOAT,size,vertices_array);
        if(_normals_activated)
            glNormalPointer(GL_FLOAT,size,normal_array);
        if(_colors_activated)
            glColorPointer(4,GL_FLOAT,size,color_array);
        if(_textures_activated)
            glTexCoordPointer(3,GL_FLOAT,size,textures_array);
    } else {
//        qDebug()<<"Attempting to render empty mesh";
    }
}

void Mesh::drawElements(const unsigned short * polygons, int number_of_polygons) const{
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
                drawElements(polygons,number_of_polygons);
            }
            current_material_index++;
        }
    } else {
        enableClientStates();
        insertArrayValues();
        polygons = _polygons.constData();
        drawElements(polygons,_polygons.size());
    }

    disableClientStates();
}


void Mesh::loadFromFile(QString filepath, int filetype){
    switch (filetype){
    case obj:
        parseOBJ(filepath);
        break;
    default:
        break;
    }
}

void Mesh::parseOBJ(QString filepath){

    QFile file(filepath);
    QTextStream stream(&file);
    QString str;
    QStringList list[3];
    QString type,buffer;
    int current_mtl_index=-1;
    float x,y,z;

    QVector<Point3df> _temp_vertices;
    QVector<Point3df> _temp_normals;
    QVector<Point3df> _temp_textures;

    Point3dus temp_polygon,temp_texture_polygon,temp_normal_polygon;
    QTime load_time;
    load_time.start();
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!stream.atEnd()) {
            stream >> type;
            if(type=="v" || type=="vn") {
                stream >> x >> y >> z;
                if(type=="v"){
                    _temp_vertices.append(Point3df(x,y,z));
                } else if(type=="vn") {
                    _temp_normals.append(Point3df(x,y,z));
                }
            } else if(type=="vt") {
                stream >> x >> y;
                _temp_textures.append(Point3df(x,y,0));
            } else if(type=="f") {
                for (int j = 0; j < 3; ++j) {
                    stream >> buffer;
                    list[j] = buffer.split(QRegExp("[//| ]"),QString::SkipEmptyParts);
                }
                switch (list[0].size()) {
                case 1: // Only vertex ( f v1 v2 v3 )
                    temp_polygon =         Point3dus(list[0][0].toInt(),list[1][0].toInt(),list[2][0].toInt());
                    fillVertice(_temp_vertices,_temp_normals,_temp_textures,temp_polygon,temp_normal_polygon,temp_texture_polygon,false,false);
                    break;
                case 2: // vertex and normals or vertex and textures (f v1/vt1 v2/vt2 v3/vt3 or f v1//vn1 v2//vn2 v3//vn3 )
                    if (buffer.count('/') == 1){
                        temp_polygon =         Point3dus(list[0][0].toInt(),list[1][0].toInt(),list[2][0].toInt());
                        temp_texture_polygon = Point3dus(list[0][1].toInt(),list[1][1].toInt(),list[2][1].toInt());
                        fillVertice(_temp_vertices,_temp_normals,_temp_textures,temp_polygon,temp_normal_polygon,temp_texture_polygon,false,true);
                    } else if (buffer.count('/') == 2){
                        temp_polygon =         Point3dus(list[0][0].toInt(),list[1][0].toInt(),list[2][0].toInt());
                        temp_normal_polygon =  Point3dus(list[0][1].toInt(),list[1][1].toInt(),list[2][1].toInt());
                        fillVertice(_temp_vertices,_temp_normals,_temp_textures,temp_polygon,temp_normal_polygon,temp_texture_polygon,true,false);
                    }
                    break;
                case 3:// vertex and normals and textures
                    temp_polygon =         Point3dus(list[0][0].toInt(),list[1][0].toInt(),list[2][0].toInt());
                    temp_texture_polygon = Point3dus(list[0][1].toInt(),list[1][1].toInt(),list[2][1].toInt());
                    temp_normal_polygon =  Point3dus(list[0][2].toInt(),list[1][2].toInt(),list[2][2].toInt());
                    fillVertice(_temp_vertices,_temp_normals,_temp_textures,temp_polygon,temp_normal_polygon,temp_texture_polygon,true,true);
                    break;
                default:
                    qWarning()<<"Invalid size";
                    break;
                }
            } else if (type == "mtllib"){
                stream >> buffer;
                qDebug()<<"mtl detected : "<<buffer;
                parseMaterials(buffer);
            } else if(type=="usemtl"){
                stream >> buffer;
                current_mtl_index = findMaterialIndex(buffer);
                if (current_mtl_index == -1) qWarning()<<"Impossible to find corresponding material";
                _material_indices.insert(_polygons.size(),current_mtl_index);
            } else if (type=="#"){
//                qDebug()<<"Comment ignored"<<str;
            } else  {
//                qDebug()<<"Ignored line : "<<str;
            }
            str = stream.readLine();
        }
        file.close();
    }
    if (_temp_normals.size()>0) _normals_activated = true;
    if (_temp_textures.size()>0) _textures_activated = true;
    qDebug()<<"loading time: "<<load_time.elapsed();
}

void Mesh::fillVertice(
                  const QVector<Point3df>& _temp_vertices,
                  const QVector<Point3df>& _temp_normals,
                  const QVector<Point3df>& _temp_textures,
                  const Point3dus& temp_polygon,
                  const Point3dus& temp_normal_polygon,
                  const Point3dus& temp_texture_polygon,
                  bool insert_normal, bool insert_texture){


    int index[3];
    Vertex v[3];
    Point3dus polygon = temp_polygon;
    Point3dus normal_polygon = temp_normal_polygon;
    Point3dus texture_polygon = temp_texture_polygon;

    for(int j = 0 ; j< 3; j++){
        v[j]._point=_temp_vertices[polygon[j]-1];
        if (insert_normal)
            v[j]._normal=_temp_normals[normal_polygon[j]-1];
        if (insert_texture)
            v[j]._texture=_temp_textures[texture_polygon[j]-1];

        index[j] = _vertices.indexOf(v[j]);

        if(index[j]==-1){
            _vertices.append(v[j]);
            index[j] = _vertices.size()-1;
        }
    }
    _polygons.append(index[0]);
    _polygons.append(index[1]);
    _polygons.append(index[2]);

}

void Mesh::parseMaterials(const QString& material_path){
    QFile file(":/materials/"+material_path);
    QTextStream stream(&file);
    Material m;
    float value,x,y,z;
    QString buffer;
    bool first = true;
    QString type;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!stream.atEnd()) {
            stream >> type;
            if (type == "#") ; // comment
            else if (type == "newmtl"){
                if (!first)
                    _materials.append(m);
                else first = false;
                m=Material();
                stream >> m._file_name;
            } else if (type == "Ns"){
                stream >> value;
                m._ns=value;
            } else if (type == "Ka"){
                stream >> x >> y >> z;
                m._ka=Point3df(x,y,z);
            } else if (type == "Kd"){
                stream >> x >> y >> z;
                m._kd=Point3df(x,y,z);
            } else if (type == "Ks"){
                stream >> x >> y >> z;
                m._ks=Point3df(x,y,z);
            } else if (type == "Ni"){
                stream >> value;
                m._ni=value;
            } else if (type == "d"){
                stream >> value;
                m._d=value;
            } else if (type == "illum"){
                stream >> value;
                m._illum=value;
            } else if (type == "map_Kd"){
                stream >> buffer;
                m._texture_file=buffer;
                m._texture_index=loadTexture(":/textures/"+m._texture_file);
            }
        }
        _materials.append(m);
    }
}

int Mesh::findMaterialIndex(const QString& name){
    for (int i = 0; i < _materials.size(); ++i) {
        if (_materials.at(i)._file_name == name) return i;
    }
    return -1;
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


