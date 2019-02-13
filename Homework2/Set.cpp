//
//  Set.cpp
//  Project2
//
//  Created by christopher kha on 1/24/19.
//  Copyright © 2019 christopher kha. All rights reserved.
//

#include "Set.h"
#include <iostream>

using namespace std;

struct Set::Node{            // Node class
public:
    ItemType value;
    Node* next;
    Node* prev;
};

///////////////////////////////

// Set functions
Set::Set(){
    m_size = 0;
    m_head = nullptr;
}

Set::~Set(){
    Node* t = m_head;
    while(t != nullptr){
        Node* killMe = t;
        t = t->next;
        delete killMe;
    }
}

Set::Set(const Set& src){
    m_size = 0;
    m_head = nullptr;
    
    Node* t = src.m_head;
    while(t != nullptr){        // traverse through src's linked list and insert Nodes into the new Set with the same values
        insert(t->value);
        t = t->next;
    }
}

Set& Set::operator=(const Set& src){
    if(this == &src)              // if src and this refer to the same Set, return without changing anything
        return *this;
    
    Node* t = m_head;
    while(t != nullptr){        // delete the current linked list
        Node* killMe = t;
        t = t->next;
        delete killMe;
    }
    
    m_size = 0;        // same as copy constructor
    m_head = nullptr;
    
    Node* q = src.m_head;
    while(q != nullptr){
        insert(q->value);
        q = q->next;
    }
    return *this;
}


bool Set::empty() const{
    return (m_head == nullptr);
}

int Set::size() const{
    return m_size;
}

bool Set::insert(const ItemType& value){
    // Case #1: the value is already in the Set
    if(contains(value)){
        return false;
    }
    
    Node* n = m_head;
    // Case #2: Set is empty, add the value as the first Node
    // Case #3: Node belongs in first position
    if(empty() || (n != nullptr && n->value > value)){
        Node* p = new Node;
        p->value = value;
        p->next = m_head;                           // update new Node's pointers
        p->prev = nullptr;
        m_head = p;                                     // update head pointer
        
        if(n != nullptr){                           // if this is the only node in the Set, don't need to
                                                    // update another node
            n->prev = p;                                // update the next Node's prev pointer
        }
        m_size++;
        return true;
    }
    
    // Case #4: insert in middle/end
    while(n->next != nullptr){
        if(n->next->value > value)          // get the position right before where we are inserting
            break;                          // if value is the greatest in the Set, add to the end
        n = n->next;
    }
    
    Node* p = new Node;
    p->value = value;
    
    p->prev = n;                        // set the pointers
    
    p->next = n->next;
    if(n->next != nullptr)
        n->next->prev = p;
    n->next = p;
    
    m_size++;
    return true;
}

bool Set::erase(const ItemType& value){ 
    // Case #1: value is the first Node in the Set
    if(!empty() && m_head->value == value){
        Node* killMe = m_head;
        m_head = killMe->next;
        
        if(killMe->next != nullptr)
            killMe->next->prev = nullptr;
        
        delete killMe;
        m_size--;
        return true;
    }
    
    // Case #2: value is in the middle/end
    Node* n = m_head;
    while(n != nullptr){
        if(n->value == value){
            break;
        }
        n = n->next;
    }
    
    if(n != nullptr){
        Node* killMe = n;
        n->prev->next = n->next;         // update the previous Node's next pointer
        if(n->next != nullptr)           // check if the next is a Node --> if yes, update it's prev pointer
            n->next->prev = n->prev;
        delete killMe;
        m_size--;
        return true;
    }
    return false;
}

bool Set::contains(const ItemType& value) const{
    if(empty())
        return false;
    
    Node* p = m_head;
    while(p != nullptr){            // traverse array, look for a Node equal to value
        if(p->value == value)
            return true;
        
        p = p->next;
    }
    
    return false;
}

bool Set::get(int pos, ItemType& value) const{
    if(pos < 0 || pos >= m_size)    // check that i is a valid position
        return false;
    
    Node* t = m_head;
    for(int j = 0; j < pos; j++){    // traverse list to get to the ith Node
        if(t != nullptr)
            t = t->next;
    }
    if(t != nullptr)
        value = t->value;       // change value
    return true;
}

void Set::swap(Set& other){
    Node* temp = m_head;        // switch head pointers
    m_head = other.m_head;
    other.m_head = temp;
    
    int s = m_size;             // switch sizes
    m_size = other.m_size;
    other.m_size = s;
}

void unite(const Set& s1, const Set& s2, Set& result){
    if(!(&result == &s2)){
        result = s1;                            // make a copy of s1
                                                // if they are aliases, the assignment operator does nothing
        for(int i = 0; i < s2.size(); i++){     // traverse through s2 and add any nonduplicates
            ItemType temp;
            s2.get(i, temp);
            
            result.insert(temp);
        }
    }else{      // if s2 and result are aliases for the same Set, traverse through s1 insead
        for(int i = 0; i < s1.size(); i++){
            ItemType temp;
            s1.get(i, temp);
            
            result.insert(temp);
        }
    }
}

void subtract(const Set& s1, const Set& s2, Set& result){
    if((&result == &s2)){   // if s2 and result are aliases, make a copy of s2 so that when result changes
        Set tempSet = s2;   // the Set we are traversing through doesn't change
        result = s1;
        
        for(int i = 0; i < tempSet.size(); i++){     // traverse through tempSet and erase any duplicates
            ItemType temp;
            tempSet.get(i, temp);
            result.erase(temp);
        }
        return;
    }
    
    result = s1;                // make a copy of s1
                                // doesn't matter if s1 and result are aliases

    for(int i = 0; i < s2.size(); i++){     // traverse through s2 and erase any duplicates
        ItemType temp;
        s2.get(i, temp);
        
        result.erase(temp);
    }
}

////////////////////
//void Set::dump() const{         // prints out every value and index in Set
//    Node* t = m_head;
//    int i = 0;
//
//    while(t != nullptr){
//        cerr << t->value << "\t\t\t\t\t\t" << i << endl;
//        t = t->next;
//        i++;
//    }
//    cerr << m_size << endl;
//    cerr << "___________________________________" << endl;
//
//    Node* s = m_head;
//    int j = 0;
//    ItemType val;
//
//    while(s->next != nullptr){
//        val = s->next->value;
//        s = s->next;
//    }
//    while(s != nullptr){
//        cerr << s->value << "\t\t\t\t\t\t" << j << endl;
//        s = s->prev;
//        j++;
//    }
//    cerr << "\n\n";
//}
