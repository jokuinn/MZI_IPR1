#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "conio.h"
void VivodSimvolov(FILE* stream, int* x);          //Вывод символов
long Pow(int a, int b);                             //Возведение в степень
long FileSize(FILE* stream);                        //Размер файла (в байтах)
int Zapros(FILE** a, FILE** b, FILE** log);         //Выбор режима работы
int NumbOfBloks(FILE* a, long size);                //Определение числа блоков
void ZapolnT(int* x);                               //Заполнение блока T символами из входного файла (fi)
void ZapolnB();                                     //Заполнение блока B
void ZapolnA();                                     //Заполнение блока A
void ZapolnK(int* t);                               //Заполнение блока K
void Sum232();                                      //Сумматор по модулю Pow(2,32)=2^32
void ZapolnN();                                     //Заполнение накопителя
void UzliZamen();                                   //Прохождение заполнителя через узлы замен
void ZapolnN1();                                    //Преобразование результатов функции UzliZamen() в накопитель N1
void Sdvig11();                                     //Сдвиг на 11 бит влево. Результат сдвига - результа функции Фейстеля F(Ai,Ki)
void XOR(int* t);                                   //Ai+1 = Bi XOR F(Ai,Ki)
void SohrBnext(int* t);                             //Bi+1 = Ai
void SkleivanieAB();                                //Склеивание блоков. T=AB
void ViviodTexta(int* x);                           //Преобразование блока T в символы и вывод этих символов в выходной файл (fo)

FILE* log;                                          //Файл лога.
FILE* fi;                                           //Файловая переменная с открытым/расшифрованным текстом
FILE* fo;                                           //Файловая переменная с зашифрованным/расшифрованным текстом
int T[64], c[8], A[32], B[32], C[32], K[32], SUM32[32];   //Блоки для обработки битов
int N[8] = { 0,0,0,0,0,0,0,0 }, N1[32], F[32];            //Вспомогательные блоки (накопители)
unsigned long KEY[8] =                               //Ключ
{
    123456,
    654321,
    162534,
    111111,
    222222,
    333333,
    444444,
    555555
};
unsigned char ch;                                   //Символьная переменная (для обработки символов)
int n;                                              //Вспомогательная переменная
int blokN;                                          //Число блоков
long size;                                          //Размер файла в байтах
bool nekratno = 0;                                    //Логическая переменная: 0 - если число символов кратно 8, 1 - если не кратно
int rejim;                                          //Выбор режима работы (1 - шифрование, 2 -расшифрование)

int TABLE[8][16] =                                   //Таблица замен
{
  4, 10,  9,  2, 13,  8,  0, 14,  6, 11,  1, 12,  7, 15,  5,  3,
 14, 11,  4, 12,  6, 13, 15, 10,  2,  3,  8,  1,  0,  7,  5,  9,
  5,  8,  1, 13, 10,  3,  4,  2, 14, 15, 12,  7,  6,  0,  9, 11,
  7, 13, 10,  1,  0,  8,  9, 15, 14,  4,  6, 12, 11,  2,  5,  3,
  6, 12,  7,  1,  5, 15, 13,  8,  4, 10,  9, 14,  0,  3, 11,  2,
  4, 11, 10,  0,  7,  2,  1, 13,  3,  6,  8,  5,  9, 12, 15, 14,
 13, 11,  4,  1,  3, 15,  5,  9,  0, 10, 14,  7,  6,  8,  2, 12,
  1, 15, 13,  0,  5,  7, 10,  4,  9,  2,  3, 14,  6, 11,  8, 12
};
//************************************************************************************
int main()
{
    rejim = Zapros(&fi, &fo, &log);     //Выбор режима работы (шифрование или расшифрование)
    blokN = NumbOfBloks(fi, size);     //Определение числа блоков

    for (int p = 0; p < blokN; p++)        //Заполнение всех блоков T:
    {
        VivodSimvolov(fi, &p);
        ZapolnT(&p);                //Заполнение (p+1)-ого блока Т
        ZapolnB();                  //Заполнение блока B
        ZapolnA();                  //Заполнение блока A

        for (int t = 1; t <= 32; t++)     //Начало основного цикла:
        {
            fprintf(log, "\n%-2d*************************************", t);
            ZapolnK(&t);            //Заполнение ключа K

            //Функция Фейстеля:
            Sum232();               //1) Сумматор по модулю 2^32
            ZapolnN();              //2) Заполнение накопителя
            UzliZamen();            //3) Проход через узлы замен
            ZapolnN1();             //4) Заполнение выходного накопителя
            Sdvig11();              //5) Сдвиг на 11 битов влево   
            //Конец функции Фейстеля.

            for (int i = 0; i < 32; i++)   //Сохраняем блок A в C
                C[i] = A[i];
            XOR(&t);                //Двоичное исключающее "или" для Bi и F. Результат сохраняем в At+1
            SohrBnext(&t);          //Сохраняем At в Bt+1


        }                           //Конец основного цикла.

        SkleivanieAB();             //Склеивание блоков A33+B33
        ViviodTexta(&p);            //Вывод зашифрованного текста в выходной файл
    }                               //Конец алгоритма.

    return 0;
}
//************************************************************************************
long Pow(int a, int b)              //Возведение в степень
{
    long rez = 1;
    for (int k = 1; k <= b; k++)
    {
        rez = rez * a;
    }
    return rez;
}

long FileSize(FILE* stream)         //Размер файла (в байтах)
{
    long curpos, length;
    curpos = ftell(stream);         //Сохраняем значение указателя на символ
    fseek(stream, 0L, SEEK_END);
    length = ftell(stream);
    fseek(stream, curpos, SEEK_SET);//Возвращяемся на исходное место
    return length;
}

void VivodSimvolov(FILE* stream, int* x)    //Вывод символов
{
    unsigned char cc;
    long curpos;
    int d;

    curpos = ftell(stream);         //Сохраняем значение указателя на символ

    fprintf(log, "      ");
    if ((*x == blokN - 1) && (nekratno == 1))
        d = FileSize(stream) % 8;
    else
        d = 8;
    for (int i = 0; i < d; i++)
    {
        cc = fgetc(stream);
        fprintf(log, "%8c", cc);
    }
    if ((*x == blokN - 1) && (nekratno == 1))
        for (int i = d; i < 8; i++)
            fprintf(log, "%8c", ' ');

    fprintf(log, "\n");
    fseek(stream, curpos, SEEK_SET);//Возвращяемся на исходное место
}

int Zapros(FILE** a, FILE** b, FILE** log)      //Выбор режима работы
{
    int vib;
    char c;
    printf("Shifrovanie ili Deshifrovanie? ('1' i '2' sootvetstvenno): ");
    c = _getch();
    printf("%c", c);
    if (c == '1')                               //Режим шифрования
    {
        *a = fopen("1.txt", "rb");
        *b = fopen("2.txt", "wb");
        *log = fopen("LogShifr.txt", "wb");
        vib = 1;
    }
    if (c == '2')                               //Режим расшифрования
    {
        *a = fopen("2.txt", "rb");
        *b = fopen("3.txt", "wb");
        *log = fopen("LogRasshifr.txt", "wb");
        vib = 2;
    }
    return vib;
}

int NumbOfBloks(FILE* a, long size)             //Определение числа блоков
{
    int n;
    size = FileSize(a);
    n = size / 8;                                   //Сохраняем число ПОЛНЫХ (8-символьных) блоков
    if (size % 8 != 0)                               //Если число символов в файле не кратно 8, то увеличивыем число блоков
    {
        n++;
        nekratno = 1;
    }
    fprintf(log, "%d %d\n", size, n);
    return n;
}

void ZapolnT(int* x)                        //Заполнение блока T символами из входного файла (fi)
{
    int d = FileSize(fi) % 8;
    if ((*x == blokN - 1) && (nekratno == 1))        //Если идёт последний блок и число символов в файле не кратно 8 то:
    {
        for (int i = 0; i < d; i++)
        {
            ch = fgetc(fi);               //Берем символ
            n = ch;                       //Сохраняем его номер
            for (int j = 0; j < 8; j++)        //Заполняем блок битами символов (число символов N<8)
            {
                if (n / Pow(2, 7 - j) >= 1)
                {
                    T[8 * i + j] = 1;
                    n = n - Pow(2, 7 - j);
                }
                else
                    T[8 * i + j] = 0;
            }
        }
        for (int i = 8 * (d % 8); i < 64; i++)     //Заполняем оставшуюся часть блока нулями.
            T[i] = 0;
    }
    else                                    //Иначе
    {
        for (int i = 0; i < 8; i++)
        {
            ch = fgetc(fi);               //Берем символ
            n = ch;                       //Сохраняем его номер
            for (int j = 0; j < 8; j++)        //Заполняем блок битами символов (число символов N=8)
            {
                if (n / Pow(2, 7 - j) >= 1)
                {
                    T[8 * i + j] = 1;
                    n = n - Pow(2, 7 - j);
                }
                else
                    T[8 * i + j] = 0;
            }
        }
    }
    //Вывод блока Т
    fprintf(log, "Tnach ");
    for (int i = 0; i < 64; i++)
        fprintf(log, "%d", T[i]);
}

void ZapolnB()                      //Заполнение блока B
{
    //Заполнение и вывод блока В1
    fprintf(log, "\nB1    ");
    for (int i = 0; i < 32; i++)
    {
        B[i] = T[i];
        fprintf(log, "%d", B[i]);
    }
}

void ZapolnA()                      //Заполнение блока A
{
    //Заполнение и вывод блока A1
    fprintf(log, "\nA1    ");
    for (int i = 0; i < 32; i++)
    {
        A[i] = T[32 + i];
        fprintf(log, "%d", A[i]);
    }
}


void ZapolnK(int* t)                //Заполнение и вывод подключа K
{
    fprintf(log, "\nK%-5d", *t);
    unsigned long x = 0;
    if (rejim == 1)                   //Если режим шифрования, то
    {                               //последовательность подключей имеет следующий вид:
        if ((*t >= 25) && (*t <= 32))
            x = KEY[7 - (*t - 1) % 8];
        else
            x = KEY[(*t - 1) % 8];
    }
    if (rejim == 2)                   //Если режим шифрования, то
    {                               //последовательность подключей имеет следующий вид:
        if ((*t >= 1) && (*t <= 8))
            x = KEY[(*t - 1) % 8];
        else
            x = KEY[7 - (*t - 1) % 8];
    }
    for (int i = 0; i < 32; i++)           //Переводим значение подключа в массив битов
    {
        if (x / Pow(2, 31 - i) >= 1)
        {
            K[i] = 1;
            x = x - Pow(2, 31 - i);
        }
        else
            K[i] = 0;
        fprintf(log, "%d", K[i]);       //Вывод подключа
    }
}

void Sum232()                       //Заполнение блока K
{
    fprintf(log, "\nSUM32 ");
    for (int c = 0, i = 31; i >= 0; i--)      //Поразрядное сложение блоков
    {
        if ((A[i] + K[i] + c) >= 2)       //Если переполнение, то:
        {
            SUM32[i] = A[i] + K[i] + c - 2;
            c = 1;
        }
        else                        //Иначе:
        {
            SUM32[i] = A[i] + K[i] + c;
            c = 0;
        }
    }
    for (int i = 0; i < 32; i++) //вывод сумматора
        fprintf(log, "%d", SUM32[i]);
}

void ZapolnN()                      //Заполнение накопителя
{
    fprintf(log, "\nN     ");
    for (int i = 0; i < 8; i++)            //В накопителе 8 чисел
    {
        for (int j = 0; j < 4; j++)        //Преобразуем 4 бита сумматора (начиная с конца, т.е. 4 посл бита, 4 предпосл бита и т.д.) в 10-чное число
            if (SUM32[31 - 4 * i - j] == 1)
                N[i] = N[i] + Pow(2, j);
        fprintf(log, "%d ", N[i]);    //Выводим накопитель
    }
}

void UzliZamen()                    //Прохождение заполнителя через узлы замен
{
    fprintf(log, "%\nN     ");
    for (int i = 0, k; i < 8; i++)
    {
        k = N[i];
        N[i] = TABLE[i][k];           //Выход из i-ого узла
        fprintf(log, "%d ", N[i]);
    }
}

void ZapolnN1()                     //Преобразование результатов функции UzliZamen() в накопитель N1
{
    for (int i = 0; i < 8; i++)            //Все 8 чисел накопителя N
        for (int j = 0; j < 4; j++)        //переводим в двоичный вид
        {
            if (N[i] / Pow(2, 3 - j) >= 1)
            {
                N1[4 * i + j] = 1;
                N[i] = N[i] - Pow(2, 3 - j);
            }
            else
                N1[4 * i + j] = 0;
        }
    fprintf(log, "%\nN1    ");
    for (int i = 0; i < 32; i++) //вывод выходного накопителя
        fprintf(log, "%d", N1[i]);
}

void Sdvig11()                      //Сдвиг на 11 бит влево. Результат сдвига - результа функции Фейстеля F(Ai,Ki)
{
    fprintf(log, "\nF     ");
    for (int i = 0; i < 21; i++)
    {
        F[i] = N1[i + 11];          //Сохраняем биты с 12 по 32
        fprintf(log, "%d", F[i]);
    }
    for (int i = 0; i < 11; i++)
    {
        F[i + 21] = N1[i];          //Сохраняем биты с 1 по 11
        fprintf(log, "%d", F[i + 21]);
    }
}

void XOR(int* t)                    //Ai+1 = Bi XOR F(Ai,Ki)
{
    fprintf(log, "\nA%-5d", *t + 1);
    for (int i = 0; i < 32; i++)
    {
        A[i] = F[i] ^ B[i];             //Двоичное исключающее "ИЛИ"
        fprintf(log, "%d", A[i]);
    }
}

void SohrBnext(int* t)              //Bi+1 = Ai
{
    fprintf(log, "\nB%-5d", *t + 1);
    for (int i = 0; i < 32; i++)
    {
        B[i] = C[i];
        fprintf(log, "%d", B[i]);
    }
}

void SkleivanieAB()                 //Склеивание блоков. T=AB
{
    fprintf(log, "\nTkon  ");
    for (int i = 0; i < 32; i++)
        T[i] = A[i];
    for (int i = 32; i < 64; i++)
        T[i] = B[i - 32];
    for (int i = 0; i < 64; i++)
        fprintf(log, "%d", T[i]);
}

void ViviodTexta(int* x)                    //Преобразование блока T в символы и вывод этих символов в выходной файл (fo)
{
    int d;
    fprintf(log, "\n\nRezultat: ");
    if ((*x == blokN - 1) && (nekratno == 1) && (rejim == 2))    //Если последний блок, "..." - некратно и режим расшифрования, то
        d = FileSize(fi) % 8;                           //выводим первые d=FileSize(fi)%8 символов.
    else                                            //Иначе
        d = 8;                                        //выводим 8 символов
    for (int i = 0; i < d; i++)
    {
        n = 0;
        for (int j = 0; j < 8; j++)                        //Преобразование из 8 бит в соответствующий символ
            if (T[8 * i + j] == 1)
                n = n + Pow(2, 7 - j);
        ch = n;
        fprintf(log, "%c", ch);
        fprintf(fo, "%c", ch);
    }
    fprintf(log, "\n");
}
