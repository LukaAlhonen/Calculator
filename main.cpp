#include <iostream>

using namespace std;

int main() {
    int lval, rval, res;
    char op;
    cout << "> ";
    cin>>lval>>op>>rval;
    switch(op) {
        case '*':
            res = lval * rval;
            break;
        case '+':
            res = lval + rval;
            break;
        case '-':
            res = lval - rval;
            break;
        case '/':
            res = lval / rval;
            break;
        default:
            cout << "idk";
    }

    cout << res << endl;

    return 0;
}
