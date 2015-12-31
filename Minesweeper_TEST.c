#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

const int Side = 10; // Comprimento dos lados do jogo
const int NUMBOMBS = 10; // Número de bombas
const double DUMMY = 0.001; // Tolerância, para que as bombas adjacentes sejam contadas devidamente.

char getoption();
void printboard(char[][Side]);
int numbombs(int,int,int[][2]);
int domove(char[][Side], char[][Side], int[][2], int, int, int*);
void getbombs(int[][2]);
void getmove(int*, int*);
int valid(int,int);


int main() {
  
    int gameover = 0; // Confirmar se o jogo acabou ou não
  
    char uncovered_board[Side][Side]; // Regista o quadro de jogo com a localização de todas
                                      // as bombas e números.

    char current_board[Side][Side]; // Regista o quadro de jogo que deve ser
                                    // mostrado ao utilizador.

    int totalmoves; // Regista o número total de movimentos que restam ao utilizador até ganhar o jogo.

    int i,j;
    int x,y;

    int listbombs[NUMBOMBS][2]; // Regista (x,y), coordenadas para todas as bombas.
    
    char opt;  

    srand(time(0)); // Implementa o gerador de números aleatórios.

    // Inicializa o quadro de jogo.
    for (i=0;i<Side;i++) {
        for (j=0;j<Side;j++) {
            current_board[i][j] = 'X';
            uncovered_board[i][j] = 'X';
        }
    }

    // Aleatóriamente escolhe onde as bombas devem estar.
    getbombs(listbombs);
  
    // Coloca as bombas no quadro de jogo.
    for (i=0;i<NUMBOMBS;i++) 
        uncovered_board[listbombs[i][0]][listbombs[i][1]]='B';
  
    totalmoves = Side*Side - NUMBOMBS;

    // Continuar a jogar até acabar o jogo.
    while (!gameover) {
        
        // Obter um movimento do utilizador. <---------------------- HERE!!!!
        printboard(current_board);

        // Escolhe tipo de jogada
        opt = getoption();
        getmove(&x,&y);
        
        if (opt == 'A' || opt == 'a') {
            // Executar esse movimento.
            gameover = domove(current_board,uncovered_board,listbombs,x,y,&totalmoves);
        } else if (opt == 'M' || opt == 'm') {
            // Marcar bomba
            // ...
            printf("\n\tTODO!\n\n");
            // Implementar o código para marcar a bomba
        }

        // Verificar se o utilizador ganhou.
        if ((!gameover) && (totalmoves==0)) {
            printf("Parabens, ganhou!!!\n");
            gameover = 1;
        }
    }
  
    system("PAUSE");
    return 0;
  
}


char getoption() {

    char opt;
    int valid = 0;

    do {
        printf("\nEscolha tipo de jogada: (A/M)\n");
		scanf(" %c", &opt);

        if (opt == 'A' || opt == 'a' || opt == 'M' || opt == 'm')
            valid = 1;
        else
            printf("Opção invalida! Por favor tente de novo\n");
    } while (valid == 0);

    return opt;
}

// Pós-Instrução: x e y vão obter os valores que estão escondidos
// 				  nas coordenadas que o utilizador indicar.
void getmove(int *x,int *y) {

    char c;

    // Ler o movimento
    printf("A sua jogada:\nLinha: ");
		scanf(" %c", &c);
	printf("Coluna: ");
		scanf("%d", y);
    *x = (int)(c) - 65;
    
    // Ler novamente o movimento, caso o anterior tenha sido inválido
    while (!valid(*x,*y)) {
        printf("\nCoordenadas invalidas. Tente novamente.\n");
		printf("A sua jogada:\nLinha: ");
			scanf(" %c", &c);
		printf("Coluna: ");
			scanf("%d", y);
        *x = (int)(c) - 65;
    }
}
// Pré-Instrução: A array de caracteres passados para a função
//                devem ser um quadrado de array dimensional de tamanho SIZE
// Pós-Instrução: O quadro de jogo apresentado na função será desenhado
// 				  exactamente conforme os caracteres guardados na array.
void printboard(char board[][Side]) {

    int i,j,k;
	char a=186, b=201, c=187, d=200, e=188, f=205;

    // Desenhar os lados da tabela
    printf("    ");
    for (i=0;i<Side;i++)
        printf("%d   ", i);
		
	printf("\n  %c",b);
	for (k=0;k<39;k++)
		printf("%c",f);
	printf("%c",c);
    printf("\n");

    // Desenhar o quadro de jogo.
    for (i=0;i<Side;i++) {
    
        printf("%c %c", 65 + i , a);
        for (j=0;j<Side;j++)
            if (board[i][j] != '0')
                printf(" %c %c",board[i][j], a);
            else
                printf(" %c %c",' ', a);
        printf("\n");
		
    }
		printf("  %c",d);
	for (k=0;k<39;k++)
		printf("%c",f);
	printf("%c",e);
    printf("\n");
    printf("\nA - Abrir casa\t\t|\tM - Marcar Bomba\nX - Casa por explorar\t|\t  - Casa vazia ja explorada\n\n");
}
// Pré-Instrução: Ambos os caracteres da arrays são quadrados com dimensões
//                 SIZE, a primeira dimensão da lista de bombas
//                 deve ter o tamanho de NUMBOMBS,
//                 LINHA e COLUNA devem representar a linha e coluna
//                 para a qual o utilizador se moveu, e o número de quadrados
//                 restantes que o utilizador tem de descobrir para ganhar.
// Pós-Instrução: Esta função irá fazer com que o programa execute a jogada
//                e fará as mudanças necessárias nas estruturas auxiliares 
//                para assegurar que o jogo continua a correr devidamente.
int domove(char board[][Side], char realboard[][Side],
            int listbombs[][2], int row, int column, int *totalmoves) {


    int i, j, num;
  
    // Tratar dos casos em que o utilizador perde o jogo.
    if (realboard[row][column]=='B') {
    
        board[row][column]='B';
        for (i=0;i<NUMBOMBS;i++) 
            board[listbombs[i][0]][listbombs[i][1]]='B';
    
        printboard(board);
        printf("Perdeu!\n");
        return 1;
    }

    // Tratar dos casos em que o utilizador escolhe um espaço que já foi aberto ou marcado.
    else if (board[row][column]!='X') {
    
        printf("Essas coordenadas ja foram usadas, tente novamente!\n");
        return 0;
    }
  
    // Executar um movimento normal (Sem perder o jogo).
    else {
 
        // Calcular o número de bombas adjacentes, e colocar este número
        // no quadro regular.
        num = numbombs(row, column, listbombs);
        (*totalmoves)--;
        
        // "Cast" para ter a certeza que o caractere é guardado na array
        board[row][column]=(char)(num+'0');

        // Se não houver bombas adjacentes, limpar a área em redor.
        if (num == 0) {
                
            // Loop por todos os elementos adjacentes do quadrado actual.  
            for (i=-1;i<2;i++) {
	            for (j=-1;j<2;j++) {
	  
	                // Limpar o quadrado apenas se ainda não foi
	                // utilizado anteriormente.
	                if (valid(row+i,column+j) && (board[row+i][column+j]=='X'))
	                    domove(board, realboard, listbombs, row+i, column+j, totalmoves);
	            }
            }
	      
        } // end-if num==0

        return 0;
        
    } // end else
}
// Pós-Instrução: Devolver 1 se as coordenadas recebidas estiverem
//                dentro do quadro de jogo.
int valid(int row, int column) {

    // Procurar possiveis coordenadas inválidas.
    if (row < 0) return 0;
    
    else if (row >= Side) return 0;
  
    else if (column < 0) return 0;
  
    else if (column >= Side) return 0;
  
    // If we get here we're good!!!!!!!!!!!!! \O/
    else return 1;
}
// Pré-Instrução: O tamanho da primeira dimensão de listbombs é NUMBOMBS,
//                e a linha e coluna indica para onde o utilizador se deslocou pela ultima vez.
// Pós-Instrução: A função irá verificar quantas bombas estão adjacentes
//                ao quadrado que o utilizador deseja descobrir.
int numbombs(int row ,int column ,int listbombs[][2]) {

    int i;
    int count = 0;
    for (i=0;i<NUMBOMBS;i++) {

        // Check each bomb one by one against the square to uncover. Verificar bombas, uma a uma em cada quadrado para destapar.
        if ((abs(row-listbombs[i][0])<=1+DUMMY) && (abs(column-listbombs[i][1])<=1+DUMMY)) {
            count++; 
        }  
    }
    
    return count;
}
// Pré-Instrução: O tamanho da primeira dimensão da listbombs é NUMBOMBS.
// Pós-Instrução: A função irá escolher aleatóriamente o número apropriado
//                de bombas e colocá-las no quadro de jogo, guardando a
//                informação na bomblist.
void getbombs(int bomblist[][2]) {
  
    int i;
    int Hdummy;
    int curbombs[Side*Side];
  
    for (i=0; i<Side*Side; i++)
        curbombs[i] = 0;
    
    // Controla o número de bombas colocadas actualmente no jogo.
    i = 0;
 
    // Continuar até que todas as bombas aleatórias sejam criadas.
    while(i<NUMBOMBS) {
    
        Hdummy = rand()%(Side*Side);

        // Adicionar bomba apenas se não for em duplicado.
        if (curbombs[Hdummy]==0) {
      
            bomblist[i][0] = Hdummy/Side;
            bomblist[i][1] = Hdummy%Side;
            i++;
            curbombs[Hdummy] = 1;
        }
    }
}
