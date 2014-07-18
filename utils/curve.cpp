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
    _interpolation(linear)
{
}

Curve::Curve() :
    _interpolation(linear) {
}


Curve::Curve(const Curve& other):
    QMap<float,float>(other),
    _interpolation(other._interpolation)
{
}

Curve& Curve::operator= (Curve other)
{
    QMap<float,float>::operator =(other);
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
        Curve::const_iterator it1,it2,it3,it0;
        float target = x;
        it1 = lowerBound(target);
        if (it1.key() == target || it1 == begin()) return it1.value(); // on key or before first key
        it1 = it1-1;
        if (it1 != begin())
            it0 = it1-1;
        else it0 = it1;
        if (it1 != end()){
            it2 = it1+1;
            if (it2 != end())
                it3 = it2+1;
            else it3 = it2;
        } else {
            return it1.value();
        }
        float x1,x2,y1,y2;
        x1 = it1.key();
        x2 = it2.key();
        y1 = it1.value();
        y2 = it2.value();
        Point3df controls[4];
        Point3df points[4];
        Point3df control_buffer[2];
        controls[0] =  Point3df(x1,y1,0);
        controls[3] =  Point3df(x2,y2,0);
        points[0]= Point3df(it0.key(),it0.value(),0);
        points[1]= controls[0];
        points[2]= controls[3];
        points[3]= Point3df(it3.key(),it3.value(),0);
        calculateAnchorPoints(points[0],points[1],points[2],points[3],.40f,control_buffer);

        controls[1] =  control_buffer[0];
        controls[2] =  control_buffer[1];
        float t,t1,t2;
        if (target == points[1].x()) t = 1/3;
        else if (target == points[2].x()) t = 2/3;
        else {
            if (controls[0].x() <= target && target < controls[1].x()){
                t1=0.0f;
                t2=1.f/3.f;
                x1 = controls[0].x();
                x2 = controls[1].x();
            } else if (controls[1].x() <= target && target  < controls[2].x()){
                t1=1.f/3.f;
                t2=2.f/3.f;
                x1 = controls[1].x();
                x2 = controls[2].x();
            } else if (controls[2].x() <= target && target  < controls[3].x()){
                t1=2.f/3.f;
                t2=1.0f;
                x1 = controls[2].x();
                x2 = controls[3].x();
            } else {
                qWarning()<<"value asked not found in curve";
            }
        }
        t = t1 + ((target - x1 )/( x2 - x1 )) * (t2-t1);
        float r1,r2;
        r1 = BezierPath::variation(controls[0].y(),controls[1].y(),controls[2].y(),controls[3].y(),t);
        r2 = BezierPath::variation(controls[0].x(),controls[1].x(),controls[2].x(),controls[3].x(),t);
        return r1;
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
//        qDebug()<<(y2-y1) / (x2-x1);
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
    Curve::const_iterator it1,it2,it3,it0;

    it1 = lowerBound(target);
    if (it1.key() == target || it1 == begin()) return it1.value(); // on key or before first key
    it1 = it1-1;
    if (it1 != begin())
        it0 = it1-1;
    else it0 = it1;
    if (it1 != end()){
        it2 = it1+1;
        if ((it2+1) != end())
            it3 = it2+1;
        else it3 = it2;
    } else {
        return it1.value();
    }
    float x1,x2,y1,y2;
    x1 = it1.key();
    x2 = it2.key();
    y1 = it1.value();
    y2 = it2.value();
    Point3df controls[4];
    Point3df points[4];
    Point3df control_buffer[2];
    controls[0] =  Point3df(x1,y1,0);
    controls[3] =  Point3df(x2,y2,0);
    points[0]= Point3df(it0.key(),it0.value(),0);
    points[1]= controls[0];
    points[2]= controls[3];
    points[3]= Point3df(it3.key(),it3.value(),0);
    calculateAnchorPoints(points[0],points[1],points[2],points[3],.40f,control_buffer);

    controls[1] =  control_buffer[0];
    controls[2] =  control_buffer[1];
    x1 = BezierPath::interpolation(controls[0].x(),controls[1].x(),controls[2].x(),controls[3].x(),1.0f/3.0f);
    x2 = BezierPath::interpolation(controls[0].x(),controls[1].x(),controls[2].x(),controls[3].x(),2.0f/3.0f);
    float t,t1,t2;
    if (target == points[1].x()) t = 1/3;
    else if (target == points[2].x()) t = 2/3;
    else {
        if (controls[0].x() <= target && target < x1){
            if (x1 < x2){
                t1=0.0f;
                t2=1.f/3.f;
            } else {
                t1=0.0f;
                t2=2.f/3.f;
            }
        } else if (x1 <= target && target  < x2){
            t1=1.f/3.f;
            t2=2.f/3.f;
        } else if (x2 <= target && target  < controls[3].x()){
            if (x2 > x1){
                t1=2.f/3.f;
                t2=1.0f;
            } else {
                t1=1.f/3.f;
                t2=1.0f;
            }
        } else {
            qWarning()<<"value asked not found in curve";
        }
    }
    x1 = BezierPath::interpolation(controls[0].x(),controls[1].x(),controls[2].x(),controls[3].x(),t1);
    x2 = BezierPath::interpolation(controls[0].x(),controls[1].x(),controls[2].x(),controls[3].x(),t2);

    float r1;
    t = t1 + ((target - x1 )/( x2 - x1 )) * (t2-t1);
    r1 = BezierPath::interpolation(controls[0].y(),controls[1].y(),controls[2].y(),controls[3].y(),t);
    return r1;
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

Curve Curve::smoothen() const{

    BezierPath bezier;
    BezierPath temp;
    float x1,x2,y1,y2;
    const QList<float>& xkeys = keys();
    Curve bezier_curve;
    if (xkeys.size() == 1){
        bezier_curve.insert(xkeys[0],value(xkeys[0]));
    } else {
        for (int i = 0; i < xkeys.size()-1;i++) {
            x1 = xkeys[i];
            x2 = xkeys[i+1];
            y1 = value(x1);
            y2 = value(x2);
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
                calculateAnchorPoints(points[0],points[1],points[2],points[3],.40f,control_buffer);
            } else {
                if (i == 0 && xkeys.size() > 2){
                    points[0]= controls[0];
                    points[1]= controls[0];
                    points[2]= controls[3];
                    points[3]= Point3df(xkeys[i+2],value(xkeys[i+2]),0);
                } else if (i == xkeys.size()-2 && i > 0) {
                    points[0]= Point3df(xkeys[i-1],value(xkeys[i-1]),0);
                    points[1]= controls[0];
                    points[2]= controls[3];
                    points[3]= controls[3];
                } else {
                    points[0]= controls[0];
                    points[1]= controls[0];
                    points[2]= controls[3];
                    points[3]= controls[3];
                }
                calculateAnchorPoints(points[0],points[1],points[2],points[3],.40f,control_buffer);

            }
            controls[1] =  control_buffer[0];
            controls[2] =  control_buffer[1];

            temp.setControlPoints(  controls[0],
                                    controls[1],
                                    controls[2],
                                    controls[3]);
            temp.compute(temp._bezier,7);
            bezier.merge(temp);
        }
    }

    QList<Point3df> points = bezier.get_points();
    for (int i = 0; i < points.size(); ++i) {
        bezier_curve.insert(points[i].x(),points[i].y());
    }
    return bezier_curve;
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



