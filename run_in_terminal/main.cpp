#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;

void print_grid(char **my, char **enemy, int dim) {
    int i,j;
    char ch = 'A';
    cout << "   ";
    for (i=0; i<dim; i++) {
        cout << "   " << ch;
        ch++;
    }
    cout << endl;
    for (i=0; i<2*dim; i++) {
        if (i%2 == 1)
            cout << setw(3) << (i/2)+1 << ' ';
        else    
            cout << "    ";
        for (j=0; j<4*dim; j++) {
            if (i%2 == 0) {
                if (j%4 == 0)
                    cout << '+';
                else    
                    cout << '-';
            }
            else {
                if (j%4 == 0)
                    cout << '|';
                else    
                    cout << ' ';
            }
            if (i%2 == 0 && j==(4*dim)-1)
                cout << '+';
            else if (i%2 != 0 && j==(4*dim)-1)   
                cout << '|';
        }
        cout << endl;
    }
    cout << "    ";
    for (j=0; j<dim; j++) {
        cout << "+---";
    } 
    cout << '+' << endl;
}

int main(int argc, char *argv[]) {
    int i,j,dim;
    char **my_fleet, **enemy_fleet;
    dim = atoi(argv[1]); //Dimension of the table (default is 10)
    if (dim>26)
        dim = 10;
    my_fleet = new char*[dim];
    enemy_fleet = new char*[dim];
    for (i=0; i<dim; i++) {
        my_fleet[i] = new char[dim];
        enemy_fleet[i] = new char[dim];
    }

    for (i=0; i<dim; i++) {
        for (j=0; j<dim; j++) {
            my_fleet[i][j] = ' ';
        }
    }
    print_grid(my_fleet, enemy_fleet, dim);
}