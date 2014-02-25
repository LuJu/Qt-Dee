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

using namespace std;

Mesh::Mesh():
    _type(triangle),
    _normals_activated(true),
    _textures_activated(true)
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
    glEnableClientState(GL_COLOR_ARRAY);
        if(_textures_activated)
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

}
void Mesh::disableClientStates() const{
    glDisableClientState(GL_VERTEX_ARRAY);
    if(_normals_activated)
        glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    if(_textures_activated)
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Mesh::insertArrayValues() const{

    const Vertex * vertices =_vertices.constData();
    int size = sizeof(vertices[0]);
    const Point3df * vertices_array = &(vertices[0]._point);
    const Point3df * normal_array   = &(vertices[0]._normal);
    const float *    color_array    = vertices[0]._color;
    const Point3df * textures_array = &(vertices[0]._texture);

    if(_normals_activated)
        glNormalPointer(GL_FLOAT,size,normal_array);
    glColorPointer(4,GL_FLOAT,size,color_array);
    glVertexPointer(3,GL_FLOAT,size,vertices_array);
    if(_textures_activated)
        glTexCoordPointer(3,GL_FLOAT,size,textures_array);
}

void Mesh::drawTextures(const unsigned short * polygons, int number_of_polygons) const{
    switch(_type){
    case triangle :
        glDrawElements(GL_TRIANGLES,number_of_polygons,GL_UNSIGNED_SHORT,polygons);
        break;
    case lines:
        glDrawElements(GL_LINES,number_of_polygons,GL_UNSIGNED_SHORT,polygons);
        break;
    case line_strip:
        glDrawElements(GL_LINE_STRIP,number_of_polygons,GL_UNSIGNED_SHORT,polygons);
        break;
    case points:
        glDrawElements(GL_POINTS,number_of_polygons,GL_UNSIGNED_SHORT,polygons);
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

    if(_material_indices.size() > 0){
        enableClientStates();
        insertArrayValues();
        while(current_material_index<_material_indices.size()){

            current_polygon_index = _material_indices.keys()[current_material_index];
            if (_material_indices.value(_material_indices.keys()[current_material_index]) == -1) {
                qWarning()<<"Material not found";
            } else {
                current_material = _materials[_material_indices.value(_material_indices.keys()[current_material_index])];
                if ( current_material_index < _material_indices.size()-1)
                    next_polygon_index = _material_indices.keys()[current_material_index+1];
                else
                    next_polygon_index = _polygons.size();

                number_of_polygons = next_polygon_index - current_polygon_index;

                texture = current_material._texture_index;
                polygons = &(_polygons.constData()[current_polygon_index]);

                glBindTexture(GL_TEXTURE_2D,texture);
                drawTextures(polygons,number_of_polygons);
            }
            current_material_index++;
        }
        disableClientStates();
    } else {
        enableClientStates();
        insertArrayValues();
        polygons = _polygons.constData();
        if(_vertices.size() > 0){
            drawTextures(polygons,_polygons.size());
        } else qDebug()<<"Attempting to render empty mesh";
        disableClientStates();
    }
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
                qDebug()<<buffer;
                m._texture_index=loadTexture(":/textures/"+m._texture_file);
            }
        }
        _materials.append(m);
    }
}

int Mesh::findMaterialIndex(const QString& name){
    qDebug()<<name;
    for (int i = 0; i < _materials.size(); ++i) {
        if (_materials.at(i)._file_name == name) return i;
    }
    return -1;
}

void Mesh::loadFromOBJ(QString filepath){

    QFile file(filepath);
    QTextStream stream(&file);
    QString str;
    QStringList list;
    int current_mtl_index=-1;

    QVector<Point3df> _temp_vertices;
    QVector<Point3df> _temp_normals;
    QVector<Point3df> _temp_textures;

    Point3dus temp_polygon,temp_texture_polygon,temp_normal_polygon;
    QTime load_time;
    load_time.start();
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        str = stream.readLine();
        while (!str.isNull()) {
            if (str.startsWith("mtllib")){
                qDebug()<<"mtl detected";
                parseMaterials(str.replace(0,7,""));
            } else if (str.startsWith("usemtl")){
                current_mtl_index = findMaterialIndex(str.replace(0,7,""));
                if (current_mtl_index == -1) qWarning()<<"Impossible to find corresponding material";
                _material_indices.insert(_polygons.size(),current_mtl_index);
            } else if(str[0]=='v') {
                list = str.split(" ",QString::SkipEmptyParts);
                if(str[1]==' ')
                {
                    if (list.size()==4) {
                        _temp_vertices.append(Point3df(list[1].toFloat(),list[2].toFloat(),list[3].toFloat()));
                    } else qWarning()<<"Invalid line"<<str;
                } else if(str[1]=='n') {
                    if (list.size()==4) {
                        _temp_normals.append(Point3df(list[1].toFloat(),list[2].toFloat(),list[3].toFloat()));
                    } else qWarning()<<"Invalid line"<<str;
                } else if(str[1]=='t') {
                    if (list.size()==3) {
                        _temp_textures.append(Point3df(list[1].toFloat(),list[2].toFloat(),0));
                    } else qWarning()<<"Invalid line"<<str;
                }
            } else if (str[0]=='f'){
                list = str.split(QRegExp("[//| ]"),QString::SkipEmptyParts);
                if(str[1]==' '){
                    switch (list.size()) {
                    case 4: // Only polygons
                        temp_polygon = Point3dus(list[1].toInt(),list[2].toInt(),list[4].toInt());
                        _normals_activated = false;
                        _textures_activated = false;
                        fillVertice(_temp_vertices,temp_polygon);
                        break;
                    case 7: // Only polygons and normals
                        temp_polygon = Point3dus(list[1].toInt(),list[3].toInt(),list[5].toInt());
                        temp_normal_polygon = Point3dus(list[2].toInt(),list[4].toInt(),list[6].toInt());
                        _textures_activated = false;
                        fillVertice(_temp_vertices,_temp_normals,temp_polygon,temp_normal_polygon);
                        break;
                    case 10:// Polygons and normals and textures
                        temp_polygon = Point3dus(list[1].toInt(),list[4].toInt(),list[7].toInt());
                        temp_texture_polygon = Point3dus(list[2].toInt(),list[5].toInt(),list[8].toInt());
                        temp_normal_polygon = Point3dus(list[3].toInt(),list[6].toInt(),list[9].toInt());
                        fillVertice(_temp_vertices,_temp_normals,_temp_textures,temp_polygon,temp_normal_polygon,temp_texture_polygon);
                        break;
                    default:
                        qWarning()<<"Invalid line"<<str;
                        break;
                    }
                } else qWarning()<<"Invalid line"<<str;
            } else if (str[0]=='#'){
//                qDebug()<<"Comment ignored"<<str;
            } else  {
//                qDebug()<<"Ignored line : "<<str;
            }
            str = stream.readLine();
        }
        file.close();
    }
    qDebug()<<"loading time: "<<load_time.elapsed();
}



void Mesh::fillVertice(
                  const QVector<Point3df>& _temp_vertices,
                  const QVector<Point3df>& _temp_normals,
                  const QVector<Point3df>& _temp_textures,
                  const Point3dus& temp_polygon,
                  const Point3dus& temp_normal_polygon,
                  const Point3dus& temp_texture_polygon){

    int index[3];
    Vertex v[3];
    Point3dus polygon = temp_polygon;
    Point3dus normal_polygon = temp_normal_polygon;
    Point3dus texture_polygon = temp_texture_polygon;

    for(int j = 0 ; j< 3; j++){
        v[j]._point=_temp_vertices[polygon[j]-1];
        v[j]._normal=_temp_normals[normal_polygon[j]-1];
        v[j]._texture=_temp_textures[texture_polygon[j]-1];

        v[j]._color[0]=v[j]._normal[0];
        v[j]._color[1]=v[j]._normal[1];
        v[j]._color[2]=v[j]._normal[2];
        v[j]._color[3]=0.5;
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

void Mesh::fillVertice(
        const QVector<Point3df>& _temp_vertices,
        const QVector<Point3df>& _temp_normals,
        const Point3dus& temp_polygon,
        const Point3dus& temp_normal_polygon){

    int index[3];
    Vertex v[3];
    Point3dus polygon = temp_polygon;
    Point3dus normal_polygon = temp_normal_polygon;

    for(int j = 0 ; j< 3; j++){
        v[j]._point=_temp_vertices[polygon[j]-1];
        v[j]._normal=_temp_normals[normal_polygon[j]-1];

        v[j]._color[0]=v[j]._normal[0];
        v[j]._color[1]=v[j]._normal[1];
        v[j]._color[2]=v[j]._normal[2];
        v[j]._color[3]=0.5;
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

void Mesh::fillVertice(
        const QVector<Point3df>& _temp_vertices,
        const Point3dus& temp_polygon){

    int index[3];
    Vertex v[3];
    Point3dus polygon = temp_polygon;

    for(int j = 0 ; j< 3; j++){
        v[j]._point=_temp_vertices[polygon[j]-1];

        v[j]._color[0]=v[j]._point[0];
        v[j]._color[1]=v[j]._point[1];
        v[j]._color[2]=v[j]._point[2];
        v[j]._color[3]=0.5;
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




void Mesh::loadFromFile(QString filepath, int filetype){
    switch (filetype){
    case obj:
        loadFromOBJ(filepath);
        break;
    default:
        break;
    }
}

void Mesh::render(const Curve& curve,int normalization, const QColor &color, int thickness, bool points){
    Mesh mesh;
    mesh.set_type(Mesh::line_strip);
    if(points){

        mesh.set_type(Mesh::points);
        glPointSize((GLfloat)thickness);
    }
    QMap<float,float>::const_iterator iterator = curve.begin();
    int i = 0;
    glLineWidth((GLfloat)thickness);
    while (iterator != curve.end()){
        Vertex v(iterator.key(),iterator.value()/normalization,0);
        v._color[0]=color.red();
        v._color[1]=color.green();
        v._color[2]=color.blue();
        mesh.get_vertices().append(v);
        mesh.get_polygons().append(i);
        i++;
        iterator++;
    }
    mesh.render();
}

void Mesh::drawGrid(QRect bounds, const QColor &color, int thickness, int horizontal_progression ,int vertical_progression){
    Mesh mesh;
    float x1=bounds.topLeft().x();
    float y1=bounds.topLeft().y();
    float x2=bounds.bottomRight().x();
    float y2=bounds.bottomRight().y();
    float xmin = qMin(x1,x2);
    float xmax = qMax(x1,x2);
    float ymin = qMin(y1,y2);
    float ymax = qMax(y1,y2);
    mesh.set_type(Mesh::lines);
    int i = 0;
    glLineWidth((GLfloat)thickness);
    while (i < xmax) {
        Vertex v1(i,y1,0);
        Vertex v2(i,y2,0);
        
        v1.set_color((float)color.red()/255,(float)color.green()/255,(float)color.blue()/255,(float)color.alpha()/255);
        v2.set_color((float)color.red()/255,(float)color.green()/255,(float)color.blue()/255,(float)color.alpha()/255);

        mesh.get_vertices().append(v1);
        mesh.get_polygons().append(mesh.get_vertices().size()-1);
        mesh.get_vertices().append(v2);
        mesh.get_polygons().append(mesh.get_vertices().size()-1);
        i+= horizontal_progression;
    }
    i=0;
    while (i > xmin) {
        Vertex v1(i,y1,0);
        Vertex v2(i,y2,0);

        v1.set_color((float)color.red()/255,(float)color.green()/255,(float)color.blue()/255,(float)color.alpha()/255);
        v2.set_color((float)color.red()/255,(float)color.green()/255,(float)color.blue()/255,(float)color.alpha()/255);

        mesh.get_vertices().append(v1);
        mesh.get_polygons().append(mesh.get_vertices().size()-1);
        mesh.get_vertices().append(v2);
        mesh.get_polygons().append(mesh.get_vertices().size()-1);
        i-= vertical_progression;
    }
    i = 0;
    while (i < ymax){
        Vertex v1(x1,i,0);
        Vertex v2(x2,i,0);

        v1.set_color((float)color.red()/255,(float)color.green()/255,(float)color.blue()/255,(float)color.alpha()/255);
        v2.set_color((float)color.red()/255,(float)color.green()/255,(float)color.blue()/255,(float)color.alpha()/255);

        mesh.get_vertices().append(v1);
        mesh.get_polygons().append(mesh.get_vertices().size()-1);
        mesh.get_vertices().append(v2);
        mesh.get_polygons().append(mesh.get_vertices().size()-1);
        i+=vertical_progression;
    }
    i = 0;
    while (i > ymin){
        Vertex v1(x1,i,0);
        Vertex v2(x2,i,0);

        v1.set_color((float)color.red()/255,(float)color.green()/255,(float)color.blue()/255,(float)color.alpha()/255);
        v2.set_color((float)color.red()/255,(float)color.green()/255,(float)color.blue()/255,(float)color.alpha()/255);

        mesh.get_vertices().append(v1);
        mesh.get_polygons().append(mesh.get_vertices().size()-1);
        mesh.get_vertices().append(v2);
        mesh.get_polygons().append(mesh.get_vertices().size()-1);
        i-=vertical_progression;
    }

    mesh.render();
}

void Mesh::addCircle(float position_y, float radius){
    for (int i = 0; i < 360; i+=5) {
        Vertex v;
        v.set_color(1,1,1,1);
        v._point = Point3df(0,position_y,0);
        _vertices.append(v);
        v._point = Point3df(radius * qSin(deg2rad(i)),position_y,radius * qCos(deg2rad(i)));
        _vertices.append(v);
        v._point = Point3df(radius * qSin(deg2rad(i+5)),position_y,radius * qCos(deg2rad(i+5)));
        _vertices.append(v);
        for (int j = 0; j < 3; ++j) {
            _polygons.append(_polygons.size());
        }
    }
}

Point3df Mesh::calculateNormal(Point3df u, Point3df v){
    Point3df normal;
    normal.x((u.y() * v.z()) - (u.z() * v.y()));
    normal.y((u.z() * v.x()) - (u.x() * v.z()));
    normal.z((u.x() * v.y()) - (u.y() * v.x()));
    return normal;
}

Point3df Mesh::addPolygon(Vertex& a,Vertex& b,Vertex& c){
    Point3df normal = calculateNormal(a._point,b._point);
    a._normal = normal;
    b._normal = normal;
    c._normal = normal;
    _vertices.append(a);
    _polygons.append(_polygons.size());
    _vertices.append(b);
    _polygons.append(_polygons.size());
    _vertices.append(c);
    _polygons.append(_polygons.size());
}

void Mesh::addHalfSphere(float position_y, float radius, bool up){
    float k=0.0;
    float direction = up?radius:-radius;
    while (k<90){
        for (int i = 0; i < 360; i+=5) {
            _normals_activated = true;
            Vertex v[3];
            v[0].set_color(radius * qSin(deg2rad(i)) * qCos(deg2rad((k+5))),
                           position_y +direction * qSin(deg2rad(k+5)),
                           radius * qCos(deg2rad(i)) * qCos(deg2rad((k+5))),1 );
            v[1].set_color(radius * qSin(deg2rad(i)) * qCos(deg2rad((k+5))),
                           position_y +direction * qSin(deg2rad(k+5)),
                           radius * qCos(deg2rad(i)) * qCos(deg2rad((k+5))),1 );
            v[2].set_color(radius * qSin(deg2rad(i)) * qCos(deg2rad((k+5))),
                           position_y +direction * qSin(deg2rad(k+5)),
                           radius * qCos(deg2rad(i)) * qCos(deg2rad((k+5))),1 );
            v[0]._point = Point3df(radius * qSin(deg2rad(i)) * qCos(deg2rad((k+5))),
                                position_y +direction * qSin(deg2rad(k+5)),
                                radius * qCos(deg2rad(i)) * qCos(deg2rad((k+5))));
            v[1]._point = Point3df(radius * qSin(deg2rad(i)) * qCos(deg2rad(k)),
                                position_y + direction * qSin(deg2rad(k)),
                                radius * qCos(deg2rad(i)) * qCos(deg2rad(k)));
            v[2]._point = Point3df(radius * qSin(deg2rad(i+5)) * qCos(deg2rad(k)),
                                position_y +direction * qSin(deg2rad(k)),
                                radius * qCos(deg2rad(i+5)) * qCos(deg2rad(k)));
            addPolygon(v[0],v[1],v[2]);

            v[0]._point = Point3df(radius * qSin(deg2rad(i)) * qCos(deg2rad(k+5)),
                                position_y + direction * qSin(deg2rad(k+5)),
                                radius * qCos(deg2rad(i)) * qCos(deg2rad(k+5)));
            v[1]._point = Point3df(radius * qSin(deg2rad(i+5)) * qCos(deg2rad(k)),
                                position_y + direction * qSin(deg2rad(k)),
                                radius * qCos(deg2rad(i+5)) * qCos(deg2rad(k)));
            v[2]._point = Point3df(radius * qSin(deg2rad(i+5)) * qCos(deg2rad(k+5)),
                                position_y + direction * qSin(deg2rad(k+5)),
                                radius * qCos(deg2rad(i+5)) * qCos(deg2rad(k+5)));
            addPolygon(v[0],v[1],v[2]);

        }
        k+=5;
    }
}

void Mesh::addTube(float length, float radius){
    for (int i = 0; i < 360; i+=5) {
        Vertex v[3];
        v[0].set_color(radius * qSin(deg2rad(i)),-length,radius * qCos(deg2rad(i)),1 );
        v[1].set_color(radius * qSin(deg2rad(i)),-length,radius * qCos(deg2rad(i)),1 );
        v[2].set_color(radius * qSin(deg2rad(i)),-length,radius * qCos(deg2rad(i)),1 );
        v[0]._point = Point3df(radius * qSin(deg2rad(i)),-length,radius * qCos(deg2rad(i)));
        v[1]._point = Point3df(radius * qSin(deg2rad(i+5)),-length,radius * qCos(deg2rad(i+5)));
        v[2]._point = Point3df(radius * qSin(deg2rad(i)),length,radius * qCos(deg2rad(i)));
        addPolygon(v[0],v[1],v[2]);

        v[0]._point = Point3df(radius * qSin(deg2rad(i)),length,radius * qCos(deg2rad(i)));
        v[1]._point = Point3df(radius * qSin(deg2rad(i+5)),length,radius * qCos(deg2rad(i+5)));
        v[2]._point = Point3df(radius * qSin(deg2rad(i+5)),-length,radius * qCos(deg2rad(i+5)));
        addPolygon(v[0],v[1],v[2]);

        //        _vertices.append(Vertex(radius * qSin(deg2rad(i)),length,radius * qCos(deg2rad(i))));
        //        _vertices.append(Vertex(radius * qSin(deg2rad(i+1)),length,radius * qCos(deg2rad(i+1))));
//        for (int j = 0; j < 6; ++j) {
//            _polygons.append(_polygons.size());
//        }
    }
}

void Mesh::fromCapsuleShape(float length, float radius) {
    addHalfSphere(length/2,radius,true);
    addTube(length/2,radius);
    addHalfSphere(-length/2,radius,false);
}


