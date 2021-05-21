#ifndef QUEUE_H
#define QUEUE

template <class T>
class Queue_Node{
private:
    T message;
    Queue_Node *next;
public:
    Queue_Node(T msg);
    template <class T> T getMessage();
    ~Queue_Node();
};

template <class T>
class Queue{
private:
    T messages;
    Queue_Node<T> queue;
    Queue_Node<T> *queue_back;
public:
    Queue();
    ~Queue();
    bool empty();
    int size();
    T front();
    T back();
    void push(T msg);
    void pop();
};

#endif