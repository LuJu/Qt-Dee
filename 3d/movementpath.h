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
#ifndef MOVEMENTPATH_H
#define MOVEMENTPATH_H

#include "utils/geometry.h"

#include "graphicobject3d.h"

class MovementPath
{
public:

    //! brief
    /*!
    How the path should behave : \n
    once : the path is done once and stops after that \n
    loop : the path restarts afer the end\n
    erase : the path should be deleted from the path list after the end
    */
    enum type{
        once,
        loop,
        erase,
        disappear,
        goon
    };

    MovementPath();
    MovementPath(Point3d<float> origin);

    //! returns the next position of the path, and sets it to the target of the path if it has one
    /*!
        This function computes the next position of the path. If it has a target, then it changes its current position\n
        \return the next position of the path
    */
    virtual Point3d<float> nextPosition();


    //! Resets the state of the path to its base state
    /*!
    */
    void resetPosition();

    virtual void drawPath(bool partial = true);
    virtual void displayPoints();

    void start(){_activated=true;}
    void stop(){_activated=false;}

    Point3d<float> get_position() const {return _position;}
    void set_position(Point3d<float> position){_position = position;}

    Point3d<float> get_target_position() const {return _target_position;}
    void set_target_position(Point3d<float> target_position){_target_position = target_position;}

    Point3d<float> get_origin() const {return _origin;}
    void set_origin(Point3d<float> origin){_origin = origin;}

    GraphicObject3D * get_target() const {return _target;}
    void set_target(GraphicObject3D * target){_target = target;}

    bool is_active() const {return _activated;}

    int get_type() const {return _type;}
    void set_type(int type){_type = type;}

    int get_progression() const {return _progression;}
    int get_progression_max() const {return _progression_max;}
    void set_progression_max(int progression_max){_progression_max = progression_max;}

    //! Returns true if the path has reached the last state
    bool is_over() const {return _progression_max==_progression;}


protected:
    GraphicObject3D * _target;
    Point3d<float> _position;
    Point3d<float> _target_position;
    Point3d<float> _origin;
    float _speed;
    int _progression;
    int _progression_max;
    int _type;
    bool _activated;
    void progressionOver();
    void updateProgression();
    float newPosition(float origin, float target, int progression, int progression_max);

private:
    void __build();


};

#endif // MOVEMENTPATH_H
