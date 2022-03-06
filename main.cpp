//============================================================================
// Name         : lab5.cpp
// Author       :
// Version      : 1.0
// Date Created :
// Date Modified:
// Copyright    : All rights are reserved
// Description  : Convert Infix expression to PostFix and then evaluate it
//============================================================================
#include<iostream>
#include<string>
#include<exception>
#include<math.h>
#include<ctype.h>

using namespace std;

bool isOperator(char); //Function which checks if the input char is an operator
int convertOpToInt (char); //Function which converts operators into int so their precdence can be compared
bool isleq(char, char); //Function which compare two operators  and return True if first operator has less or equal predence than the right operator
string infix2postfix(string);   //Function which converts an infix notaiton into a postfix notation (lab4)
float evaluate(string postfix);     //Function which will evaluate a PostfixExpression and return the result
bool isBalanced(string expression); // Funciton which will check for balanced parentheses

//==============Exception Classes===============
class StackEmpty {
private:
    string errMsg;
public:
    StackEmpty(const string& err): errMsg("Stack is empty!") {}
    string getError() {return errMsg;}
};

class StackFull {
private:
    string errMsg;
public:
    StackFull(const string& err): errMsg(err) {}
    string getError() {return errMsg;}
};
//==============CStack Class===============
template <typename E>
class ArrayStack {
    enum {DEF_CAPACITY = 100};
public:
    ArrayStack(int cap = DEF_CAPACITY);
    int size() const;
    bool empty() const;
    const E& top() const;
    void push(const E& e);
    void pop();
private:
    E* S;
    int capacity;
    int t;
};

template <typename E>
ArrayStack<E>::ArrayStack(int cap) {
    S = new E[DEF_CAPACITY];
    capacity = DEF_CAPACITY;
    t= -1;
}

template <typename E>
int ArrayStack<E>::size() const {
    return (t+1);
}

template <typename E>
bool ArrayStack<E>::empty() const {
    return (t<0);
}

template <typename E>
const E& ArrayStack<E>::top() const {
    return S[t];
}

template <typename E>
void ArrayStack<E>::push(const E& e) {
    if (size() == capacity) throw StackFull("Push to full stack");
    S[++t] = e;
}

template <typename E>
void ArrayStack<E>::pop() {
    if (empty()) throw StackEmpty("Pop from empty stack");
    --t;
}

//=============Main funciton ==============
int main()
{

    while(true)
    {
        string infix;                          //Infix expression
        cout<<"Enter an Infix Expression: ";
        cin>>infix;
        try
        {
            if(infix=="exit")
                break;
            
            else if(!isBalanced(infix))
            {
                cout<<"Expression is not Balanced "<<endl;
                continue;
            }

            string postfix = infix2postfix(infix);    //Postfix Expression
            cout<<"The postfix form is: "<<postfix<<endl;
            float ans=evaluate(postfix);            //evaluate the postfix Expresion
            cout<<infix<<"="<<ans<<endl;        //print the final answer
        }
        catch(exception &e)
        {
            cout<<"Exception: "<<e.what()<<endl;
        }
    }

    return EXIT_SUCCESS;
}
//=====================================================================
bool isOperator(char ch)
{
    if( ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch=='^')
        return true;
    else
        return false;

}
//Function which converts operators into int so their precdence can be compared
int convertOpToInt (char ch)
{
    if (ch=='+' || ch=='-') return 1;
    if (ch=='*' || ch=='/') return 2;
    if (ch=='^') return 3;
    return 0;
}
//Helper Function which compare two operators and return True if first operator
//has less or equal predence than the right operator
bool isleq(char opA, char opB)
{
    return (convertOpToInt(opA)<=convertOpToInt(opB));
}

//Funciton which will check for balanced parentheses
bool isBalanced(string expression)
{
    ArrayStack<char> s;
    for (int i=0; i<expression.length(); i++) {
        if (expression[i] == '(') {
            s.push('(');
            continue;
        }
        else if (expression[i] == ')') {
            s.pop();
            continue;
        }
    }
    return (s.empty());
}

// Function which converts an Infix Notaiton to a Postfix notation
string infix2postfix(string infix)
{

    //0. Create a stack of type char to store operators/parenthesis
    ArrayStack<char> s;
    string postfix;
    //1.    Push “(“onto Stack, and add “)” to the end of INFIX.
    s.push('(');
    infix += ")";
    //2. Scan INFIX from left to right and repeat Step 3 to 6 for each element of X
    //until the Stack is empty.3.
        for (int i=0; i<infix.length(); i++) {
        //3. If an operand is found, add it to POSTFIX.
            if(infix[i]>='0' && infix[i]<='9')
            postfix += infix[i];
        //4. If a left parenthesis is found, push it onto Stack.
        else if (infix[i] == '(')
            s.push(infix[i]);
        //5.    If an operator is found, then:
        else if (infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/') {
            //a. Repeatedly pop from Stack and add to POSTFIX each operator (on the top of Stack) which has the same precedence as or higher precedence than operator.
            while(!s.empty() && isleq(infix[i], s.top())) {
                postfix += s.top();
                s.pop();
            }
            //b. Push the operator onto Stack.
            s.push(infix[i]);
        }
        //.6. If a ")" is found, then:
        else if (infix[i] == ')') {
            // until a "(" found.
            while(s.top() != '(') {
                //a. Repeatedly pop from Stack and add to POSTFIX each operator (on the top of Stack)
                postfix += s.top();
                //b. Remove(pop) the "(" from the stack
                s.pop();
            }
            s.pop();
        }
    }
    return postfix;
}

// //Function which will evaluate a PostfixExpression and return the result
float evaluate(string postfix)
{
    //1. Create a stack of type float to store the operands
    ArrayStack<float> s;
    string result;
    //2. Scan the postfix expression from left to right for every element
    for (int i=0; i<postfix.length(); i++) {
        // a. if an operand is found push it to the stack
        if (postfix[i]>='0' && postfix[i]<='9') {
            float f=(float)(postfix[i]-'0');
            s.push(f);
        }
            
        // b. if an operator is found pop 2 elements from the stack,
        // apply the operator on it and push the result back to the stack
        else if (postfix[i] == '+' || postfix[i] == '-' || postfix[i] == '*' || postfix[i] == '/') {
            float first, second;
            first = s.top();
            s.pop();
            second = s.top();
            s.pop();
            if (postfix[i] == '+')
                s.push(first+second);
            else if (postfix[i] == '-')
                s.push(second-first);
            else if (postfix[i] == '*')
                s.push(first*second);
            else if (postfix[i] == '/')
                s.push(second/first);
            continue;
        }
    }
    //3. return the value from the top of the stack (i.e. the final answer)
    return s.top();
}
