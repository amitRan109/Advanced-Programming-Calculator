#include "scanner.h"
#include "symtab.h"
// globals
Token_stream ts;
Symbol_table st; 

int statement() { // called from calc. return the result of the line so far
	Token t = ts.get();        // get the next token from token stream
	switch (t.kind) {
        case INT:
            return declaration();
        case IF:
            return condition();
        default:
            ts.putback(t);     // put t back into the token stream
            return expression();
	}
}
int condition() {
    int cond = primary(); // condition in parenthesis ()
    int stmt1 = expression(); // if cond is true
    Token t = ts.get(); // else
    if(t.kind != ELSE) throw runtime_error("else expected in condition");
    int stmt2 = expression();
    return cond ? stmt1 : stmt2;
}

int declaration() { // decleration of variable. like "int value; " expected to get the name
    Token t = ts.get();
	if (t.kind != ID) throw runtime_error("name expected in declaration");
	string name = t.name;
    st.declare(name, 0);
	return 0;
}

// + and -
int expression() {
    int left = term();      // read and evaluate a Term
    Token t = ts.get();        // get the next token from token stream

    while (true) {
        switch (t.kind) {
        case '+':
            left += term();    // evaluate Term and add
            t = ts.get();
            break;
        case '-':
            left -= term();    // evaluate Term and subtract
            t = ts.get();
            break;
        case '<':
            left = left < term();
            t = ts.get();
            break;
        case '>':
            left = left > term();
            t = ts.get();
            break;
        case '&':
            left = left && expression();
            t = ts.get();
            break;
        case '|':
            left = left || expression();
            t = ts.get();
            break;
        default:
            ts.putback(t);     // put t back into the token stream
            return left;       // finally: no more + or -: return the answer
        }
    }
}

// * and /
int term() {
    int left = primary();
    Token t = ts.get();
    while (true) {
        switch (t.kind) {
            case '*':
                left *= primary();
                t = ts.get();
                break;
            case '/': {
                int d = primary();
                if (d == 0) throw runtime_error("divide by zero");
                left /= d;
                t = ts.get();
                break;
            }
            default:
                ts.putback(t);
                return left;
        }
    }
}

int primary() {
    Token t = ts.get();
    int result;
    switch (t.kind) {
        case '(': {   // handle '(' expression ')'
            int d = expression();
            t = ts.get();
            if (t.kind != ')') {
                throw runtime_error("')' expected");
            }
                return d;
        }
        case '-':
            return - primary();
        case '+':
            return primary();
        case NUM:
            return t.value;  // return the number value
        case ID: {
            string n = t.name;
            Token next = ts.get();
            if (next.kind == '=') {	// name = expression
                int d = expression();
                st.set(n, d);
                return d; // return the assignment value
            }
            else {
                ts.putback(next);		// not an assignment
                return st.get(t.name).value;  // return the id value
            }
        }
        case '!':
            return !primary();
        default:
            throw runtime_error("primary expected");
    }
}