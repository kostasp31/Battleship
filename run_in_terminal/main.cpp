#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <Windows.h>

using namespace std;

void print_grid(char **my, int dim) {
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
    //srand(time(NULL));
    string inp, delimiter = "-", token0, token1;
    int st_x, st_y, end_x, end_y, i;
    bool repeat;
    do {
        repeat = false;
        cout << "Set the location of the " << name << " (" << length << "x1): ";
        cin >> inp;
        token0 = inp.substr(0, inp.find(delimiter));
        //cout << "Token0: " << token0 << endl;
        if (inp.length() == 7)
            inp.erase(0, 4);
        else 
            inp.erase(0,3);
        token1 = inp;
        //cout << "Token1: " << token1 << endl;  

        st_y = token0[0] -  97;
        st_x = token0[1] - 49;
        end_y = token1[0] - 97;
        end_x = token1[1] - 49;

        if (token0.length() == 3)
            st_x = 9;
        if (token1.length() == 3)
            end_x = 9;

        /*cout << "Start x,y: " << st_x << ", " << st_y << endl;
        cout << "End x,y: " << end_x << ", " << end_y << endl;
        Sleep(10000);*/
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
        else {
            cout << "Diagonal ships aren\'t allowed\n";
            repeat = true;
            continue;
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

void placeEnemy(char** enemy, int ship_n, char symbol) {    //Should make this so that ship dont touch each other
    bool repeat = false;
    do {
        int rd, row_col, cell, i;
        repeat = false;
        rd = rand()%2;
        row_col = rand()%10; //0-9
        cell = rand()%(ship_n+1); //0-ship_n
        if (rd == 0) { //Horizontal
            for (i = cell; i<cell+(ship_n); i++) {
                if (enemy[row_col][i] != ' ')
                    repeat = true;
            }
            if (repeat) continue;
            for (i = cell; i<cell+(ship_n); i++)
                enemy[row_col][i] = symbol;                 
        }
        else {  //Vertical
            for (i = cell; i<cell+(ship_n); i++) {
                if (enemy[i][row_col] != ' ')
                    repeat = true;
            }
            if (repeat) continue;
            for (i = cell; i<cell+(ship_n); i++)
                enemy[i][row_col] = symbol;   
        }
    } while (repeat);
}

class game {
    private:
        char** my;
        char** enemy;
        char** i_fire;
        char** enemy_fires;
        int dim;
        bool winner = 0;
        int air_h = 5;
        int battl_h = 4;
        int crui_h = 3;
        int sub_h = 2;
        int dest_h = 2;
    public:
        void newGame(void) {
            int i, j;
            if (dim>26 || dim < 10)
                dim = 10;
            my = new char*[dim];
            enemy = new char*[dim];
            i_fire = new char*[dim];
            enemy_fires = new char*[dim];
            for (i=0; i<dim; i++) {
                my[i] = new char[dim];
                enemy[i] = new char[dim];
                i_fire[i] = new char[dim];
                enemy_fires[i] = new char[dim];
            }
            for (i=0; i<dim; i++) {
                for (j=0; j<dim; j++) {
                    my[i][j] = ' ';
                    enemy[i][j] = ' ';
                    i_fire[i][j] = ' ';
                    enemy_fires[i][j] = ' ';
                }
            }
        }

        void printMyBoard(void) {
            system("CLS");
            cout << "My fleet\n\n";
            print_grid(my, dim);
        }

        void printAllBoards(void) {
            system("CLS");
            cout << "My fleet\n\n";
            print_grid(my, dim);
            /*cout << "\n\nOpponents\' fleet\n\n";
            print_grid(enemy, dim);*/
            cout << "\n\nI_Fire\n\n";
            print_grid(i_fire, dim);
            /*cout << "\n\nEnemy_Fires\n\n";
            print_grid(enemy_fires, dim);*/
        }

        void setShip(void) {
            bool repeat = false;
            int st_x, st_y, end_x, end_y;
            int i,j;
            string inp;
            string delimiter = "-";
            string token0, token1;
            /*printMyBoard();
            placeShip(my, "Aircraft Carrier", 5);
            printMyBoard();
            placeShip(my, "Battleship", 4);
            printMyBoard();
            placeShip(my, "Cruiser", 3);
            printMyBoard();
            placeShip(my, "Submarine", 3);
            printMyBoard();
            placeShip(my, "Destroyer", 2);*/
            placeEnemy(my, 5, 'A');
            placeEnemy(my, 4, 'B');
            placeEnemy(my, 3, 'C');
            placeEnemy(my, 3, 'D');
            placeEnemy(my, 2, 'E');

        }
        void setEnemy(void) {
            srand(time(NULL));
            /*Επιλέγω τυχαία είτε στήλες είτε γραμμές (rd)
            Έπειτα, επιλέγω μια τυχαία στήλη/γραμμή (row_col)
            και ένα τυχαίο κελί αυτής της στήλης/γραμμής
            το οποίο απέχει από το όριο του πίνακα <= το μήκος του πλοίου
            και τοποθετώ το πλοίο από το κελί αυτό και προς την κατεύθυνση 
            (οριζόντια ή κάθετη) που επιλέχθηκε τυχαία στην αρχή*/

            placeEnemy(enemy, 5, 'A');
            placeEnemy(enemy, 4, 'B');
            placeEnemy(enemy, 3, 'C');
            placeEnemy(enemy, 3, 'D');
            placeEnemy(enemy, 2, 'E');
        } 
        void setWinner(void) {
            this->winner = true;
        }

        int fire(int x, int y) {    //returns 0 if succeed, 1 if not
            if (enemy[x][y] != ' ')
                i_fire[x][y] = 'X';
            else 
                i_fire[x][y] = '*';
            return 0;
        }

        int enemy_fire(void) {     //returns 0 if succeed, 1 if not
            srand(time(NULL));
            int x = rand()%10; //0-9
            int y = rand()%10; //0-9
            if (my[x][y] == '*' || my[x][y] == 'X')
                return 1;
            if (my[x][y] != ' ') {
                enemy_fires[x][y] = 'X';
                my[x][y] = 'X';
                return 0;
            }
            else {
                enemy_fires[x][y] = '*';
                my[x][y] = '*';
                return 0;
            }
        }

        bool isaWinner(void)const {
            return this->winner;
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
    game gm(10);
    gm.newGame();
    gm.setShip();
    gm.printAllBoards();
    gm.setEnemy();
    gm.printAllBoards();
    
    string inp;
    int x, y;
    while (!gm.isaWinner()) {
        cin >> inp;
        x = inp[0] - 97;
        y = inp[1] - 49;
        if (inp.length() == 3) y = 9;
        gm.fire(x, y);
        while (gm.enemy_fire()) {};
        gm.printAllBoards();
    }
    Sleep(10000);
}