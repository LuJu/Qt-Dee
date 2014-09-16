#include "objloader.h"

OBJLoader::OBJLoader():
    _mesh(NULL)
{
}

GLuint OBJLoader::loadTexture(const QString &textureName)
{
    QImage qim_Texture;
    QImage qim_TempTexture;
    GLuint texture_index;
    glEnable(GL_TEXTURE_2D);
    if(qim_TempTexture.load(textureName)){
        qim_Texture = QGLWidget::convertToGLFormat( qim_TempTexture );
        glGenTextures( 1, &texture_index);
        glBindTexture( GL_TEXTURE_2D, texture_index);
        glTexImage2D( GL_TEXTURE_2D, 0, 3, qim_Texture.width(), qim_Texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qim_Texture.bits() );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        return texture_index;
    } else {
        qCritical()<<"failed to load texture :"<<textureName;
    }
}


void OBJLoader::parseOBJ(QString filepath){

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

        _mesh = QSharedPointer<Mesh>(new Mesh);
        qDebug()<<"Parsing obj file "<<filepath;
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
                _mesh->_material_indices.insert(_mesh->_polygons.size(),current_mtl_index);
            } else if (type=="#"){
//                qDebug()<<"Comment ignored"<<str;
            } else  {
//                qDebug()<<"Ignored line : "<<str;
            }
            str = stream.readLine();
        }
        file.close();
    }
    if (_temp_normals.size()>0) _mesh->_normals_activated = true;
    if (_temp_textures.size()>0) _mesh->_textures_activated = true;
    qDebug()<<"loading time: "<<load_time.elapsed();
}

void OBJLoader::fillVertice(
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

        index[j] = _mesh->_vertices.indexOf(v[j]);

        if(index[j]==-1){
            _mesh->_vertices.append(v[j]);
            index[j] = _mesh->_vertices.size()-1;
        }
    }
    _mesh->_polygons.append(index[0]);
    _mesh->_polygons.append(index[1]);
    _mesh->_polygons.append(index[2]);

}

void OBJLoader::parseMaterials(const QString& material_path){
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
                    _mesh->_materials.append(m);
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
                qDebug()<<"load texture "<<m._texture_file;
                m._texture_index=loadTexture(":/textures/"+m._texture_file);
            }
        }
        _mesh->_materials.append(m);
    }
}

int OBJLoader::findMaterialIndex(const QString& name){
    for (int i = 0; i < _mesh->_materials.size(); ++i) {
        if (_mesh->_materials.at(i)._file_name == name) return i;
    }
    return -1;
}
