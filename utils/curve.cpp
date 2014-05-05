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
#include "utils/curve.h"

Curve::Curve(QString label) :
    _interpolation(linear),
    _bezier(NULL),
    _label(label) {
}

Curve::Curve() :
    _bezier(NULL),
    _interpolation(linear) {
}


Curve::Curve(const Curve& other):
    QMap<float,float>(other),
    _interpolation(other._interpolation),
    _bezier(NULL)
{
    if (other._bezier != NULL){
        _bezier = new Curve(*(other._bezier));
    }
}

Curve& Curve::operator= (Curve other)
{
    QMap<float,float>::operator =(other);
    if(other._bezier != NULL){
        _bezier = new Curve(*(other._bezier));
    } else _bezier = NULL;
    return *this;
}

void Curve::set_interpolation(Interpolation interpolation){
    _interpolation = interpolation;
}


float Curve::get_value(float x) const{
    QMap<float,float>::const_iterator it;
    it = lowerBound(x);

    if (it== end()){
        if (end() == begin())
            return 0.0f;
        else
            return (--it).value();
    }
    if (it.key() == x || it==begin()) {
        return it.value();
    } else {
        QMap<float,float>::const_iterator it2 = it;
        it2--;
        return interpolate(it2.key(),it2.value(),it.key(),it.value(),x);
    }
}

//(y2-y1)/(x2-x1)
float Curve::tangentAt(float x, bool right) const{
    float value = 0.0f;
    QMap<float,float>::const_iterator it;
    QMap<float,float>::const_iterator it2;
    it = lowerBound(x);
    if (_interpolation == bezier){
        if (_bezier == NULL)
            toBezier();
        return _bezier->tangentAt(x,right);
    }


    if (it==end() || (it==begin() && it.key()!=x)){ //not in the definition of the function
        return 0.0f;
    } else if (it==(end()-1)){ // last key
        if (it==begin()){ // only one key
            return 0.0f;
        } else return get_variation((it-1).key(),(it-1).value(),it.key(),it.value());
    } else if (it==begin()) { // first key
        return get_variation(it.key(),it.value(),(it+1).key(),(it+1).value());
    } else if (it.key() == x) { //exactly on  key, returns value depending on right or left inclusion
        if (right)
            return  get_variation(it.key(),it.value(),(it+1).key(),(it+1).value());
        else return get_variation((it-1).key(),(it-1).value(),it.key(),it.value());
    } else { //not exactly on one of the keys
        it2 = it-1;
        return get_variation(it2.key(),it2.value(),it.key(),it.value());
    }
}

Curve Curve::tangentCurve() const{
    Curve c;
    for (int i = 0; i < keys().size(); ++i) {
        float key = keys()[i];
        c.insert(keys()[i],tangentAt(key));
    }
    return c;
}

float Curve::get_variation(float x1,float y1,float x2,float y2) const{
    if (x1 != x2 && abs(x2-x1) > 1e-10){
        qDebug()<<(y2-y1) / (x2-x1);
        return (y2-y1) / (x2-x1);
    }
    else return y1;
}

float Curve::interpolate(float x1,float y1 ,float x2,float y2, float target) const{
    switch (_interpolation){
    case linear:
        return linearInterpolation(x1, y1, x2, y2,  target);
        break;
    case upper:
        return y2;
        break;
    case lower:
        return y1;
        break;
    case closest:
        return (absolute_value(target-x1) < absolute_value(x2-target))?y1:y2;
        break;
    case bezier:
        return bezierInterpolation(target);
    }

}

float Curve::bezierInterpolation (float target)const{
    if (_bezier == NULL)
        toBezier();
    return _bezier->get_value(target);
}

float Curve::linearInterpolation(float x1,float y1,float x2,float y2, float target) const {
    float interval,interval_target,proportion,diff,value_prop,ret;
    if (target < x1 || target > x2){
        return 0;
    } else {
        interval = x2 - x1;
        interval_target = target - x1;
        proportion = interval_target/interval;
        diff = y2 - y1;
        value_prop = proportion * diff;
        ret = y1 + value_prop;
        return ret;
    }
}

void Curve::toBezier() const{

    BezierPath bezier;
    BezierPath temp;
    float x1,x2,y1,y2,x_dist;
    const QList<float>& xkeys = keys();
    _bezier = new Curve();
    if (xkeys.size() == 1){
        _bezier->insert(xkeys[0],value(xkeys[0]));
    }
    for (int i = 0; i < xkeys.size()-1;i++) {
        x1 = xkeys[i];
        x2 = xkeys[i+1];
        y1 = value(x1);
        y2 = value(x2);
        x_dist = (x2-x1)/4;
        qDebug()<<"x1 :"<<x1;
        qDebug()<<"x2 :"<<x2;
        qDebug()<<"y1 :"<<y1;
        qDebug()<<"y2 :"<<y2;
        temp = BezierPath();
        Point3df controls[4];
        Point3df points[4];
        Point3df control_buffer[2];
        controls[0] =  Point3df(x1,y1,0);
        controls[3] =  Point3df(x2,y2,0);
        if (i>0 && i< xkeys.size()-2){
            points[0]= Point3df(xkeys[i-1],value(xkeys[i-1]),0);
            points[1]= controls[0];
            points[2]= controls[3];
            points[3]= Point3df(xkeys[i+2],value(xkeys[i+2]),0);
            calculateAnchorPoints(points[0],points[1],points[2],points[3],.75f,control_buffer);
        } else {
            points[0]= controls[0];
            points[1]= controls[0];
            points[2]= controls[3];
            points[3]= controls[3];
            calculateAnchorPoints(points[0],points[1],points[2],points[3],.75f,control_buffer);

        }
        controls[1] =  control_buffer[0];
        controls[2] =  control_buffer[1];

        temp.setControlPoints(  controls[0],
                                controls[1],
                                controls[2],
                                controls[3]);
        temp.compute(temp._bezier,5);
        bezier.merge(temp);
    }

    QList<Point3df> points = bezier.get_points();
    for (int i = 0; i < points.size(); ++i) {
        _bezier->insert(points[i].x(),points[i].y());
    }
}

void Curve::display()const{
    for (int i = 0; i < keys().size(); ++i) {
        qDebug()<<keys()[i]<<value(keys()[i]);
    }
}
void Curve::displayB()const{
    _bezier->display();
}

void Curve::calculateAnchorPoints(Point3df pnm1,Point3df pn,  Point3df pnp1, Point3df pnp2, float factor, Point3df* out) const{
    Point3df s1a,s1b,s2a,s2b,s2,s1,m;
    s1 = pn + (pn-pnm1);
    s2 = pnp1 + (pnp1-pnp2);
    s1a = pn + (s1-pn) * factor;
    s2a = pnp1 + (s2-pnp1) * factor;
    m = pn + (pnp1-pn) / 2.f;
    s1b = pn + (s1a-pn)/2.f;
    s2b = pnp1 + (s2a-pnp1)/2.f;
    out[0] = m + (s1b-m)/2.f;
    out[1] = m + (s2b-m)/2.f;
}



