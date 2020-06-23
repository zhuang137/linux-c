#include <stdio.h>
#include <unistd.h>

template <class T>
class Stack {
private:
    T* data;
    int top;
    int maxSize;
    void reSize();

public:

    Stack(int size = 100) {
        if (size <= 0) {
            printf("stack size <=0, bad operation!!!");
            return;
        }
        data = new T[size];
        maxSize = size;
        top = -1;
    }
    ~Stack() {
        delete[] data;
    }

    void clear() {
        top = -1;
    }
    bool empty() {
        return top == -1;
    }
    int size() {
        return top + 1;
    }

    T* pop() {
        if (top == -1) {
            printf("pop failed for empty stack\n");
            return NULL;
        }
        printf("pop success!!!\n");
        return &data[top--]; 
    }
    void push(const T& value) {
        if (++top >= maxSize) {
            printf("push failed for stack full\n");
            return;
        }
        printf("push success!!!\n");
        data[top] = value;
    }

    Stack(const Stack&) = delete;
    void operator=(const Stack&) = delete;
};

typedef struct {
    int value;
} Element;

int main() {
    Stack<Element> stack(10);
    Element xx{100};
    stack.push(xx);
    stack.pop();
    return 0;
}
