//
//  eval.cpp
//  Homework2
//
//  Created by christopher kha on 2/4/19.
//  Copyright © 2019 christopher kha. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <stack>
#include "Set.h"
#include <string>
#include <cstring>
#include <cctype>
#include <cassert>

using namespace std;

int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result)
{
    postfix = "";
    stack<char> istack;
    
    for(int i = 0; i < infix.size(); i++){
        char ch = infix[i];
        
        if(isalpha(ch) && islower(ch)){     // check for operand --> must be letter, lowercase, and contained in one Set
            if(!trueValues.contains(ch) && !falseValues.contains(ch)){          // error code 2 --> letter not in either Set
                return 2;
            }else if(trueValues.contains(ch) && falseValues.contains(ch)){       // error code 3 --> letter in both Sets
               return 3;
            }else if(!istack.empty() && isalpha(istack.top())){               // error code 1 --> two letters without operator
                return 1;
            }else{                                                              // valid operand
                istack.push(ch);
                continue;
            }
        }
        
        switch(ch){
            case '(':
                istack.push(ch);
                break;
            case ')':
                while(!istack.empty() && (istack.top() != '(')){
                    postfix += istack.top();
                    istack.pop();
                }
                if(!istack.empty() && istack.top() == '('){
                    istack.pop();                           // pop the '('
                }else{
                    return 1;                               // error code 1 --> no opening '('
                }
                break;
            case '&':
            case '|':
            case '!':
                while(!istack.empty() && istack.top() != '('){
                    if(isalpha(istack.top())){
                        postfix += istack.top();
                        istack.pop();
                    }
                    // check precedence of operators --> pop() if precedence of char <= to operator on top of stack
                    else if(    (ch == '&' && (istack.top() == '&' || istack.top() == '!'))
                            ||  (ch == '!' && (istack.top() == '!'))
                            ||  (ch == '|' && ((istack.top() == '|') || (istack.top() == '&') || (istack.top() == '!')))
                            ){
                        postfix += istack.top();
                        istack.pop();
                    }else{
                        break;
                    }
                }
                istack.push(ch);
                break;
            case ' ':
                continue;
            default:                    // error code 1 --> illegal char
                return 1;
        }
    }
    while(!istack.empty()){
        if(istack.top() == '('){        // error code 1: no closing parentheses
            return 1;
        }
        
        postfix += istack.top();
        istack.pop();
    }
    
    stack<bool> operands;
    for(int i = 0; i < postfix.size(); i++){
        if(isalpha(postfix[i])){            // operand
            operands.push(trueValues.contains(postfix[i]));
        }else{                              // operator
            bool r = false;
            
            bool op2 = operands.top();
            operands.pop();
            
            if(postfix[i] == '&'){
                if(operands.empty())            // check for error
                    return 1;
                
                bool op1 = operands.top();
                operands.pop();
                
                r = op1 && op2;

            }else if(postfix[i] == '|'){
                if(operands.empty())            // check for error
                    return 1;
                
                bool op1 = trueValues.contains(operands.top());
                operands.pop();
                
                r = op1 || op2;
                
            }else if(postfix[i] == '!'){
                r = !op2;
            }
            
            operands.push(r);
        }
    }
    if(operands.size() != 1){
        return 1;
    }
    result = operands.top();
    return 0;
}


int main(){
    string trueChars  = "tywz";
    string falseChars = "fnx";
    Set trues;
    Set falses;
    for (int k = 0; k < trueChars.size(); k++)
        trues.insert(trueChars[k]);
    for (int k = 0; k < falseChars.size(); k++)
        falses.insert(falseChars[k]);
    
    string pf;
    bool answer;
    
    assert(evaluate("x)", trues, falses, pf, answer) == 1);
    assert(evaluate("(x", trues, falses, pf, answer) == 1);
    assert(evaluate("y|", trues, falses, pf, answer) == 1);
    assert(evaluate("n t", trues, falses, pf, answer) == 1);
    assert(evaluate("nt", trues, falses, pf, answer) == 1);
    assert(evaluate("()", trues, falses, pf, answer) == 1);
    assert(evaluate("y(n|y)", trues, falses, pf, answer) == 1);
    assert(evaluate("t(&n)", trues, falses, pf, answer) == 1);
    assert(evaluate("(n&(t|7)", trues, falses, pf, answer) == 1);
    assert(evaluate("", trues, falses, pf, answer) == 1);
    assert(evaluate("f  |  !f & (t&n) ", trues, falses, pf, answer) == 0
           &&  pf == "ff!tn&&|"  &&  !answer);
    assert(evaluate(" x  ", trues, falses, pf, answer) == 0  &&  pf == "x"  &&  !answer);
    trues.insert('x');
    assert(evaluate("((x))", trues, falses, pf, answer) == 3);
    falses.erase('x');
    assert(evaluate("((x))", trues, falses, pf, answer) == 0  &&  pf == "x"  &&  answer);
    trues.erase('w');
    assert(evaluate("w| f", trues, falses, pf, answer) == 2);
    falses.insert('w');
    assert(evaluate("w| f", trues, falses, pf, answer) == 0  &&  pf == "wf|" &&  !answer);
    
    cerr << "All tests passed!" << endl;
    
}

