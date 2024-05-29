#pragma once

template <typename T>
struct Node
{
    T data;
    Node* next;
    Node(T data) : data(data), next(nullptr) {}
};

template <typename T>
struct Stack
{
    Node<T>* head;
    
    Stack() : head(nullptr) {}
};

//Function of Stack

//is empty
template <typename T>
bool isEmpty(Stack<T>& stack) {
    return stack.head == nullptr;
}

//push_back
template <typename T>
void push(Stack<T>& stack,T data) {
    Node<T>* newNode = new Node<T>(data);
    newNode->next = stack.head;
    stack.head = newNode;
}

//pop
template <typename T>
T pop(Stack<T>& stack) {
    if (isEmpty(stack)) {
        return;
    }
    Node<T>* temp = stack.head;
    T Data = stack.head->data;
    stack.head = stack.head->next;
    delete temp;
    return Data;
}

template <typename T>
T top(Stack<T>& stack) {
    if (isEmpty(stack)) {
        return;
    }
    return stack.head->data;
}

template <typename T>
void destroy(Stack<T>& stack) {
    while (!isEmpty(stack)) {
        pop(stack);
    }
}