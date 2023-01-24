#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "conio.h"
void VivodSimvolov(FILE* stream, int* x);          //����� ��������
long Pow(int a, int b);                             //���������� � �������
long FileSize(FILE* stream);                        //������ ����� (� ������)
int Zapros(FILE** a, FILE** b, FILE** log);         //����� ������ ������
int NumbOfBloks(FILE* a, long size);                //����������� ����� ������
void ZapolnT(int* x);                               //���������� ����� T ��������� �� �������� ����� (fi)
void ZapolnB();                                     //���������� ����� B
void ZapolnA();                                     //���������� ����� A
void ZapolnK(int* t);                               //���������� ����� K
void Sum232();                                      //�������� �� ������ Pow(2,32)=2^32
void ZapolnN();                                     //���������� ����������
void UzliZamen();                                   //����������� ����������� ����� ���� �����
void ZapolnN1();                                    //�������������� ����������� ������� UzliZamen() � ���������� N1
void Sdvig11();                                     //����� �� 11 ��� �����. ��������� ������ - �������� ������� �������� F(Ai,Ki)
void XOR(int* t);                                   //Ai+1 = Bi XOR F(Ai,Ki)
void SohrBnext(int* t);                             //Bi+1 = Ai
void SkleivanieAB();                                //���������� ������. T=AB
void ViviodTexta(int* x);                           //�������������� ����� T � ������� � ����� ���� �������� � �������� ���� (fo)

FILE* log;                                          //���� ����.
FILE* fi;                                           //�������� ���������� � ��������/�������������� �������
FILE* fo;                                           //�������� ���������� � �������������/�������������� �������
int T[64], c[8], A[32], B[32], C[32], K[32], SUM32[32];   //����� ��� ��������� �����
int N[8] = { 0,0,0,0,0,0,0,0 }, N1[32], F[32];            //��������������� ����� (����������)
unsigned long KEY[8] =                               //����
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
unsigned char ch;                                   //���������� ���������� (��� ��������� ��������)
int n;                                              //��������������� ����������
int blokN;                                          //����� ������
long size;                                          //������ ����� � ������
bool nekratno = 0;                                    //���������� ����������: 0 - ���� ����� �������� ������ 8, 1 - ���� �� ������
int rejim;                                          //����� ������ ������ (1 - ����������, 2 -�������������)

int TABLE[8][16] =                                   //������� �����
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
    rejim = Zapros(&fi, &fo, &log);     //����� ������ ������ (���������� ��� �������������)
    blokN = NumbOfBloks(fi, size);     //����������� ����� ������

    for (int p = 0; p < blokN; p++)        //���������� ���� ������ T:
    {
        VivodSimvolov(fi, &p);
        ZapolnT(&p);                //���������� (p+1)-��� ����� �
        ZapolnB();                  //���������� ����� B
        ZapolnA();                  //���������� ����� A

        for (int t = 1; t <= 32; t++)     //������ ��������� �����:
        {
            fprintf(log, "\n%-2d*************************************", t);
            ZapolnK(&t);            //���������� ����� K

            //������� ��������:
            Sum232();               //1) �������� �� ������ 2^32
            ZapolnN();              //2) ���������� ����������
            UzliZamen();            //3) ������ ����� ���� �����
            ZapolnN1();             //4) ���������� ��������� ����������
            Sdvig11();              //5) ����� �� 11 ����� �����   
            //����� ������� ��������.

            for (int i = 0; i < 32; i++)   //��������� ���� A � C
                C[i] = A[i];
            XOR(&t);                //�������� ����������� "���" ��� Bi � F. ��������� ��������� � At+1
            SohrBnext(&t);          //��������� At � Bt+1


        }                           //����� ��������� �����.

        SkleivanieAB();             //���������� ������ A33+B33
        ViviodTexta(&p);            //����� �������������� ������ � �������� ����
    }                               //����� ���������.

    return 0;
}
//************************************************************************************
long Pow(int a, int b)              //���������� � �������
{
    long rez = 1;
    for (int k = 1; k <= b; k++)
    {
        rez = rez * a;
    }
    return rez;
}

long FileSize(FILE* stream)         //������ ����� (� ������)
{
    long curpos, length;
    curpos = ftell(stream);         //��������� �������� ��������� �� ������
    fseek(stream, 0L, SEEK_END);
    length = ftell(stream);
    fseek(stream, curpos, SEEK_SET);//������������ �� �������� �����
    return length;
}

void VivodSimvolov(FILE* stream, int* x)    //����� ��������
{
    unsigned char cc;
    long curpos;
    int d;

    curpos = ftell(stream);         //��������� �������� ��������� �� ������

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
    fseek(stream, curpos, SEEK_SET);//������������ �� �������� �����
}

int Zapros(FILE** a, FILE** b, FILE** log)      //����� ������ ������
{
    int vib;
    char c;
    printf("Shifrovanie ili Deshifrovanie? ('1' i '2' sootvetstvenno): ");
    c = _getch();
    printf("%c", c);
    if (c == '1')                               //����� ����������
    {
        *a = fopen("1.txt", "rb");
        *b = fopen("2.txt", "wb");
        *log = fopen("LogShifr.txt", "wb");
        vib = 1;
    }
    if (c == '2')                               //����� �������������
    {
        *a = fopen("2.txt", "rb");
        *b = fopen("3.txt", "wb");
        *log = fopen("LogRasshifr.txt", "wb");
        vib = 2;
    }
    return vib;
}

int NumbOfBloks(FILE* a, long size)             //����������� ����� ������
{
    int n;
    size = FileSize(a);
    n = size / 8;                                   //��������� ����� ������ (8-����������) ������
    if (size % 8 != 0)                               //���� ����� �������� � ����� �� ������ 8, �� ����������� ����� ������
    {
        n++;
        nekratno = 1;
    }
    fprintf(log, "%d %d\n", size, n);
    return n;
}

void ZapolnT(int* x)                        //���������� ����� T ��������� �� �������� ����� (fi)
{
    int d = FileSize(fi) % 8;
    if ((*x == blokN - 1) && (nekratno == 1))        //���� ��� ��������� ���� � ����� �������� � ����� �� ������ 8 ��:
    {
        for (int i = 0; i < d; i++)
        {
            ch = fgetc(fi);               //����� ������
            n = ch;                       //��������� ��� �����
            for (int j = 0; j < 8; j++)        //��������� ���� ������ �������� (����� �������� N<8)
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
        for (int i = 8 * (d % 8); i < 64; i++)     //��������� ���������� ����� ����� ������.
            T[i] = 0;
    }
    else                                    //�����
    {
        for (int i = 0; i < 8; i++)
        {
            ch = fgetc(fi);               //����� ������
            n = ch;                       //��������� ��� �����
            for (int j = 0; j < 8; j++)        //��������� ���� ������ �������� (����� �������� N=8)
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
    //����� ����� �
    fprintf(log, "Tnach ");
    for (int i = 0; i < 64; i++)
        fprintf(log, "%d", T[i]);
}

void ZapolnB()                      //���������� ����� B
{
    //���������� � ����� ����� �1
    fprintf(log, "\nB1    ");
    for (int i = 0; i < 32; i++)
    {
        B[i] = T[i];
        fprintf(log, "%d", B[i]);
    }
}

void ZapolnA()                      //���������� ����� A
{
    //���������� � ����� ����� A1
    fprintf(log, "\nA1    ");
    for (int i = 0; i < 32; i++)
    {
        A[i] = T[32 + i];
        fprintf(log, "%d", A[i]);
    }
}


void ZapolnK(int* t)                //���������� � ����� �������� K
{
    fprintf(log, "\nK%-5d", *t);
    unsigned long x = 0;
    if (rejim == 1)                   //���� ����� ����������, ��
    {                               //������������������ ��������� ����� ��������� ���:
        if ((*t >= 25) && (*t <= 32))
            x = KEY[7 - (*t - 1) % 8];
        else
            x = KEY[(*t - 1) % 8];
    }
    if (rejim == 2)                   //���� ����� ����������, ��
    {                               //������������������ ��������� ����� ��������� ���:
        if ((*t >= 1) && (*t <= 8))
            x = KEY[(*t - 1) % 8];
        else
            x = KEY[7 - (*t - 1) % 8];
    }
    for (int i = 0; i < 32; i++)           //��������� �������� �������� � ������ �����
    {
        if (x / Pow(2, 31 - i) >= 1)
        {
            K[i] = 1;
            x = x - Pow(2, 31 - i);
        }
        else
            K[i] = 0;
        fprintf(log, "%d", K[i]);       //����� ��������
    }
}

void Sum232()                       //���������� ����� K
{
    fprintf(log, "\nSUM32 ");
    for (int c = 0, i = 31; i >= 0; i--)      //����������� �������� ������
    {
        if ((A[i] + K[i] + c) >= 2)       //���� ������������, ��:
        {
            SUM32[i] = A[i] + K[i] + c - 2;
            c = 1;
        }
        else                        //�����:
        {
            SUM32[i] = A[i] + K[i] + c;
            c = 0;
        }
    }
    for (int i = 0; i < 32; i++) //����� ���������
        fprintf(log, "%d", SUM32[i]);
}

void ZapolnN()                      //���������� ����������
{
    fprintf(log, "\nN     ");
    for (int i = 0; i < 8; i++)            //� ���������� 8 �����
    {
        for (int j = 0; j < 4; j++)        //����������� 4 ���� ��������� (������� � �����, �.�. 4 ���� ����, 4 �������� ���� � �.�.) � 10-���� �����
            if (SUM32[31 - 4 * i - j] == 1)
                N[i] = N[i] + Pow(2, j);
        fprintf(log, "%d ", N[i]);    //������� ����������
    }
}

void UzliZamen()                    //����������� ����������� ����� ���� �����
{
    fprintf(log, "%\nN     ");
    for (int i = 0, k; i < 8; i++)
    {
        k = N[i];
        N[i] = TABLE[i][k];           //����� �� i-��� ����
        fprintf(log, "%d ", N[i]);
    }
}

void ZapolnN1()                     //�������������� ����������� ������� UzliZamen() � ���������� N1
{
    for (int i = 0; i < 8; i++)            //��� 8 ����� ���������� N
        for (int j = 0; j < 4; j++)        //��������� � �������� ���
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
    for (int i = 0; i < 32; i++) //����� ��������� ����������
        fprintf(log, "%d", N1[i]);
}

void Sdvig11()                      //����� �� 11 ��� �����. ��������� ������ - �������� ������� �������� F(Ai,Ki)
{
    fprintf(log, "\nF     ");
    for (int i = 0; i < 21; i++)
    {
        F[i] = N1[i + 11];          //��������� ���� � 12 �� 32
        fprintf(log, "%d", F[i]);
    }
    for (int i = 0; i < 11; i++)
    {
        F[i + 21] = N1[i];          //��������� ���� � 1 �� 11
        fprintf(log, "%d", F[i + 21]);
    }
}

void XOR(int* t)                    //Ai+1 = Bi XOR F(Ai,Ki)
{
    fprintf(log, "\nA%-5d", *t + 1);
    for (int i = 0; i < 32; i++)
    {
        A[i] = F[i] ^ B[i];             //�������� ����������� "���"
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

void SkleivanieAB()                 //���������� ������. T=AB
{
    fprintf(log, "\nTkon  ");
    for (int i = 0; i < 32; i++)
        T[i] = A[i];
    for (int i = 32; i < 64; i++)
        T[i] = B[i - 32];
    for (int i = 0; i < 64; i++)
        fprintf(log, "%d", T[i]);
}

void ViviodTexta(int* x)                    //�������������� ����� T � ������� � ����� ���� �������� � �������� ���� (fo)
{
    int d;
    fprintf(log, "\n\nRezultat: ");
    if ((*x == blokN - 1) && (nekratno == 1) && (rejim == 2))    //���� ��������� ����, "..." - �������� � ����� �������������, ��
        d = FileSize(fi) % 8;                           //������� ������ d=FileSize(fi)%8 ��������.
    else                                            //�����
        d = 8;                                        //������� 8 ��������
    for (int i = 0; i < d; i++)
    {
        n = 0;
        for (int j = 0; j < 8; j++)                        //�������������� �� 8 ��� � ��������������� ������
            if (T[8 * i + j] == 1)
                n = n + Pow(2, 7 - j);
        ch = n;
        fprintf(log, "%c", ch);
        fprintf(fo, "%c", ch);
    }
    fprintf(log, "\n");
}
