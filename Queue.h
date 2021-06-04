#ifndef QUEUE_H
#define QUEUE

template <class T>
class Queue_Node{
private:
    T message;
    Queue_Node *next;

public:
    Queue_Node(T msg){
        this->message = msg;
        this->next = nullptr;
    }

    T get_message(){
        return message;
    }

    ~Queue_Node();
};

template <class T>
class Queue{
private:
    T messages;
    Queue_Node<T> queue;
    Queue_Node<T> *queue_back;

public:
    Queue(){
        queue = nullptr;
        queue_back = nullptr;
    }

    ~Queue();
    bool is_empty(){
        return (queue == nullptr);
    }

    void push(T msg);
    void pop();
    int size();
    T front();
    T back();
    
};

#endif