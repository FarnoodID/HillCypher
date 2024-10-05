#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include<time.h>
#define N 1024
#define M 256
//If input text is supposed to be read from File, This function is called to read it
void readFile(char* name,char text[N]){
    while(1){
        FILE *file = fopen (name, "r");
        long length;
        if (file)
        {
            char str[200];
            while (fgets(str, 200, file) != NULL){
                strcat(text, str);
            }
            fclose (file);
            break;
        }
        else{
            printf("Couldn't find %s!\nPlease enter a correct existing file:\n",name);
            char temp[20];
            scanf("%s",&temp);
            strcpy(name,temp);
        }
    }
}

//Get text either from CMD or file
void getText (char text[N]){
    printf("How do you want to input your text?\nEnter the number: \n1. file.txt\n2. CMD\n");
    int choice;
    scanf("%d",&choice);
    switch (choice){
        case 1: ;
            char name[20] = {'\0'};
            printf("Type the file name to read the text from:\n");
            scanf("%s",name);
            readFile(name, text);
            //printf("Your text:\n%s",text);
            break;
        case 2:  ;
            printf("Type your text and press Enter to show that your text is finished:\n");
            getchar();
            scanf("%1023[^\n]", text);
            break;
    }
}

//adding spaces at the end of text so it is dividable by 4
void padding(char text[N]){
    int len = strlen(text);
    int to_padd = len%4;
    if (to_padd !=0)
        to_padd = 4 - to_padd;
    while(to_padd){
        text[len++] = ' ';
        to_padd--;
    }
    text[len] = '\0';
}

//fill asciiText matrix
void fillAsciiText(char text[N], int asciiText[4][M]){
    int p = 0,q=0;
    for (int i=0;text[i]!='\0';i++){
        asciiText[p][q] = text[i];
        p++;
        if(p%4==0){
            p = 0;
            q+=1;
        }
    }
}

//generate KEY MATRIX
void generateKey(int key[4][4]){
    int keySample[4][4]={{1,4,1,6},
                    {2,7,5,15},
                    {3,11,7,24},
                    {2,8,2,11}};
    for (int i=0;i<4;i++){
        for (int j=0;j<4;j++)
            key[i][j] = keySample[i][j];
    }
}

//generate a random upper triangular matrix with determinant of 1 or -1
void generateUpperTria(int key[4][4]){
    int random=0;
    for (int i=0;i<4;i++){
        for (int j=0;j<4;j++){
            if (i==j){
                random = rand();
                random %= 2;
                if (random)
                    key[i][j] = 1;
                else
                    key[i][j] = -1;
            }
            else if (i>j)
                key[i][j] = 0;
            else{
                random = rand();
                random %= 101;
                random -= 50;
                key[i][j] = random;
            }
        }
    }
}

//each time this function is called it does an elementary row operation on given matrix
//it adds row "fromRow" multiplied to "multiple" to row "toRow"
// "multiple" can be -3,-2,-1,1,2,3
void elementaryRowOperation(int key[4][4], int fromRow, int multiple, int toRow){
    for (int i=0; i<4 ;i++){
        key[toRow][i] += multiple*key[fromRow][i];
        //key[toRow][i] %= 97;//prime number
    }
}

//generate KEY MATRIX
void generateKey2(int key[4][4]){
    for (int i=0;i<4;i++){
        for(int j=0;j<4;j++)
            key[i][j] = 0;
    }
    srand(time(0));
    generateUpperTria(key);
    int multiple = 0, fromRow = 0, toRow = 0, condition = 10;
    while(condition>0){
        multiple = (rand()%7)-3 ;
        if (multiple == 0)
            continue;
        do{
            fromRow = rand()%4;
            toRow = rand()%4;
        }while(toRow == fromRow);
        elementaryRowOperation(key,fromRow,multiple,toRow);
        condition--;
    }
}

// Function to calculate and store inverse, returns 0 if
// matrix is singular and if matrix inverse is not integer
int inverseCalc(int key[4][4], int rKey[4][4])
{
    float m[16],invOut[16];
    for (int i=0;i<4;i++){
        for (int j=0;j<4;j++)
            m[(i*4)+j] = (float)key[i][j];
    }
    for (int i=0;i<4;i++){
        for (int j=0;j<4;j++)
            invOut[(i*4)+j] = (float)rKey[i][j];
    }
    float inv[16], det;
    int i;

    inv[ 0] =  m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
    inv[ 4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
    inv[ 8] =  m[4] * m[ 9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[ 9];
    inv[12] = -m[4] * m[ 9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[ 9];
    inv[ 1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
    inv[ 5] =  m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
    inv[ 9] = -m[0] * m[ 9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[ 9];
    inv[13] =  m[0] * m[ 9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[ 9];
    inv[ 2] =  m[1] * m[ 6] * m[15] - m[1] * m[ 7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[ 7] - m[13] * m[3] * m[ 6];
    inv[ 6] = -m[0] * m[ 6] * m[15] + m[0] * m[ 7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[ 7] + m[12] * m[3] * m[ 6];
    inv[10] =  m[0] * m[ 5] * m[15] - m[0] * m[ 7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[ 7] - m[12] * m[3] * m[ 5];
    inv[14] = -m[0] * m[ 5] * m[14] + m[0] * m[ 6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[ 6] + m[12] * m[2] * m[ 5];
    inv[ 3] = -m[1] * m[ 6] * m[11] + m[1] * m[ 7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[ 9] * m[2] * m[ 7] + m[ 9] * m[3] * m[ 6];
    inv[ 7] =  m[0] * m[ 6] * m[11] - m[0] * m[ 7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[ 8] * m[2] * m[ 7] - m[ 8] * m[3] * m[ 6];
    inv[11] = -m[0] * m[ 5] * m[11] + m[0] * m[ 7] * m[ 9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[ 9] - m[ 8] * m[1] * m[ 7] + m[ 8] * m[3] * m[ 5];
    inv[15] =  m[0] * m[ 5] * m[10] - m[0] * m[ 6] * m[ 9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[ 9] + m[ 8] * m[1] * m[ 6] - m[ 8] * m[2] * m[ 5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if(det == 0){
        printf("Determinant is zero! Inverse doesn't exist\n");
        return 0;
    }
    if (det != 1 && det != -1){
        printf("Determinant is neither 1 nor -1! Inverse is not integer\n");
        return 0;
    }

    det = 1.f / det;

    for(i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;
    for(int i=0;i<16;i++)
        rKey[i/4][i%4] = (int)invOut[i];
    return 1;
}

// Multiplies two matrices mat1[][] and mat2[][]
// and returns result.
// (m1) x (m2) and (n1) x (n2) are dimensions
// of given matrices.
int multiply(int m1, int m2, int mat1[][4], int n1, int n2, int mat2[][M], int res[][M])
{
    int sum=0;
    if (m2 != n1){
        printf("The multiplication of matrices isn't possible.\n");
        return 0;
    }
    for (int c = 0; c < m1; c++) {
      for (int d = 0; d < n2; d++) {
        for (int k = 0; k < n1; k++) {
          sum += mat1[c][k]*mat2[k][d];
        }
        res[c][d]=sum;
        sum=0;
      }
    }
    return 1;
}

//print KEY MATRIX
void printKey(int key[4][4], int isInv){
    if (isInv)
        printf("Inverse of key matrix:\n");
    else
        printf("\nKey matrix:\n");
    printf(" __                   __\n");
    printf("|                       |\n");
    printf("|%5d %5d %5d %5d|\n",key[0][0],key[0][1],key[0][2],key[0][3]);
    printf("|                       |\n");
    printf("|%5d %5d %5d %5d|\n",key[1][0],key[1][1],key[1][2],key[1][3]);
    printf("|                       |\n");
    printf("|%5d %5d %5d %5d|\n",key[2][0],key[2][1],key[2][2],key[2][3]);
    printf("|                       |\n");
    printf("|%5d %5d %5d %5d|\n",key[3][0],key[3][1],key[3][2],key[3][3]);
    printf("|__                   __|   4x4\n\n");
}

//print matrix in numbers
void printNumerical(int asciiText[4][M], int len, int isEnc){
    if (isEnc)
        printf("\nEncrypted message:\n\n");
    else
        printf("\nText message in ascii:\n\n");
    for (int i=0;i<4;i++){
        for (int j=0;j<(len/4);j++)
            printf("%5d ",asciiText[i][j]);
        printf("\n");
    }
    printf("\n");
}

//print matrix in character
void printAlphabetical(int decrypted[4][M], int len){
    printf("\nDecrypted message:\n\n");
    for (int i=0;i<(len/4);i++){
        for (int j=0;j<4;j++)
            printf("%c",decrypted[j][i]);
    }
    printf("\n\n");
}
int main(){
    char text[N]={'\0'};
    int key[4][4] ={0};
    int rKey[4][4]={0};
    int asciiText[4][M]={0};
    int encrypted[4][M]={0};
    int decrypted[4][M]={0};
    //get text either from CMD or FILE
    getText(text);

    //adding space to text so it is dividable to 4
    padding(text);

    //fill  asciiText matrix with ascii converted characters of the text
    fillAsciiText(text,asciiText);
    int condition=0;
    do {
        printf("\n*********************************************************************************************\n");
        //generate key matrix and print it
        generateKey2(key);
        //generateKey2(key);
        printKey(key,0);

        //make encrypted
        multiply(4,4,key,4,(strlen(text)/4),asciiText,encrypted);

        //print encrypted matrix
        printNumerical(encrypted, strlen(text),1);
        printf("\nIs this key and encrypted message good enough?\n1. Yes\n2. No\n");
        scanf("%d",&condition);
        if (condition != 1)
            condition = 1;
        else
            condition = 0;
    } while( condition );
    printf("\n*********************************************************************************************\n");
    printf("\nWhich set of information do you want to be shown?\n1. Inverse Key, Decrypted Message\n2. Key, Inverse Key, Encrypted Message, Decrypted Message\n");
    int info=0;
    scanf("%d",&info);
    while (info != 1 && info != 2){
        printf("\nInvalid input!\n");
        scanf("%d",&info);
    }
    //calculate rKey which is inverse for key matrix
    inverseCalc(key, rKey);
    //make decrypted matrix
    multiply(4,4,rKey,4,(strlen(text)/4),encrypted,decrypted);
    if (info == 1){
        //print inverse of key matrix
        printKey(rKey,1);
        //print decrypted matrix in characters
        printAlphabetical(decrypted, strlen(text));
    }
    else if (info == 2){
        //print key matrix
        printKey(key,0);
        //print inverse of key matrix
        printKey(rKey,1);
        //print encrypted matrix
        printNumerical(encrypted, strlen(text),1);
        //print decrypted matrix in characters
        printAlphabetical(decrypted, strlen(text));
    }
    printf("\n****************************************  Thank You  ****************************************\n");
    getch();
}
