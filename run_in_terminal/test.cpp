#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <Windows.h>
using namespace std;

/*int main(void) {
    string inp;
    string delimiter = "-";
    string token0, token1;
    cin >> inp; 

    while (inp != "") {
        cout << "Set the location of the aircraft carrier (5x1): ";
        cin  >> inp;

        token0 = inp.substr(0, inp.find(delimiter));
        cout << "Token0: " << token0 << endl;
        cout << "Length of token0 = " << token0.length() << endl;
        inp.erase(0, 3);
        token1 = inp;
        cout << "Token1: " << token1 << endl;  
        cout << "Length of token1 = " << token1.length() << endl;

        int st_x, st_y, end_x, end_y;
        st_x = token0[0] -  97;
        st_y = token0[1] - 49;
        end_x = token1[0] - 97;
        end_y = token1[1] - 49;
        if (token1.length() == 3)
            end_y = 9;

        cout << "Start x,y: " << st_x << ", " << st_y << endl;
        cout << "End x,y: " << end_x << ", " << end_y << endl;
    }

    Sleep(100000);
    return 0;
}*/
int max(int a, int b) {
    return a>b;
}

// a1-a4