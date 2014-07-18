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
#include "meshutils.h"

void MeshUtils::addHalfSphereZ(Mesh * mesh, float position_z, float radius, bool up){
    float k=0.0;
    float direction = up?radius:-radius;
    while (k<90){
        for (int i = 0; i < 360; i+=5) {
            mesh->set_normal_activated(true);
            Vertex v[3];
            v[0]._point = Point3df(radius * qSin(deg2rad(i)) * qCos(deg2rad(k+5)),
                                   radius * qCos(deg2rad(i)) * qCos(deg2rad(k+5)),
                                   position_z +direction * qSin(deg2rad(k+5))
                                );
            v[0].colorFromPoint();
            v[1]._point = Point3df(radius * qSin(deg2rad(i)) * qCos(deg2rad(k)),
                                   radius * qCos(deg2rad(i)) * qCos(deg2rad(k)),
                                    position_z + direction * qSin(deg2rad(k))
                                );
            v[1].colorFromPoint();
            v[2]._point = Point3df(radius * qSin(deg2rad(i+5)) * qCos(deg2rad(k)),
                                   radius * qCos(deg2rad(i+5)) * qCos(deg2rad(k)),
                                   position_z +direction * qSin(deg2rad(k))
                                );
            v[2].colorFromPoint();
            mesh->addPolygon(v[0],v[1],v[2]);

            v[0]._point = Point3df(radius * qSin(deg2rad(i)) * qCos(deg2rad(k+5)),
                                   radius * qCos(deg2rad(i)) * qCos(deg2rad(k+5)),
                                    position_z + direction * qSin(deg2rad(k+5)))
                                ;
            v[0].colorFromPoint();
            v[1]._point = Point3df(radius * qSin(deg2rad(i+5)) * qCos(deg2rad(k)),
                                   radius * qCos(deg2rad(i+5)) * qCos(deg2rad(k)),
                                position_z + direction * qSin(deg2rad(k))
                                );
            v[1].colorFromPoint();
            v[2]._point = Point3df(radius * qSin(deg2rad(i+5)) * qCos(deg2rad(k+5)),
                                   radius * qCos(deg2rad(i+5)) * qCos(deg2rad(k+5)),
                                position_z + direction * qSin(deg2rad(k+5))
                                );
            v[2].colorFromPoint();
            mesh->addPolygon(v[0],v[1],v[2]);

        }
        k+=5;
    }
}

void MeshUtils::addHalfSphereX(Mesh * mesh, float position_x, float radius, bool up){
    float k=0.0;
    float direction = up?radius:-radius;
    while (k<90){
        for (int i = 0; i < 360; i+=5) {
            mesh->set_normal_activated(true);
            Vertex v[3];
            v[0]._point = Point3df(position_x + direction * qSin(deg2rad(k+5)),
                                   radius * qSin(deg2rad(i)) * qCos(deg2rad(k+5)),
                                   radius * qCos(deg2rad(i)) * qCos(deg2rad(k+5))
                                   );
            v[0].colorFromPoint();
            v[1]._point = Point3df(position_x + direction * qSin(deg2rad(k)),
                                   radius * qSin(deg2rad(i)) * qCos(deg2rad(k)),
                                   radius * qCos(deg2rad(i)) * qCos(deg2rad(k))
                                   );
            v[1].colorFromPoint();
            v[2]._point = Point3df(position_x +direction * qSin(deg2rad(k)),
                                   radius * qSin(deg2rad(i+5)) * qCos(deg2rad(k)),
                                   radius * qCos(deg2rad(i+5)) * qCos(deg2rad(k))
                                   );
            v[2].colorFromPoint();
            mesh->addPolygon(v[0],v[1],v[2]);

            v[0]._point = Point3df(position_x + direction * qSin(deg2rad(k+5)),
                                   radius * qSin(deg2rad(i)) * qCos(deg2rad(k+5)),
                                   radius * qCos(deg2rad(i)) * qCos(deg2rad(k+5))
                                   );
            v[0].colorFromPoint();
            v[1]._point = Point3df(position_x + direction * qSin(deg2rad(k)),
                                   radius * qSin(deg2rad(i+5)) * qCos(deg2rad(k)),
                                   radius * qCos(deg2rad(i+5)) * qCos(deg2rad(k))
                                   );
            v[1].colorFromPoint();
            v[2]._point = Point3df(position_x + direction * qSin(deg2rad(k+5)),
                                   radius * qSin(deg2rad(i+5)) * qCos(deg2rad(k+5)),
                                   radius * qCos(deg2rad(i+5)) * qCos(deg2rad(k+5))
                                   );
            v[2].colorFromPoint();
            mesh->addPolygon(v[0],v[1],v[2]);

        }
        k+=5;
    }
}

void MeshUtils::addHalfSphere(Mesh * mesh, float position_y, float radius, bool up){
    float k=0.0;
    float direction = up?radius:-radius;
    while (k<90){
        for (int i = 0; i < 360; i+=5) {
            mesh->set_normal_activated(true);
            Vertex v[3];
            v[0]._point = Point3df(radius * qSin(deg2rad(i)) * qCos(deg2rad((k+5))),
                                   position_y +direction * qSin(deg2rad(k+5)),
                                   radius * qCos(deg2rad(i)) * qCos(deg2rad((k+5))));
            v[0].colorFromPoint();
            v[1]._point = Point3df(radius * qSin(deg2rad(i)) * qCos(deg2rad(k)),
                                   position_y + direction * qSin(deg2rad(k)),
                                   radius * qCos(deg2rad(i)) * qCos(deg2rad(k)));
            v[1].colorFromPoint();
            v[2]._point = Point3df(radius * qSin(deg2rad(i+5)) * qCos(deg2rad(k)),
                                   position_y +direction * qSin(deg2rad(k)),
                                   radius * qCos(deg2rad(i+5)) * qCos(deg2rad(k)));
            v[2].colorFromPoint();
            mesh->addPolygon(v[0],v[1],v[2]);

            v[0]._point = Point3df(radius * qSin(deg2rad(i)) * qCos(deg2rad(k+5)),
                                   position_y + direction * qSin(deg2rad(k+5)),
                                   radius * qCos(deg2rad(i)) * qCos(deg2rad(k+5)));
            v[0].colorFromPoint();
            v[1]._point = Point3df(radius * qSin(deg2rad(i+5)) * qCos(deg2rad(k)),
                                   position_y + direction * qSin(deg2rad(k)),
                                   radius * qCos(deg2rad(i+5)) * qCos(deg2rad(k)));
            v[1].colorFromPoint();
            v[2]._point = Point3df(radius * qSin(deg2rad(i+5)) * qCos(deg2rad(k+5)),
                                   position_y + direction * qSin(deg2rad(k+5)),
                                   radius * qCos(deg2rad(i+5)) * qCos(deg2rad(k+5)));
            v[2].colorFromPoint();
            mesh->addPolygon(v[0],v[1],v[2]);

        }
        k+=5;
    }
}

void MeshUtils::addHalfSphereY(Mesh * mesh, float position_y, float radius, bool up){
    float k=0.0;
    float direction = up?radius:-radius;
    while (k<90){
        for (int i = 0; i < 360; i+=5) {
            mesh->set_normal_activated(true);
            Vertex v[3];
            v[0]._point = Point3df(radius * qSin(deg2rad(i)) * qCos(deg2rad((k+5))),
                                   position_y +direction * qSin(deg2rad(k+5)),
                                   radius * qCos(deg2rad(i)) * qCos(deg2rad((k+5))));
            v[0].colorFromPoint();
            v[1]._point = Point3df(radius * qSin(deg2rad(i)) * qCos(deg2rad(k)),
                                   position_y + direction * qSin(deg2rad(k)),
                                   radius * qCos(deg2rad(i)) * qCos(deg2rad(k)));
            v[1].colorFromPoint();
            v[2]._point = Point3df(radius * qSin(deg2rad(i+5)) * qCos(deg2rad(k)),
                                   position_y +direction * qSin(deg2rad(k)),
                                   radius * qCos(deg2rad(i+5)) * qCos(deg2rad(k)));
            v[2].colorFromPoint();
            mesh->addPolygon(v[0],v[1],v[2]);

            v[0]._point = Point3df(radius * qSin(deg2rad(i)) * qCos(deg2rad(k+5)),
                                   position_y + direction * qSin(deg2rad(k+5)),
                                   radius * qCos(deg2rad(i)) * qCos(deg2rad(k+5)));
            v[0].colorFromPoint();
            v[1]._point = Point3df(radius * qSin(deg2rad(i+5)) * qCos(deg2rad(k)),
                                   position_y + direction * qSin(deg2rad(k)),
                                   radius * qCos(deg2rad(i+5)) * qCos(deg2rad(k)));
            v[1].colorFromPoint();
            v[2]._point = Point3df(radius * qSin(deg2rad(i+5)) * qCos(deg2rad(k+5)),
                                   position_y + direction * qSin(deg2rad(k+5)),
                                   radius * qCos(deg2rad(i+5)) * qCos(deg2rad(k+5)));
            v[2].colorFromPoint();
            mesh->addPolygon(v[0],v[1],v[2]);

        }
        k+=5;
    }
}


void MeshUtils::addTubeX(Mesh * mesh, float length, float radius){
    for (int i = 0; i < 360; i+=5) {
        Vertex v[3];
        v[0]._point = Point3df(-length,radius * qSin(deg2rad(i)),radius * qCos(deg2rad(i)));
        v[1]._point = Point3df(-length,radius * qSin(deg2rad(i+5)),radius * qCos(deg2rad(i+5)));
        v[2]._point = Point3df(length,radius * qSin(deg2rad(i)),radius * qCos(deg2rad(i)));
        for (int i = 0; i < 3; ++i) {
            v[i].colorFromPoint();
        }
        mesh->addPolygon(v[0],v[1],v[2]);

        v[0]._point = Point3df(length,radius * qSin(deg2rad(i)),radius * qCos(deg2rad(i)));
        v[1]._point = Point3df(length,radius * qSin(deg2rad(i+5)),radius * qCos(deg2rad(i+5)));
        v[2]._point = Point3df(-length,radius * qSin(deg2rad(i+5)),radius * qCos(deg2rad(i+5)));
        for (int i = 0; i < 3; ++i) {
            v[i].colorFromPoint();
        }
        mesh->addPolygon(v[0],v[1],v[2]);
    }
}
void MeshUtils::addTubeZ(Mesh * mesh, float length, float radius){
    for (int i = 0; i < 360; i+=5) {
        Vertex v[3];
        v[0]._point = Point3df(radius * qSin(deg2rad(i)),radius * qCos(deg2rad(i)),-length);
        v[1]._point = Point3df(radius * qSin(deg2rad(i+5)),radius * qCos(deg2rad(i+5)),-length);
        v[2]._point = Point3df(radius * qSin(deg2rad(i)),radius * qCos(deg2rad(i)),length);
        for (int i = 0; i < 3; ++i) {
            v[i].colorFromPoint();
        }
        mesh->addPolygon(v[0],v[1],v[2]);

        v[0]._point = Point3df(radius * qSin(deg2rad(i)),radius * qCos(deg2rad(i)),length);
        v[1]._point = Point3df(radius * qSin(deg2rad(i+5)),radius * qCos(deg2rad(i+5)),length);
        v[2]._point = Point3df(radius * qSin(deg2rad(i+5)),radius * qCos(deg2rad(i+5)),-length);
        for (int i = 0; i < 3; ++i) {
            v[i].colorFromPoint();
        }
        mesh->addPolygon(v[0],v[1],v[2]);
    }
}
void MeshUtils::addTube(Mesh * mesh, float length, float radius){
    for (int i = 0; i < 360; i+=5) {
        Vertex v[3];
        v[0]._point = Point3df(radius * qSin(deg2rad(i)),-length,radius * qCos(deg2rad(i)));
        v[1]._point = Point3df(radius * qSin(deg2rad(i+5)),-length,radius * qCos(deg2rad(i+5)));
        v[2]._point = Point3df(radius * qSin(deg2rad(i)),length,radius * qCos(deg2rad(i)));
        for (int i = 0; i < 3; ++i) {
            v[i].colorFromPoint();
        }
        mesh->addPolygon(v[0],v[1],v[2]);

        v[0]._point = Point3df(radius * qSin(deg2rad(i)),length,radius * qCos(deg2rad(i)));
        v[1]._point = Point3df(radius * qSin(deg2rad(i+5)),length,radius * qCos(deg2rad(i+5)));
        v[2]._point = Point3df(radius * qSin(deg2rad(i+5)),-length,radius * qCos(deg2rad(i+5)));
        for (int i = 0; i < 3; ++i) {
            v[i].colorFromPoint();
        }
        mesh->addPolygon(v[0],v[1],v[2]);
    }
}
void MeshUtils::addTubeY(Mesh * mesh, float length, float radius){
    for (int i = 0; i < 360; i+=5) {
        Vertex v[3];
        v[0]._point = Point3df(radius * qSin(deg2rad(i)),-length,radius * qCos(deg2rad(i)));
        v[1]._point = Point3df(radius * qSin(deg2rad(i+5)),-length,radius * qCos(deg2rad(i+5)));
        v[2]._point = Point3df(radius * qSin(deg2rad(i)),length,radius * qCos(deg2rad(i)));
        for (int i = 0; i < 3; ++i) {
            v[i].colorFromPoint();
        }
        mesh->addPolygon(v[0],v[1],v[2]);

        v[0]._point = Point3df(radius * qSin(deg2rad(i)),length,radius * qCos(deg2rad(i)));
        v[1]._point = Point3df(radius * qSin(deg2rad(i+5)),length,radius * qCos(deg2rad(i+5)));
        v[2]._point = Point3df(radius * qSin(deg2rad(i+5)),-length,radius * qCos(deg2rad(i+5)));
        for (int i = 0; i < 3; ++i) {
            v[i].colorFromPoint();
        }
        mesh->addPolygon(v[0],v[1],v[2]);
    }
}

void MeshUtils::addCapsuleShape(Mesh *mesh, float length, float radius) {
    addHalfSphereY(mesh,length/2,radius,true);
    addTubeY(mesh,length/2,radius);
    addHalfSphereY(mesh,-length/2,radius,false);
    mesh->set_color_activated(true);
    mesh->set_normal_activated(false);
    mesh->set_texture_activated(false);
}
void MeshUtils::addCapsuleShapeX(Mesh *mesh, float length, float radius) {
    addHalfSphereX(mesh,length/2,radius,true);
    addTubeX(mesh,length/2,radius);
    addHalfSphereX(mesh,-length/2,radius,false);
    mesh->set_color_activated(true);
    mesh->set_normal_activated(false);
    mesh->set_texture_activated(false);
}
void MeshUtils::addCapsuleShapeY(Mesh *mesh, float length, float radius) {
    addHalfSphereY(mesh,length/2,radius,true);
    addTubeY(mesh,length/2,radius);
    addHalfSphereY(mesh,-length/2,radius,false);
    mesh->set_color_activated(true);
    mesh->set_normal_activated(false);
    mesh->set_texture_activated(false);
}
void MeshUtils::addCapsuleShapeZ(Mesh *mesh, float length, float radius) {
    addHalfSphereZ(mesh,length/2,radius,true);
    addTubeZ(mesh,length/2,radius);
    addHalfSphereZ(mesh,-length/2,radius,false);
    mesh->set_color_activated(true);
    mesh->set_normal_activated(false);
    mesh->set_texture_activated(false);
}

void MeshUtils::addCircle(Mesh *mesh, float position_y, float radius){
    for (int i = 0; i < 360; i+=5) {
        Vertex v[3];
        v[0].set_color(1,1,1,1);
        v[0]._point = Point3df(0,position_y,0);
        v[0].colorFromPoint();
        v[1]._point = Point3df(radius * qSin(deg2rad(i)),position_y,radius * qCos(deg2rad(i)));
        v[1].colorFromPoint();
        v[2]._point = Point3df(radius * qSin(deg2rad(i+5)),position_y,radius * qCos(deg2rad(i+5)));
        v[2].colorFromPoint();
        mesh->addPolygon(v[0],v[1],v[2]);
    }
}

void MeshUtils::render(const Curve& curve,int normalization, const QColor &color, int thickness, bool points){
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
    mesh.set_texture_activated(false);
    mesh.set_color_activated(true);
    mesh.render();
}

void MeshUtils::drawGrid(QRectF bounds, const QColor &color, int thickness, int horizontal_progression ,int vertical_progression){
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

void MeshUtils::render(const BezierPath& path){
    Mesh mesh;
    mesh.set_type(Mesh::line_strip);
    const QList<Point3df>& points = path.get_points();
    for (int i = 0; i < points.size(); ++i) {
        Vertex v(points[i].x(),points[i].y(),0);
        v.set_color(1,1,0,1.0f);
        mesh.get_vertices().append(v);
        mesh.get_polygons().append(i);
    }
    mesh.set_texture_activated(false);
    mesh.set_color_activated(false);
    mesh.render();
}

void MeshUtils::render(const Point3df& point){
    Mesh mesh;
    mesh.set_type(Mesh::points);
    Vertex v(point);
    v.set_color(1,1,0,1.0f);
    mesh.get_vertices().append(v);
    mesh.get_polygons().append(0);

    mesh.set_texture_activated(false);
    mesh.set_color_activated(false);
    glDisable(GL_DEPTH_TEST);
    mesh.render();
    glEnable(GL_DEPTH_TEST);
}


void MeshUtils::addFlatSurface(Mesh * mesh){
    Vertex v[3];
    Point3df color;

    mesh->set_texture_activated(false);
    mesh->set_color_activated(true);
    mesh->set_normal_activated(false);

    for (int i = -10; i < 10; i++) {
        for (int j = -10; j < 10; j++) {
            if ((i+10)%2 == 0 && (j+10)%2==0 || (i+10)%2 == 1 && (j+10)%2==1)
                color = Point3df(0.1,0.1,0.1);
            else color = Point3df(0.9,0.9,0.9);
            float a= i;
            float b= j;
            float l = .1f;

            v[0]._point = Point3df( b*l,0,      a*l);
            v[1]._point = Point3df((b+1) *l ,0, a*l);
            v[2]._point = Point3df((b+1) *l ,0,(a+1)*l);
            for (int k = 0; k < 3; ++k) {
                v[k].set_color(color.x(),color.y(),color.z(),1);
            }
            mesh->addPolygon(v[0],v[1],v[2]);
            v[0]._point = Point3df(b*l,0,a*l);
            v[1]._point = Point3df((b+1)*l, 0,(a+1)*l);
            v[2]._point = Point3df(b*l ,0,(a+1)*l);
            for (int k = 0; k < 3; ++k) {
                v[k].set_color(color.x(),color.y(),color.z(),1);
            }
            mesh->addPolygon(v[0],v[1],v[2]);

        }

    }


}
