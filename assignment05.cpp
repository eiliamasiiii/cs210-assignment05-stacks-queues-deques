#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>

// Fixed-capacity arrays: no resizing or hidden container operations.
class ArrayStack {
    static constexpr std::size_t capacity = 4;
    int data[capacity]{};
    std::size_t count = 0;
public:
    bool empty() const { return count == 0; }
    bool push(int value) {
        if (count == capacity) return false;
        data[count++] = value;
        return true;
    }
    bool pop(int& value) {
        if (empty()) return false;
        value = data[--count];
        return true;
    }
};

class LinkedStack {
    struct Node { int value; Node* next; };
    Node* top = nullptr;
public:
    LinkedStack() = default;
    LinkedStack(const LinkedStack&) = delete;
    LinkedStack& operator=(const LinkedStack&) = delete;
    ~LinkedStack() { int value; while (pop(value)) {} }
    bool empty() const { return top == nullptr; }
    bool push(int value) {
        top = new Node{value, top};
        return true;
    }
    bool pop(int& value) {
        if (empty()) return false;
        Node* old = top;
        value = old->value;
        top = old->next;
        delete old;
        return true;
    }
};

class ArrayQueue {
    static constexpr std::size_t capacity = 4;
    int data[capacity]{};
    std::size_t front = 0, count = 0;
public:
    bool empty() const { return count == 0; }
    bool enqueue(int value) {
        if (count == capacity) return false;
        data[(front + count) % capacity] = value;
        ++count;
        return true;
    }
    bool dequeue(int& value) {
        if (empty()) return false;
        value = data[front];
        front = (front + 1) % capacity;
        --count;
        return true;
    }
};

class LinkedQueue {
    struct Node { int value; Node* next; };
    Node* head = nullptr;
    Node* tail = nullptr;
public:
    LinkedQueue() = default;
    LinkedQueue(const LinkedQueue&) = delete;
    LinkedQueue& operator=(const LinkedQueue&) = delete;
    ~LinkedQueue() { int value; while (dequeue(value)) {} }
    bool empty() const { return head == nullptr; }
    bool enqueue(int value) {
        Node* node = new Node{value, nullptr};
        if (tail) tail->next = node;
        else head = node;
        tail = node;
        return true;
    }
    bool dequeue(int& value) {
        if (empty()) return false;
        Node* old = head;
        value = old->value;
        head = old->next;
        if (!head) tail = nullptr;
        delete old;
        return true;
    }
};

class ArrayDeque {
    static constexpr std::size_t capacity = 4;
    int data[capacity]{};
    std::size_t front = 0, count = 0;
public:
    bool empty() const { return count == 0; }
    bool pushFront(int value) {
        if (count == capacity) return false;
        front = (front + capacity - 1) % capacity;
        data[front] = value;
        ++count;
        return true;
    }
    bool pushBack(int value) {
        if (count == capacity) return false;
        data[(front + count) % capacity] = value;
        ++count;
        return true;
    }
    bool popFront(int& value) {
        if (empty()) return false;
        value = data[front];
        front = (front + 1) % capacity;
        --count;
        return true;
    }
    bool popBack(int& value) {
        if (empty()) return false;
        value = data[(front + count - 1) % capacity];
        --count;
        return true;
    }
};

class LinkedDeque {
    struct Node { int value; Node* previous; Node* next; };
    Node* head = nullptr;
    Node* tail = nullptr;
public:
    LinkedDeque() = default;
    LinkedDeque(const LinkedDeque&) = delete;
    LinkedDeque& operator=(const LinkedDeque&) = delete;
    ~LinkedDeque() { int value; while (popFront(value)) {} }
    bool empty() const { return head == nullptr; }
    bool pushFront(int value) {
        Node* node = new Node{value, nullptr, head};
        if (head) head->previous = node;
        else tail = node;
        head = node;
        return true;
    }
    bool pushBack(int value) {
        Node* node = new Node{value, tail, nullptr};
        if (tail) tail->next = node;
        else head = node;
        tail = node;
        return true;
    }
    bool popFront(int& value) {
        if (empty()) return false;
        Node* old = head;
        value = old->value;
        head = old->next;
        if (head) head->previous = nullptr;
        else tail = nullptr;
        delete old;
        return true;
    }
    bool popBack(int& value) {
        if (empty()) return false;
        Node* old = tail;
        value = old->value;
        tail = old->previous;
        if (tail) tail->next = nullptr;
        else head = nullptr;
        delete old;
        return true;
    }
};

int checks = 0;
void check(bool passed, const std::string& operation) {
    if (!passed) throw std::runtime_error(operation);
    ++checks;
    std::cout << "  PASS " << operation << '\n';
}

template <typename T>
void testStack(const std::string& name, bool bounded) {
    std::cout << "\n" << name << '\n';
    T structure;
    int value = -999;
    check(structure.empty(), "initially empty");
    check(!structure.pop(value) && value == -999, "empty removal preserves output");
    check(structure.push(10), "push 10");
    check(structure.push(20), "push 20");
    check(structure.push(30), "push 30");
    check(structure.push(40), "push 40");
    if (bounded) check(!structure.push(99), "full array rejects insertion");
    check(structure.pop(value) && value == 40, "pop -> 40");
    check(structure.push(50), "push 50 (reuse freed space)");
    check(structure.pop(value) && value == 50, "pop -> 50");
    check(structure.pop(value) && value == 30, "pop -> 30");
    check(structure.pop(value) && value == 20, "pop -> 20");
    check(structure.pop(value) && value == 10, "pop -> 10");
    check(structure.empty(), "empty after draining");
    value = -999;
    check(!structure.pop(value) && value == -999, "empty removal again");
    check(structure.push(60), "push 60 after empty");
    check(structure.pop(value) && value == 60, "pop -> 60");
    check(structure.empty(), "final empty state");
    check(structure.push(70), "leave 70 for destructor cleanup");
}

template <typename T>
void testQueue(const std::string& name, bool bounded) {
    std::cout << "\n" << name << '\n';
    T structure;
    int value = -999;
    check(structure.empty(), "initially empty");
    check(!structure.dequeue(value) && value == -999, "empty removal preserves output");
    check(structure.enqueue(10), "enqueue 10");
    check(structure.enqueue(20), "enqueue 20");
    check(structure.enqueue(30), "enqueue 30");
    check(structure.enqueue(40), "enqueue 40");
    if (bounded) check(!structure.enqueue(99), "full array rejects insertion");
    check(structure.dequeue(value) && value == 10, "dequeue -> 10");
    check(structure.enqueue(50), "enqueue 50 (reuse freed space)");
    check(structure.dequeue(value) && value == 20, "dequeue -> 20");
    check(structure.dequeue(value) && value == 30, "dequeue -> 30");
    check(structure.dequeue(value) && value == 40, "dequeue -> 40");
    check(structure.dequeue(value) && value == 50, "dequeue -> 50");
    check(structure.empty(), "empty after draining");
    value = -999;
    check(!structure.dequeue(value) && value == -999, "empty removal again");
    check(structure.enqueue(60), "enqueue 60 after empty");
    check(structure.dequeue(value) && value == 60, "dequeue -> 60");
    check(structure.empty(), "final empty state");
    check(structure.enqueue(70), "leave 70 for destructor cleanup");
}

template <typename T>
void testDeque(const std::string& name, bool bounded) {
    std::cout << "\n" << name << '\n';
    T structure;
    int value = -999;
    check(structure.empty(), "initially empty");
    check(!structure.popFront(value) && value == -999, "empty popFront");
    check(!structure.popBack(value) && value == -999, "empty popBack");
    check(structure.pushBack(20), "pushBack 20");
    check(structure.pushFront(10), "pushFront 10");
    check(structure.pushBack(30), "pushBack 30");
    check(structure.pushFront(5), "pushFront 5");
    if (bounded) {
        check(!structure.pushFront(99), "full array rejects pushFront");
        check(!structure.pushBack(99), "full array rejects pushBack");
    }
    check(structure.popBack(value) && value == 30, "popBack -> 30");
    check(structure.pushBack(40), "pushBack 40 (reuse freed space)");
    check(structure.popFront(value) && value == 5, "popFront -> 5");
    check(structure.popBack(value) && value == 40, "popBack -> 40");
    check(structure.popFront(value) && value == 10, "popFront -> 10");
    check(structure.popBack(value) && value == 20, "popBack singleton -> 20");
    check(structure.empty(), "empty after draining");
    check(structure.pushFront(60), "pushFront 60 after empty");
    check(structure.popFront(value) && value == 60, "popFront singleton -> 60");
    value = -999;
    check(!structure.popFront(value) && !structure.popBack(value)
          && value == -999, "both ends reject empty removal");
    check(structure.pushBack(70), "leave 70 for destructor cleanup");
}

int main() {
    try {
        testStack<ArrayStack>("ARRAY STACK", true);
        testStack<LinkedStack>("LINKED STACK", false);
        testQueue<ArrayQueue>("ARRAY QUEUE", true);
        testQueue<LinkedQueue>("LINKED QUEUE", false);
        testDeque<ArrayDeque>("ARRAY DEQUE", true);
        testDeque<LinkedDeque>("LINKED DEQUE", false);
        std::cout << "\nAll " << checks << " checks PASS\n";
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
