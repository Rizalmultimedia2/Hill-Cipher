/*
Nama    : Rizal Herliansyah Hidayat
NPM     : 140810190002

Nama    : Indra Kurniawan
NPM     : 140810190014

Nama    : Rian Febriansyah
NPM     : 140810190026

Program Hill Cipher
*/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Untuk mencari mod invers dari suatu bilangan
int modInverse(int a, int m)
{
    a = a % m;
    for (int x = -m; x < m; x++)
        if ((a * x) % m == 1)
            return x;
    return 0;
}

// Untuk mencari kofaktor dari matriks
void getCofactor(vector<vector<int>> &a, vector<vector<int>> &temp, int p, int q, int n)
{
    int i = 0, j = 0;
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            if (row != p && col != q)
            {
                temp[i][j++] = a[row][col];
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

// Untuk mencari determinan dari matriks
int determinant(vector<vector<int>> &a, int n, int N)
{
    int D = 0;
    if (n == 1)
        return a[0][0];
    vector<vector<int>> temp(N, vector<int>(N));
    int sign = 1;
    for (int f = 0; f < n; f++)
    {
        getCofactor(a, temp, 0, f, n);
        D += sign * a[0][f] * determinant(temp, n - 1, N);
        sign = -sign;
    }
    return D;
}

// Untuk mencari adjoin dari matriks
void adjoint(vector<vector<int>> &a, vector<vector<int>> &adj, int N)
{
    if (N == 1)
    {
        adj[0][0] = 1;
        return;
    }
    int sign = 1;
    vector<vector<int>> temp(N, vector<int>(N));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            getCofactor(a, temp, i, j, N);
            sign = ((i + j) % 2 == 0) ? 1 : -1;
            adj[j][i] = (sign) * (determinant(temp, N - 1, N));
        }
    }
}

// Untuk melakukan invers terhadap suatu matriks dan mengembalikan boolean
bool inverse(vector<vector<int>> &a, vector<vector<int>> &inv, int N)
{
    int det = determinant(a, N, N);
    if (det == 0)
    {
        return false;
    }
    int invDet = modInverse(det, 26);
    vector<vector<int>> adj(N, vector<int>(N));
    adjoint(a, adj, N);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            inv[i][j] = (adj[i][j] * invDet) % 26;
    return true;
}

// Untuk melakukan enkripsi dengan hill cipher
string encrypt(string text, int size, vector<vector<int>> key)
{
    string result = "";
    int temp = (size - text.size() % size) % size;

    for (int i = 0; i < temp; i++)
    {
        text += 'x';
    }

    int k = 0;
    while (k < text.size())
    {
        for (int i = 0; i < size; i++)
        {
            int sum = 0;
            int temp = k;

            for (int j = 0; j < size; j++)
            {
                if (isupper(text[temp]))
                {
                    sum += (key[i][j] % 26 * (text[temp++] - 'A') % 26) % 26;
                }
                else
                {
                    sum += (key[i][j] % 26 * (text[temp++] - 'a') % 26) % 26;
                }
                sum = sum % 26;
            }
            result += (sum + 'A');
        }
        k += size;
    }
    return result;
}

// Untuk melakukan dekripsi dengan hill cipher
string decrypt(string ciphertext, vector<vector<int>> a, vector<vector<int>> inv, int n)
{
    if (inverse(a, inv, n))
    {
        int k = 0;
        string dekripsi;
        while (k < ciphertext.size())
        {
            for (int i = 0; i < n; i++)
            {
                int sum = 0;
                int temp = k;
                for (int j = 0; j < n; j++)
                {
                    if (isupper(ciphertext[temp]))
                    {
                        sum += ((inv[i][j] + 26) % 26 * (ciphertext[temp++] - 'A') % 26) % 26;
                    }
                    else
                    {
                        sum += ((inv[i][j] + 26) % 26 * (ciphertext[temp++] - 'a') % 26) % 26;
                    }
                    sum = sum % 26;
                }
                dekripsi += (sum + 'A');
            }
            k += n;
        }

        int f = dekripsi.size() - 1;
        while (dekripsi[f] == 'x')
        {
            f--;
        }

        return dekripsi;
    }
    return "Inverse matriks key tidak ada!";
}

// Untuk menampilkan matriks
void printMatrix(vector<vector<int>> matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << "\n";
    }
}

int main()
{
    string text, ciphertext, plaintext;
    int option, menu, size, count;
    bool loop = true;

    cout << "== HILL CIPHER PROGRAM ==";
    while (loop)
    {
        cout << "\nMasukkan Ukuran Key Matriks: ";
        cin >> size;

        vector<vector<int>> key(size, vector<int>(size));
        vector<vector<int>> adj(size, vector<int>(size));
        vector<vector<int>> inv(size, vector<int>(size));
        vector<vector<int>> c(size, vector<int>(size));
        vector<vector<int>> p(size, vector<int>(size));

        cout << "\n1. Enkripsi dan Dekripsi \n"
             << "2. Cari Kunci \n"
             << "Pilih Menu : ";
        cin >> menu;

        switch (menu)
        {
        case 1:
            cout << "\nMasukkan Key Matriks " << size << "x" << size << ": \n";
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    cout << "key[" << i << "][" << j << "]: ";
                    cin >> key[i][j];
                }
            }

            cin.ignore();
            cout << "\nMasukkan Text: ";
            getline(cin, text);

            ciphertext = encrypt(text, size, key);
            plaintext = decrypt(ciphertext, key, inv, size);

            cout << "\nHasil Enkripsi: " << ciphertext << '\n';
            cout << "Hasil Dekripsi: " << plaintext << '\n';
            break;
        case 2:
            cin.ignore();
            cout << "\nMasukkan Plaintext: ";
            getline(cin, plaintext);

            cout << "Masukkan Ciphertext: ";
            getline(cin, ciphertext);

            count = 0;
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    //Mendapatkan matriks C dari ciphertext
                    if (isupper(ciphertext[count]))
                    {
                        c[j][i] = ciphertext[count] - 'A';
                    }
                    else
                    {
                        c[j][i] = ciphertext[count] - 'a';
                    }

                    //Mendapatkan matriks P dari plaintext
                    if (isupper(plaintext[count]))
                    {
                        p[j][i] = plaintext[count] - 'A';
                    }
                    else
                    {
                        p[j][i] = plaintext[count] - 'a';
                    }
                    count++;
                }
            }

            //Melakukan invers pada matriks P
            inverse(p, inv, size);

            // Mengalikan matriks C dengan matriks P invers
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    key[i][j] = 0;

                    for (int k = 0; k < size; k++)
                        key[i][j] = key[i][j] + (c[i][k] * inv[k][j]);
                    key[i][j] %= 26;

                    if (key[i][j] < 0)
                        key[i][j] = 26 + key[i][j];
                }
            }

            cout << "\nKunci Hill Cipher: \n";
            printMatrix(key, size);
            break;
        default:
            break;
        }

        cout << "Apakah akan diulang? (y/n): ";
        char optionLoop;
        cin >> optionLoop;
        if (optionLoop == 'y' || optionLoop == 'Y')
            loop = true;
        else
            loop = false;
    }
    cout << "\nProgram berakhir, terima kasih!";
    return 0;
}