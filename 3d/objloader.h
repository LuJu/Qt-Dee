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
