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
#include "bezierpath.h"


//test
BezierPath::BezierPath() :
    MovementPath()
{
    __build();
}


BezierPath::BezierPath(Point3df origin) :
    MovementPath(origin)
{
    __build();
}

void BezierPath::__build(){
    _computed = false;

}

void BezierPath::drawPath(bool partial){
    glLineWidth(1);
    glLineStipple(1,0xFFFF);
    int limit;
    if (partial == true) limit = ( _progression * _bezier_points.size() ) /  _progression_max;
    else limit = _bezier_points.size();
    glBegin(GL_LINE_STRIP);{
        for(int i=0;i<limit;i++){
            glColor3b(0,0,0);
            glVertex3f(_bezier_points[i].x(),_bezier_points[i].y(),_bezier_points[i].z());
        }
    } glEnd();
}

void BezierPath::displayPoints(){
    glPointSize(5);
    glBegin(GL_POINTS); {
        for (int i=0; i<4;i++)
            glVertex3f(_bezier[i].x(),_bezier[i].y(),_bezier[i].z());
     } glEnd();
}

Point3d<float> BezierPath::nextPosition(){
    if(_computed == false) compute(_bezier);
    int point = ( _progression * _bezier_points.size() ) /  _progression_max;
    _position = _bezier_points[point];
    _target->get_transform().set_position(_position);
    drawPath();
    displayPoints();
    updateProgression();
    return _position;
}


void BezierPath::appendPoints(Point3d<float> * p)
{
    _bezier_points.append(Point3d<float>(p[0]));
    _bezier_points.append(Point3d<float>(p[1]));
    _bezier_points.append(Point3d<float>(p[2]));
    _bezier_points.append(Point3d<float>(p[3]));
}

void BezierPath::bezierRecursive (const Point3d<float> b[], int level)
{
        if (level > 0) {
            Point3d<float> left[4], right[4];
                left[0] = b[0];
                left[1].x( (b[0].x() + b[1].x()) / 2 );
                left[1].y( (b[0].y() + b[1].y()) / 2 );
                left[1].z( (b[0].z() + b[1].z()) / 2 );
                left[2].x( (b[0].x() + 2*b[1].x() + b[2].x()) / 4 );
                left[2].y( (b[0].y() + 2*b[1].y() + b[2].y()) / 4 );
                left[2].z( (b[0].z() + 2*b[1].z() + b[2].z()) / 4 );
                left[3].x( (b[0].x() + 3*b[1].x() + 3*b[2].x() + b[3].x()) / 8 );
                left[3].y( (b[0].y() + 3*b[1].y() + 3*b[2].y() + b[3].y()) / 8 );
                left[3].z( (b[0].z() + 3*b[1].z() + 3*b[2].z() + b[3].z()) / 8 );
//                if (level == 1) appendPoints(left);
                right[0] = left[3] ;
                right[1].x( (b[1].x() + 2*b[2].x() + b[3].x()) / 4 );
                right[1].y( (b[1].y() + 2*b[2].y() + b[3].y()) / 4 );
                right[1].z( (b[1].z() + 2*b[2].z() + b[3].z()) / 4 );
                right[2].x( (b[2].x() + b[3].x()) / 2 );
                right[2].y( (b[2].y() + b[3].y()) / 2 );
                right[2].z( (b[2].z() + b[3].z()) / 2 );
                right[3] = b[3];
                if (level == 1) {
                    _bezier_points.append(Point3d<float>(left[0]));
                    _bezier_points.append(Point3d<float>(left[1]));
                    _bezier_points.append(Point3d<float>(left[2]));
                    _bezier_points.append(Point3d<float>(left[3]));
                    _bezier_points.append(Point3d<float>(right[0]));
                    _bezier_points.append(Point3d<float>(right[1]));
                    _bezier_points.append(Point3d<float>(right[2]));
                    _bezier_points.append(Point3d<float>(right[3]));
                } else {
                    bezierRecursive (left, level -1);
                    bezierRecursive (right,level -1);
                }
        }
}

void BezierPath::compute(Point3d<float> * b, int level)
{
    qDebug()<<"points: "<<b[0].x()<<" "<<b[0].y();
    qDebug()<<"points: "<<b[1].x()<<" "<<b[1].y();
    qDebug()<<"points: "<<b[2].x()<<" "<<b[2].y();
    qDebug()<<"points: "<<b[3].x()<<" "<<b[3].y();
    qDebug()<<"before";
//    display();
    bezierRecursive(b,level);
//    qDebug()<<"after";
//    display();
    //      qDebug()<<"cleaning up";
    deleteDoubles();
//    display();
    _computed=true;
}
