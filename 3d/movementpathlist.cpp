#include "movementpathlist.h"

MovementPathList::MovementPathList()
{
}

MovementPathList::~MovementPathList(){
    QList<MovementPath*>::iterator it = begin();
    MovementPath * path;
    while(it!=end()){
        path=*it;
        delete path;
        it=erase(it);
    }
}

void MovementPathList::computePaths(){
    QList<MovementPath*>::iterator it = begin();
    MovementPath * path;
    while(it!=end()){
        path=*it;
        if(path->is_active()){
            path->nextPosition();
            it++;
        }
        else {
            if (path->get_type()==MovementPath::erase){
                delete(path);
                it=erase(it);
            }
            else {
                it++;
            }
        }
    }
}
