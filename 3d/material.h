#ifndef MATERIAL_H
#define MATERIAL_H

#include <QString>
#include "utils/typedefinitions.h"
#include <qgl.h>

class Material
{
public:
    Material();
    QString _file_name;
    QString _texture_file;
    float _ns;
    Point3df _ka,_ks,_kd;
    float _ni;
    float _d;
    int _illum;
    GLuint _texture_index;


};


#endif // MATERIAL_H
