#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <Windows.h>

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
                else if (j%2 == 0 && j%4 != 0)
                    cout << my[i/2][j/4]; //Test
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

class game {
    private:
        char** my;
        char ** enemy;
        int dim;
    public:
        void newGame(void) {
            int i, j;
            if (dim>26 || dim < 10)
                dim = 10;
            my = new char*[dim];
            enemy = new char*[dim];
            for (i=0; i<dim; i++) {
                my[i] = new char[dim];
                enemy[i] = new char[dim];
            }
            for (i=0; i<dim; i++) {
                for (j=0; j<dim; j++) {
                    my[i][j] = ' ';
                    enemy[i][j] = ' ';
                }
            }
        }
        void printBoard(void) {
            system("CLS");
            int i,j;
            char ch = 'A';
            cout << "My Fleet\n\n";
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
                        else if (j%2 == 0 && j%4 != 0)
                            cout << my[i/2][j/4]; //Test
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


            ch = 'A';
            cout << "\n\nOpponents\' fleet";
            cout << "\n\n   ";
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
                        else if (j%2 == 0 && j%4 != 0)
                            cout << enemy[i/2][j/4]; //Test
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
        void setShip(void) {
            string inp;
            string delimiter = "-";
            string token0, token1;
            
            cout << "Set the location of the aircraft carrier (5x1): ";
            cin >> inp;
            token0 = inp.substr(0, inp.find(delimiter));
            //cout << "Token0: " << token0 << endl;
            inp.erase(0, 3);
            token1 = inp;
            //cout << "Token1: " << token1 << endl;  

            int st_x, st_y, end_x, end_y;
            st_y = token0[0] -  97;
            st_x = token0[1] - 49;
            end_y = token1[0] - 97;
            end_x = token1[1] - 49;

            //cout << "Start x,y: " << st_x << ", " << st_y << endl;
            //cout << "End x,y: " << end_x << ", " << end_y << endl;
            if (st_y == end_y) {
                for (int i=st_x; i<end_x - st_x + 1; i++) {
                    my[i][st_y] = 'A';
                }
            }
            if (st_x == end_x) {
                for (int i=st_y; i<end_y - st_y + 1; i++) {
                    my[st_x][i] = 'A';
                }
            }
        }
        game(int dm) : dim(dm) {};
        ~game(void) {
            int i;
            for (i=0; i<dim; i++) {
		        delete my[i];
                delete enemy[i];
            }
	        delete[] my;
            delete[] enemy;
        }
};

int main(void) {
    int i,j,dim;
    char **my_fleet, **enemy_fleet;

    cout << "Dimension of table: (Def is 10): ";
    cin >> dim;
    game gm(10);
    gm.newGame();
    gm.printBoard();
    gm.setShip();
    gm.printBoard();

    Sleep(10000);
}