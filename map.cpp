#include "map.h"
#include "simulation_setup.h"
    void FileMapLoader::MapGenerator() {
        ifstream fin("matrice.txt");
        fin >> n >> m;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                fin >> map[i][j];
            }
        }
        for (int i = 0; i < n; i++) {
            cout << endl;
            for (int j = 0; j < m; j++) {
                cout << map[i][j] << " ";
            }
        }
    }
    char FileMapLoader::getCelula(int row, int col){
        return map[row][col];
    }
    int FileMapLoader::getRow() {
        return n;
    }
    int FileMapLoader::getCol() {
        return m;
    }
    void ProceduralMapGenerator::FloodFill(int row, int col, int n, int m, char map[50][50]) {
        if (row>=n || col >=m || row < 0 || col <0) {
            //cout << "Am terminat matricea";
            return;
        }
        if (map[row][col] == '#' || vizitat[row][col] == 1)
            return;
        vizitat[row][col] = 1;
        if (map[row][col] == 'S' || map[row][col] == 'D')
            gasit++;
        FloodFill(row+1, col, n, m , map);
        FloodFill(row-1, col, n, m , map);
        FloodFill(row, col+1, n, m , map);
        FloodFill(row, col-1, n, m , map); //FloodFill e luat de pe: https://usaco.guide/silver/flood-fill?lang=cpp.
    }
    void ProceduralMapGenerator::MapGenerator() {
        int probab;
        int k, l;
        int s1, s2;
        int c1, c2;
        int valid = 0;
        if (contor == 0) {
            Simulation simulare;
            simulare.simulation_setup("simulation_setup.txt");
            n = simulare.rows;
            m = simulare.cols;
            statii = simulare.max_stations;
            clienti = simulare.clients_count;
            contor++;
            srand(time(0));    //aceasta parte este in cod pentru ca eu am facut ca MapGenerator sa se apeleze recursiv pana gaseste solutia buna, iar daca nu puneam acest contor, la fiecare apelare trebuiau citite datele.
        }
        int total = statii + clienti;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                map[i][j] = '/'; //am initializat toate celulele matricii cu '/' ca sa pot sa vad mai tarziu unde e gol si unde nu ca sa stiu daca pot sa pun ceva in celula aia. Asta mi-a venit in cap de la laboratoarele de MN.
        k = rand() % n;
        l = rand() % m;
        map[k][l] = 'B'; //am gasit o celula random care sa fie baza noastra.
        for (int i = 0; i < statii; i++) {
            do {
                s1 = rand() % n;
                s2 = rand() % m;
            } while (map[s1][s2] != '/');
            map[s1][s2] = 'S'; //ne punem in celule random si statiile, am ajuns la un do while dupa foarte multe incercari nereusite, atat cu for cat si cu while normal; cred ca m-am complicat mult dar nu am gasit alta solutie.
        }
        for (int i = 0; i < clienti; i++) {
            do {
                c1 = rand() % n;
                c2 = rand() % m;
            } while (map[c1][c2] != '/');
            map[c1][c2] = 'D'; //la fel si pentru clienti
        }
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) {
                probab = rand() % 100; // Aici am observat ca daca lasam doar o sansa de 50-50, harta mea esua teribil extrem de des, asa ca am facut densitatea de probabilitate un pic diferit. Astfel e o sansa de 20% sa avem zid, si 80% sa avem drum.
                if (map[i][j] == '/') {// am vazut ca in C++ exista si o functie in libraria random, dar nu am reusit sa o implementez la mine in cod, mi s-au parut destul de ciudate articolele de pe google care explicau functia. https://cplusplus.com/reference/random/discrete_distribution/
                    if (probab < 20) {
                        map[i][j] = '#';
                    }
                    else
                        map[i][j] ='.';
                }
            }
        gasit = 0;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                vizitat[i][j] = 0; //la fiecare apelare a functiei trebuie sa resetam variabilele globale ale FloodFillului nostru
        FloodFill(k, l, n, m, map);
        if (total == gasit) { //totalul l-am stabilit mai sus, era suma dintre clienti si statii. Practic daca exista drumuri pana la toti clientii si toate statiile harta e valida.
            valid++;
        }
        if (valid > 0) {
            for (int i = 0; i < n; i++) {
                cout << endl;
                for (int j = 0; j < m; j++) {
                    cout << map[i][j] << " ";
                }
            }
        }
        else {
            //cout << "Harta nu putea fi rezolvata"; //acest cout l-am pus doar ca sa ma pot juca cu probabilitatile de mai sus sa vad de cate ori esueaza harta in general.
            MapGenerator();
        }
    }
    char ProceduralMapGenerator::getCelula(int row, int col){
        return map[row][col];
    }
    int ProceduralMapGenerator::getRow(){
        return n;
    }
    int ProceduralMapGenerator::getCol() {
        return m;
    }