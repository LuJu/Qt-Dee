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
#include "movementpath.h"

MovementPath::MovementPath()
{
    __build();
}

MovementPath::MovementPath(Point3d<float> origin)
{
    __build();
    _origin=origin;
}

void MovementPath::__build(){
    resetPosition();
    _progression_max=100;
    _target=NULL;
    _activated=false;
    _type=once;
}

void MovementPath::drawPath(bool partial){

}

void MovementPath::displayPoints(){

}


float MovementPath::newPosition(float origin, float target, int progression, int progression_max){
    float value=0.0f;
    float d;
    d=target-origin;
    value = d*((float)progression/(float)progression_max)+origin;
    return value;
}

Point3d<float> MovementPath::nextPosition(){
    _position.x(newPosition(_origin.x(),_target_position.x(), _progression, _progression_max ));
    _position.y(newPosition(_origin.y(),_target_position.y(), _progression, _progression_max ));
    _position.z(newPosition(_origin.z(),_target_position.z(), _progression, _progression_max ));

    if(_target!=NULL){
        _target->get_transform().set_position(_position);
    }

    updateProgression();
    return _position;
}

void MovementPath::updateProgression(){
    _progression++;
    if(_progression==_progression_max) progressionOver();
}

void MovementPath::progressionOver(){
    switch(_type){
    case loop:
        resetPosition();
        break;
    case once:
        _activated=false;
        _target->set_moving(false);
        break;
    case disappear:
        _activated=false;
        _target->set_moving(false);
        _target->set_visible(false);
        break;
    case erase:
        _activated=false;
        _target->set_moving(false);
        break;
    }
}

void MovementPath::resetPosition(){
    _progression=0;
    _position=_origin;
}




