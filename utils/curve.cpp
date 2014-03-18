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
    _label(label) {
}

Curve::Curve() :
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
float Curve::get_right_slope(float x) const{
    float value = 0.0f;
    QMap<float,float>::const_iterator it;
    QMap<float,float>::const_iterator it2;
    it = lowerBound(x);
    if (it==end()){
        it2 = it-1;
        return get_slope(it2.key(),it2.value(),it.key(),it.value());
    }
    if (it==begin()) {
        return get_slope(it.key(),it.value(),(it+1).key(),(it+1).value());
    } else if (it.key() == x) {
        return get_slope(it.key(),(it+1).key(),it.value(),(it+1).value());
    } else {
        QMap<float,float>::const_iterator it2 = it;
        it2--;
        return get_slope(it2.key(),it2.value(),it.key(),it.value(),x);
    }

}

float Curve::get_slope(float x1,float y1,float x2,float y2) const{
    if (x1 != x2)
        return (y2-y1) / (x2/x1);
    else return nanf();
}

float Curve::interpolate(float x1,float y1 ,float x2,float y2, float target) const{
    switch (_interpolation){
    case linear:
        return linearInterpolation(time1, value1, time2, value2,  target);
        break;
    case upper:
        return value2;
        break;
    case lower:
        return value1;
        break;
    case closest:
        return (absolute_value(target-time1) < absolute_value(time2-target))?value1:value2;
        break;
    }

}
float Curve::linearInterpolation(float time1,float value1,float time2,float value2, float target) const {
    float interval,interval_target,proportion,diff,value_prop,ret;
    if (target < time1 || target > time2){
        return 0;
    } else {
        interval = time2 - time1;
        interval_target = target - time1;
        proportion = interval_target/interval;
        diff = value2 - value1;
        value_prop = proportion * diff;
        ret = value1 + value_prop;
        return ret;
    }
}





