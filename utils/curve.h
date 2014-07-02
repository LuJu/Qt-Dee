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
#ifndef CURVE_H
#define CURVE_H

#include <QMap>
#include <QColor>

#include <QDebug>

#include <utils/triplet.h>
#include <utils/maths.h>
#include "3d/bezierpath.h"


//! A Curve defined by a list of values
//! The values are defined as [x,y] where x and y are float numbers.\n
//! The interpolation indicates what kind of interpolation should be done when trying to get a y value where no x value corresponds
class Curve : public QMap<float,float>
{
public:

    enum Interpolation{
        linear,
        upper,
        lower,
        bezier,
        closest
    };

    Curve();
    Curve(QString label);
    Curve(const Curve& other);
    /** Copy Assignment Operator */
    Curve& operator= (Curve other);

    void set_interpolation(Interpolation interpolation);
    float bezierInterpolation(float target);

    QString _label;

    //! Returns the distance between the first and last x value
    float width() const {
        return (end()-1).key()-begin().key();
    }
    //! Returns the distance between the first and last y value
    float height() const {
        return get_max()-get_min();
    }
    
    //! Returns the min of all the values in the curve
    float get_min() const{
        float min=begin().value();
        const_iterator it = begin();
        while (it != end()) {
            if (min > it.value()) min = it.value();
            ++it;
        }
        return min;
    }

    void display()const;
    void displayB()const;

    //! Returns the max of all the values in the curve
    float get_max() const{
        float max=begin().value();
        const_iterator it = begin();
        while (it != end()) {
            if (max < it.value()) max = it.value();
            ++it;
        }
        return max;
    }


    const QString& get_label() const {return _label;}
    void set_label(const QString& label){_label = label;}

    //! returns the value of the curve at the x given in parameter
    /*!
        The value returned depends on the _interpolation type if the x asked for is not in the curve
        \n Status  1 : not implemented
        \param  x : the x targeted
        \return the value at the time targeted (an interpolation might be needed)
    */
    float get_value (float x) const;

    //! returns the tangent of the curve at the x given in parameter
    /*!
        \param  x : the x targeted
        \return the tangent value at the time targeted
    */
    float tangentAt(float x, bool right = true) const;
    //! returns a curve representing the tangent of the curve at every x defined in the curve
    /*!
        \return the tangent curve
    */
    Curve tangentCurve() const;
//    mutable Curve * _bezier;
    Curve smoothen()const ;
    float bezierInterpolation (float target)const;
    void calculateAnchorPoints(Point3df pn, Point3df pnm1, Point3df pnp1, Point3df pnp2, float factor, Point3df* out) const;


protected:
    Interpolation _interpolation;

    //! returns the interpolation of the values depending on the _interpolation property
    /*!
        description
        \n Status  1 : not implemented
        \param  x1 : time for the first value
        \param  y1 : first value
        \param  x2 : time for the second value
        \param  y2 : second value
        \param target : the time targeted for the return value
        \return the value at time target, 0 if the input values are not valid
    */
    float interpolate(float x1,float y1 ,float x2,float y2, float target) const;

    //! returns the linear interpolation of the values
    /*!
        description
        \n Status  1 : not implemented
        \param  time1 : time for the first value
        \param  value1 : first value
        \param  time2 : time for the second value
        \param  value2 : second value
        \param target : the time targeted for the return value
        \return the value at time target, 0 if the input values are not valid
    */
    float linearInterpolation(float time1,float value1,float time2,float value2, float target) const;
    float get_variation(float x1,float y1,float x2,float y2) const;


};


class Curve3d : public Triplet<Curve>
{
public:
    void set_interpolation(Curve::Interpolation interpolation){
        _x.set_interpolation(interpolation);
        _y.set_interpolation(interpolation);
        _z.set_interpolation(interpolation);
    }
};


#endif // CURVE_H
