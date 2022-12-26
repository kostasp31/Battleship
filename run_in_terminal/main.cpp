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
        cout << "   " << i+1;
    }
    cout << endl;
    for (i=0; i<2*dim; i++) {
        if (i%2 == 1) {
            cout << setw(3) << ch << ' ';
            ch++;
        }
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

void placeShip(char** tab, string name, int length) {
    string inp, delimiter = "-", token0, token1;
    int st_x, st_y, end_x, end_y, i;
    bool repeat;
    do {
        repeat = false;
        cout << "Set the location of the " << name << " (" << length << "x1): ";
        cin >> inp;
        token0 = inp.substr(0, inp.find(delimiter));
        //cout << "Token0: " << token0 << endl;
        inp.erase(0, 3);
        token1 = inp;
        //cout << "Token1: " << token1 << endl;  

        st_y = token0[0] -  97;
        st_x = token0[1] - 49;
        end_y = token1[0] - 97;
        end_x = token1[1] - 49;

        if (token1.length() == 3)
            end_y = 9;

        //cout << "Start x,y: " << st_x << ", " << st_y << endl;
        //cout << "End x,y: " << end_x << ", " << end_y << endl;
        if (st_x == end_x) {
            if (end_y - st_y != length - 1) {
                cout << name << " is " << length << " sqares long\n";
                repeat = true;
            }
        }
        else if (st_y == end_y) {
            if (end_x - st_x != length - 1) {
                cout << name << " is " << length << " sqares long\n";
                repeat = true;
            }
        }
    } while (repeat);
    

    if (st_y == end_y) {
        for (i=st_x; i<=end_x; i++) {
            if (tab[st_y][i] != ' ') {
                cout << "A ship is already there, try a different location\n";
                continue;
            }
            tab[st_y][i] = 70-length;
        }
    }
    else if (st_x == end_x) {
        for (i=st_y; i<=end_y; i++) {
            if (tab[st_y][i] != ' ') {
                cout << "A ship is already there, try a different location\n";
                continue;
            }
            tab[i][st_x] = 70-length;
        }
    }
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
            print_grid(my, NULL, dim);
            /*int i,j;
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


            ch = 'A';*/
            cout << "\n\nOpponents\' fleet";
            cout << "\n\n   ";
            print_grid(enemy, NULL, dim);
            /*for (i=0; i<dim; i++) {
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
            cout << '+' << endl;*/
        }
        void setShip(void) {
            bool repeat = false;
            int st_x, st_y, end_x, end_y;
            int i,j;
            string inp;
            string delimiter = "-";
            string token0, token1;
            
            placeShip(my, "Aircraft Carrier", 5);
            placeShip(my, "Battleship", 4);
            placeShip(my, "Cruiser", 3);
            placeShip(my, "Submarine", 3);
            placeShip(my, "Destroyer", 2);
            /*do {
                repeat = false;
                cout << "Set the location of the aircraft carrier (5x1): ";
                cin >> inp;
                token0 = inp.substr(0, inp.find(delimiter));
                cout << "Token0: " << token0 << endl;
                inp.erase(0, 3);
                token1 = inp;
                cout << "Token1: " << token1 << endl;  

                st_x = token0[0] -  97;
                st_y = token0[1] - 49;
                end_x = token1[0] - 97;
                end_y = token1[1] - 49;

                cout << "Start x,y: " << st_x << ", " << st_y << endl;
                cout << "End x,y: " << end_x << ", " << end_y << endl;
                if (st_x == end_x) {
                    if (end_y - st_y != 4) {
                        cout << "Aircraft carrier is 5 sqares long\n";
                        repeat = true;
                    }
                }
                else if (st_y == end_y) {
                    if (end_x - st_x != 4) {
                        cout << "Aircraft carrier is 5 sqares long\n";
                        repeat = true;
                    }
                }
            } while (repeat);
            if (st_y == end_y) {
                for (i=st_x; i<=end_x; i++)
                    my[st_y][i] = 'A';
            }
            else if (st_x == end_x) {
                for (i=st_y; i<=end_y; i++)
                    my[i][st_x] = 'A';
            }

            do {
                repeat = false;
                cout << "Set the location of the battleship (4x1): ";
                cin >> inp;
                token0 = inp.substr(0, inp.find(delimiter));
                cout << "Token0: " << token0 << endl;
                inp.erase(0, 3);
                token1 = inp;
                cout << "Token1: " << token1 << endl;  

                st_x = token0[0] -  97;
                st_y = token0[1] - 49;
                end_x = token1[0] - 97;
                end_y = token1[1] - 49;

                cout << "Start x,y: " << st_x << ", " << st_y << endl;
                cout << "End x,y: " << end_x << ", " << end_y << endl;
                if (st_x == end_x) {
                    if (end_y - st_y != 3) {
                        cout << "Battleship is 4 sqares long\n";
                        repeat = true;
                    }
                }
                else if (st_y == end_y) {
                    if (end_x - st_x != 3) {
                        cout << "Battleship is 4 sqares long\n";
                        repeat = true;
                    }
                }
            } while (repeat);


            if (st_y == end_y) {
                for (i=st_x; i<=end_x; i++)
                    my[st_y][i] = 'B';
            }
            else if (st_x == end_x) {
                for (i=st_y; i<=end_y; i++)
                    my[i][st_x] = 'B';
            }

            do {
                repeat = false;
                cout << "Set the location of the cruiser (3x1): ";
                cin >> inp;
                token0 = inp.substr(0, inp.find(delimiter));
                cout << "Token0: " << token0 << endl;
                inp.erase(0, 3);
                token1 = inp;
                cout << "Token1: " << token1 << endl;  

                st_x = token0[0] -  97;
                st_y = token0[1] - 49;
                end_x = token1[0] - 97;
                end_y = token1[1] - 49;

                cout << "Start x,y: " << st_x << ", " << st_y << endl;
                cout << "End x,y: " << end_x << ", " << end_y << endl;
                if (st_x == end_x) {
                    if (end_y - st_y != 2) {
                        cout << "Cruiser is 3 sqares long\n";
                        repeat = true;
                    }
                }
                else if (st_y == end_y) {
                    if (end_x - st_x != 2) {
                        cout << "Cruiser is 3 sqares long\n";
                        repeat = true;
                    }
                }
            } while (repeat);

            int v;
            cin >> v;

            if (st_y == end_y) {
                for (i=st_x; i<=end_x; i++)
                    my[st_y][i] = 'C';
            }
            else if (st_x == end_x) {
                for (i=st_y; i<=end_y; i++)
                    my[i][st_x] = 'C';
            }*/
            int v;
            cin >> v;

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
    //gm.printBoard();
    gm.setShip();
    gm.printBoard();

    Sleep(100000);
}