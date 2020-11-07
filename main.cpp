#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <clocale>
#include <Windows.h>


using namespace std;
const int MAX = 1000; // MAX - Максимальное количество символов для шифрования
void Print(char *l, int b) {
    for (int i = 0; i < b; i++)
        cout << l[i];
}


using namespace std;

void shifrovanie(char *first, char *late, int a, int b) {
    srand(b);   // Инициализация секретного ключа
    unsigned short Gamma;   // Гамма
    unsigned short Block1, Block2;  // Блоки, которые будут использованы в качестве буфера обмена
    int c;  // c - Количество блоков памяти
    c = a / 1; //  Количество блоков памяти по 1 байт
    for (int i = 1; i <= c; i++) {
        Gamma = rand();// Генерируем гамму с помощью рандома
        memcpy(&Block1, first + (i - 1),1);    //  Копирование блока исходных данных размером 1 байт из исходной строки в буферный блок данных
        Block2 = Block1 ^Gamma; // Наложение гаммы на буферный блок данных, используя операцию поразрядного исключающего "ИЛИ"
        Block1 = Block2 & 128; // Наложение маски, чтобы выделить отдельные биты и сохранить их в отдельном буферном блоке
        Block1 >>= 7; // Выполнениеоперации поразрядного сдвига на 7 бит вправо
        Block2 <<= 1; // Выполнение операции поразрядного сдвига на 1 бита влево
        Block2 |= Block1; // Использование операции поразрядного "ИЛИ", получение поразрядно сдвинутого на 1 бит влево исходного блока с наложенной гаммой
        memcpy(late + (i - 1), &Block2, 1); // Копирование результата из буфера в блок с зашифрованными данными
        Block1 = 0;
        Block2 = 0;
    }
}



//Функция для расшифровывания данных

void deshifrovanie(char *late, char *first, int a, int b) {

    srand(b); // Инициализация секретного ключа
    unsigned short Gamma;   // Гамма
    int c; // c - Количество блоков памяти
    unsigned short Block1, Block2;  // Блоки, которые будут использованы в качестве буфера обмена
    c = a / 1; //  Количество блоков памяти по 1 байт
    for (int i = 1; i <= c; i++) {
        Gamma = rand();// Генерация гаммы с помощью рандома
        memcpy(&Block1, late + (i - 1),1); // Копирование блока зашифрованных данных размером 1 байт из исходной строки в буферный блок данных
        Block2 = Block1 & 1;  // Наложение маски, чтобы выделить отдельные биты и сохранить их в отдельном буферном блоке
        Block2 <<= 7; // Выполнение операции поразрядного сдвига на 7 бит влево
        Block1 >>= 1; // Выполнение операции поразрядного сдвига на 1 бита вправо
        Block1 |= Block2; // Использование операции поразрядного "ИЛИ", получение поразрядно сдвинутого на 1 бит вправо исходного блока зашифрованных данных
        Block2 = Block1 ^ Gamma; // Наложение гаммы, используя операцию поразрядного исключающего "ИЛИ", и получаем блок исходных данных
        memcpy(first + (i - 1), &Block2, 1); // Копирование результата из буфера в блок с расшифрованными данными
        Block1 = 0;
        Block2 = 0;
    }
}



int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    string mode, name_file,s;
    int a;
    int Key, Key1;   // Key, Key1 - Секретный ключ
    char Original[MAX], Encrypt[MAX], Decrypt[MAX]; // Original - Исходный текст, Encrypt - Зашифрованный текст, Decrypt - Расшифрованный текст
    ofstream file;
    ifstream file1;

    cout << "Enter mode(e for encryption/d for decryption) " << endl;
    cin >> mode;
    cout << "Enter name to the file: " << endl;
    cin >> name_file;
    if (mode == "e") {
        cout << "Enter the data you want to encrypt: " << endl;
        cin >> Original; // Ввод исходных данных
        a = strlen(Original); // Длина строки
        cout << "Enter the key:" << endl;
        cin >> Key; // Ввод секретного ключа
        cin.getline(Original, a);
        shifrovanie(Original, Encrypt, a, Key);
        file.open(name_file, ios::binary);
        string str(Encrypt, a);
        file << str;
        file.close();
    } else if (mode == "d") {
        cout << "Enter the key:" << endl;
        cin >> Key1; // Ввод секретного ключа
        cin.ignore();
        file1.open(name_file, ios::binary);
        if (!file1) {
            cout << "File not found" << endl;
            file1.close();
            return 0;
        }
        file1 >> Encrypt;
        cin.get();
        a = strlen(Encrypt); // Длина строки
        deshifrovanie(Encrypt, Decrypt, a, Key1); // Расшифровывание зашифрованных данных
        cout << "Decrypted data:" << endl;
        Print(Decrypt, a);   //Вывод расшифрованных данных
    }
    return 0;
}
