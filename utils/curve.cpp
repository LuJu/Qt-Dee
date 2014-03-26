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

void Curve::set_interpolation(Interpolation interpolation){
    _interpolation = interpolation;
}


float Curve::get_value(float x) const{
    QMap<float,float>::const_iterator it;
    it = lowerBound(x);
    if (it== end()) return (--it).value();
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

    if (it==end() || (it==begin() && it.key()!=x)){ //not in the definition of the function
        return 0.0f;
    } else if (it==(end()-1)){ // last key
        return get_variation((it-1).key(),(it-1).value(),it.key(),it.value());
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
    return (y2-y1) / (x2-x1);
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
    float x1,x2,y1,y2;
    const QList<float>& xkeys = keys();
    for (int i = 0; i < xkeys.size()-1;i++) {
        x1 = xkeys[i];
        x2 = xkeys[i+1];
        y1 = value(x1);
        y2 = value(x2);
        temp = BezierPath();
        temp.setControlPoints(  Point3df(x1,y1,0),
                                Point3df(x1+x1/4,y1,0),
                                Point3df(x2-x2/4,y2,0),
                                Point3df(x2,y2,0));
        temp.compute(temp._bezier,6);
        bezier.merge(temp);
    }
    _bezier = new Curve();
    QList<Point3df> points = bezier.get_points();
    for (int i = 0; i < points.size(); ++i) {
        _bezier->insert(points[i].x(),points[i].y());
    }
}



