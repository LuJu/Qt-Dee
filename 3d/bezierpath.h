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
#ifndef BEZIERPATH_H
#define BEZIERPATH_H

#include "movementpath.h"

//! Bezier paths
/*!
        This class is used to compute 4 points Bezier curves. To define the points, use the _bezier[4] array
        \n Status  2 : Implemented for 4 points curvres
    */
class BezierPath : public MovementPath
{
public:
    BezierPath();
    BezierPath(Point3d<float> origin);

    //! Calculates the points of the path
    /*!
        This function calculates all the points of the path and stores themin a qVector.\n
        If the user calls the nextPosition() function while this one hasn't been called yet, it will automatically will
        \n Status  2 : partially implemented (for four points)
        \param  input
        \return output
    */
    void compute(Point3d<float> * b, int level=5);
    virtual Point3d<float> nextPosition();
    Point3df _bezier[4];
    virtual void drawPath(bool partial = true);
    virtual void displayPoints();

protected:
    void appendPoints(Point3d<float> * p);
    void bezierRecursive (Point3d<float> * b, int level);
    QVector<Point3df> _bezier_points;

private:
    void __build();
    bool _computed;
};

#endif // BEZIERPATH_H
