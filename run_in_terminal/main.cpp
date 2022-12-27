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
            end_x = 9;

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

        if (st_y == end_y) {
            for (i=st_x; i<=end_x; i++) {
                if (tab[st_y][i] != ' ') {
                    cout << "A ship is already there, try a different location\n";
                    repeat = true;
                    break;
                }
            }
            if (repeat) continue;
            for (i=st_x; i<=end_x; i++)
                tab[st_y][i] = 70-length;
        }
        else if (st_x == end_x) {
            for (i=st_y; i<=end_y; i++) {
                if (tab[i][st_x] != ' ') {
                    cout << "A ship is already there, try a different location\n";
                    repeat = true;
                }
            }
            if (repeat) continue;
            for (i=st_y; i<=end_y; i++)
                tab[i][st_x] = 70-length;
        }
    } while (repeat);
    

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
            cout << "My fleet\n\n";
            print_grid(my, NULL, dim);
            cout << "\n\nOpponents\' fleet\n\n";
            print_grid(enemy, NULL, dim);
        }

        void setShip(void) {
            bool repeat = false;
            int st_x, st_y, end_x, end_y;
            int i,j;
            string inp;
            string delimiter = "-";
            string token0, token1;
            
            placeShip(my, "Aircraft Carrier", 5);
            printBoard();
            placeShip(my, "Battleship", 4);
            printBoard();
            placeShip(my, "Cruiser", 3);
            printBoard();
            placeShip(my, "Submarine", 3);
            printBoard();
            placeShip(my, "Destroyer", 2);

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