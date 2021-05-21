#include "Queue.h"

Queue_Node::Queue_Node(T msg){
    this->message = msg;
    this->next = nullptr;
}

