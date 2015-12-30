#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

const int Side = 10; // Comprimento dos lados do jogo
const int NUMBOMBS = 10; // N�mero de bombas
const double DUMMY = 0.001; // Toler�ncia, para que as bombas adjacentes sejam contadas devidamente.

void printboard(char[][Side]);
int numbombs(int,int,int[][2]);
int domove(char[][Side], char[][Side], int[][2], int, int, int*);
void getbombs(int[][2]);
void getmove(int*, int*);
int valid(int,int);


int main() {
  
    int gameover = 0; // Confirmar se o jogo acabou ou n�o
  
    char uncovered_board[Side][Side]; // Regista o quadro de jogo com a localiza��o de todas
                                      // as bombas e n�meros.

    char current_board[Side][Side]; // Regista o quadro de jogo que deve ser
                                    // mostrado ao utilizador.

    int totalmoves; // Regista o n�mero total de movimentos que restam ao utilizador at� ganhar o jogo.

    int i,j;
    int x,y;

    int listbombs[NUMBOMBS][2]; // Regista (x,y), coordenadas para todas as bombas.
  
    srand(time(0)); // Implementa o gerador de n�meros aleat�rios.

    // Inicializa o quadro de jogo.
    for (i=0;i<Side;i++) {
        for (j=0;j<Side;j++) {
            current_board[i][j] = 'X';
            uncovered_board[i][j] = 'X';
        }
    }

    // Aleat�riamente escolhe onde as bombas devem estar.
    getbombs(listbombs);
  
    // Coloca as bombas no quadro de jogo.
    for (i=0;i<NUMBOMBS;i++) 
        uncovered_board[listbombs[i][0]][listbombs[i][1]]='B';
  
    totalmoves = Side*Side - NUMBOMBS;

    // Continuar a jogar at� acabar o jogo.
    while (!gameover) {
        
        // Obter um movimento do utilizador. <---------------------- HERE!!!!
        printboard(current_board);
        getmove(&x,&y);
    
        // Executar esse movimento.
        gameover = domove(current_board,uncovered_board,listbombs,x,y,&totalmoves);

        // Verificar se o utilizador ganhou.
        if ((!gameover) && (totalmoves==0)) {
            printf("Parabens, ganhou!!!\n");
            gameover = 1;
        }
    }
  
    system("PAUSE");
    return 0;
  
}
// P�s-Instru��o: x e y v�o obter os valores que est�o escondidos
// 				  nas coordenadas que o utilizador indicar.
void getmove(int *x,int *y) {

    // Ler o movimento
    printf("\nA - Abrir casa\t\t|\tM - Marcar Bomba\nX - Casa por explorar\t|\t  - Casa vazia ja explorada\n\n");
    printf("A sua jogada:\nLinha: ");
		scanf("%d", x);
	printf("Coluna: ");
		scanf("%d", y);
  
    // Ler novamente o movimento, caso o anterior tenha sido inv�lido
    while (!valid(*x,*y)) {
        printf("\nCoordenadas invalidas. Tente novamente.\n");
		printf("A sua jogada:\nLinha: ");
			scanf("%d", x);
		printf("Coluna: ");
			scanf("%d", y);
    }
}
// Pr�-Instru��o: A array de caracteres passados para a fun��o
//                devem ser um quadrado de array dimensional de tamanho SIZE
// P�s-Instru��o: O quadro de jogo apresentado na fun��o ser� desenhado
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
    
        printf("%d %c", i, a);
        for (j=0;j<Side;j++)
            printf(" %c %c",board[i][j], a); 
        printf("\n");
		
    }
		printf("  %c",d);
	for (k=0;k<39;k++)
		printf("%c",f);
	printf("%c",e);
    printf("\n");
}
// Pr�-Instru��o: Ambos os caracteres da arrays s�o quadrados com dimens�es
//                 SIZE, a primeira dimens�o da lista de bombas
//                 deve ter o tamanho de NUMBOMBS,
//                 LINHA e COLUNA devem representar a linha e coluna
//                 para a qual o utilizador se moveu, e o n�mero de quadrados
//                 restantes que o utilizador tem de descobrir para ganhar.
// P�s-Instru��o: Esta fun��o ir� fazer com que o programa execute a jogada
//                e far� as mudan�as necess�rias nas estruturas auxiliares 
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

    // Tratar dos casos em que o utilizador escolhe um espa�o que j� foi aberto ou marcado.
    else if (board[row][column]!='X') {
    
        printf("Essas coordenadas ja foram usadas, tente novamente!\n");
        return 0;
    }
  
    // Executar um movimento normal (Sem perder o jogo).
    else {
 
        // Calcular o n�mero de bombas adjacentes, e colocar este n�mero
        // no quadro regular.
        num = numbombs(row, column, listbombs);
        (*totalmoves)--;
        
        // "Cast" para ter a certeza que o caractere � guardado na array
        board[row][column]=(char)(num+'0');

        // Se n�o houver bombas adjacentes, limpar a �rea em redor.
        if (num == 0) {
                
            // Loop por todos os elementos adjacentes do quadrado actual.  
            for (i=-1;i<2;i++) {
	            for (j=-1;j<2;j++) {
	  
	                // Limpar o quadrado apenas se ainda n�o foi
	                // utilizado anteriormente.
	                if (valid(row+i,column+j) && (board[row+i][column+j]=='X'))
	                    domove(board, realboard, listbombs, row+i, column+j, totalmoves);
	            }
            }
	      
        } // end-if num==0

        return 0;
        
    } // end else
}
// P�s-Instru��o: Devolver 1 se as coordenadas recebidas estiverem
//                dentro do quadro de jogo.
int valid(int row, int column) {

    // Procurar possiveis coordenadas inv�lidas.
    if (row < 0) return 0;
    
    else if (row >= Side) return 0;
  
    else if (column < 0) return 0;
  
    else if (column >= Side) return 0;
  
    // If we get here we're good!!!!!!!!!!!!! \O/
    else return 1;
}
// Pr�-Instru��o: O tamanho da primeira dimens�o de listbombs � NUMBOMBS,
//                e a linha e coluna indica para onde o utilizador se deslocou pela ultima vez.
// P�s-Instru��o: A fun��o ir� verificar quantas bombas est�o adjacentes
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
// Pr�-Instru��o: O tamanho da primeira dimens�o da listbombs � NUMBOMBS.
// P�s-Instru��o: A fun��o ir� escolher aleat�riamente o n�mero apropriado
//                de bombas e coloc�-las no quadro de jogo, guardando a
//                informa��o na bomblist.
void getbombs(int bomblist[][2]) {
  
    int i;
    int Hdummy;
    int curbombs[Side*Side];
  
    for (i=0; i<Side*Side; i++)
        curbombs[i] = 0;
    
    // Controla o n�mero de bombas colocadas actualmente no jogo.
    i = 0;
 
    // Continuar at� que todas as bombas aleat�rias sejam criadas.
    while(i<NUMBOMBS) {
    
        Hdummy = rand()%(Side*Side);

        // Adicionar bomba apenas se n�o for em duplicado.
        if (curbombs[Hdummy]==0) {
      
            bomblist[i][0] = Hdummy/Side;
            bomblist[i][1] = Hdummy%Side;
            i++;
            curbombs[Hdummy] = 1;
        }
    }
}