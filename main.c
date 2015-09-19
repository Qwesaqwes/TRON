/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jichen-m <jichen-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/19 15:22:23 by jichen-m          #+#    #+#             */
/*   Updated: 2015/09/19 15:40:18 by jichen-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int main(int argc, const char * argv[])
{
    int resultaMatch;
    int board [NBLIN][NBCOL];
    
    initArray(NBLIN, NBCOL, board, 0);// appelle de la fonction initArray
    resultaMatch = runGame(NBLIN, NBCOL, board);           // appelle de la foncion runGame
    endOfGame(resultaMatch);
}

void initArray(int nbLin, int nbCol, int iArray[][NBCOL], int value)
{
    int i,j;
    
    for (i = 0; i < nbLin; i++)
    {
        for (j = 0; j < nbCol; j++)
            iArray[i][j] = value;
    }
}

/**
 showArray: fonction pour afficher le tableau
 nbLin:
 nbCol:
 iArray:
 return: void
*/

void showArray(int nbLin, int nbCol, int iArray[][NBCOL]) // fonction pour afficher le tableau
{
    int i , j;
    
    usleep(100000);
    for (i = 0; i < nbLin; i++)
    {
        for (j = 0; j < nbCol; j++)
        {
            printf("%d", iArray[i][j]);
        }
        printf("\n");
    }
}



int runGame(int nbLin, int nbCol, int board[][NBCOL])
{
    int vectPlayer1[3];
    int vectPlayer2[3];
    int retourneRunAStep;
    int winner;

    initVectPlayer(nbLin, nbCol, board, vectPlayer1, 1);
    initVectPlayer(nbLin, nbCol, board, vectPlayer2, 2);
    // Fonction pour afficher l'état du plateau
    showArray(nbLin, nbCol, board);
    bool flag = false;
    do
    {
        retourneRunAStep = runAStep(nbLin, nbCol, board, vectPlayer1, vectPlayer2);
        if (retourneRunAStep == 0) // Match NUL donc 0
        {
            flag = true;
            winner = 0;
        }
        else if (retourneRunAStep == -1) // non collision donc -1
            flag = false;
        else if (retourneRunAStep == 1)
        {
            flag = true;
            winner = 1;
        }
        else if (retourneRunAStep == 2)
        {
            flag = true;
            winner = 2;
        }
        showBoard(nbLin, nbCol, board);
    }
    while (flag == false);
    return winner;
}

void initVectPlayer(int nbLin, int nbCol, int board [][NBCOL], int *vectPlayer, int numPlayer)
{
    // Déclaration variables
    int indiceLin = 0;
    int indiceCol = 0;

    // calculer indice ligne du milieu
    indiceLin = nbLin / 2 - 1;
    // affecter l'indice ligne du milieu à la 1ere case du vecteur joueur
    vectPlayer[0] = indiceLin ;
    // calculer indice colonne en fonction du numéro de joueur
    if (numPlayer == 1)
    {
        indiceCol = 2;
        vectPlayer[1] = indiceCol;
        vectPlayer[2] = droite;
    }
    else if (numPlayer == 2)
    {
        indiceCol = nbCol - 3;
        vectPlayer[1] = indiceCol;
        vectPlayer[2] = gauche;
    }
    else
        printf("incorrecte numPlayer %d", numPlayer);
    // Afficher le joueur sur le board en fonction de son vecteur
    board[indiceLin][indiceCol] = numPlayer;
}

int runAStep(int nbLin, int nbCol, int board[][NBCOL],int vectPlayer1[3],int vectPlayer2[3])
{
    int nextPosPlayer1[2];
    int nextPosPlayer2[2];
    bool wallCollisionPlayer1;
    bool wallCollisionPlayer2;
    bool beamCollisionPlayer1;
    bool beamCollisionPlayer2;
    int collision = 0;
    bool motoCollision = false;

    catchNextEvents(vectPlayer1, vectPlayer2);
    calculateNextPosition(vectPlayer1, nextPosPlayer1);
    calculateNextPosition(vectPlayer2, nextPosPlayer2);
    //vérifier qu'il n'y a pas de collision avec le mur
    wallCollisionPlayer1 = checkForWallCollision(nbLin, nbCol, nextPosPlayer1);
    if (wallCollisionPlayer1 == true)
        board[vectPlayer1[0]][vectPlayer1[1]] = -1;
    wallCollisionPlayer2 = checkForWallCollision(nbLin, nbCol, nextPosPlayer2);
    if (wallCollisionPlayer2 == true)
        board[vectPlayer2[0]][vectPlayer2[1]] = -1;
    // vérifie qu'il n'y a pas de collision avec les traces lumineuses
    beamCollisionPlayer1 = checkForBeamCollision( nbLin, nbCol, board, nextPosPlayer1);
    if (beamCollisionPlayer1 == true)
        board[nextPosPlayer1[0]][nextPosPlayer1[1]] = -1;
    beamCollisionPlayer2 = checkForBeamCollision( nbLin, nbCol, board, nextPosPlayer2);
    if (beamCollisionPlayer2 == true)
        board[nextPosPlayer2[0]][nextPosPlayer2[1]] = -1;
    //vérifier qu'il n'y a pas de collision avec les deux motos
    if ((nextPosPlayer1[0] == nextPosPlayer2[0]) && (nextPosPlayer1[1] == nextPosPlayer2[1]))
        board [nextPosPlayer1[0]][nextPosPlayer1[1]] = -1;
        motoCollision = true;
    // si la moto n'enregistre aucune collision, déplacez la sur la prochaine position
    if (!wallCollisionPlayer1 && !beamCollisionPlayer1) //
        moveVehicle(nbLin, nbCol, board, 1, vectPlayer1, nextPosPlayer1);
    if (!wallCollisionPlayer2 && !beamCollisionPlayer2)
        moveVehicle(nbLin, nbCol, board, 2, vectPlayer2, nextPosPlayer2);
    //s'il n'y a pas de collision ( retourne -1)
    if (!wallCollisionPlayer1 &&!beamCollisionPlayer1)
        collision = -1;
    if (!wallCollisionPlayer2 &&!beamCollisionPlayer2)
        collision = -1;
    // si les deux joueurs collisionnent à la fois MATCH NUL retourne 0
    if (wallCollisionPlayer1 == true || beamCollisionPlayer1 == true || motoCollision == true)
    {
        if (wallCollisionPlayer2 == true || beamCollisionPlayer2== true || motoCollision == true)
            collision = 0;
    }
    // le cas où j'ai un gagnant
    if (wallCollisionPlayer1 == true || beamCollisionPlayer1 == true)
        collision = 2;
    else if (wallCollisionPlayer2 == true || beamCollisionPlayer2== true)
        collision = 1;
    return collision;
}

char key_pressed()
{
    struct termios oldterm, newterm;
    int oldfd;
    char c, result = 0;
    
    tcgetattr(STDIN_FILENO, &oldterm);
    newterm = oldterm;
    newterm.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newterm);
    oldfd = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldfd | O_NONBLOCK);
    c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldterm);
    fcntl(STDIN_FILENO, F_SETFL, oldfd);
    
    if (c != EOF)
    {
        ungetc(c, stdin);
        result = getchar();
    }
    return result;
}


/*
 * Fonction pour indiquer ma nouvelle direction dependant de quelle key on appuille,
 * Player 1 ---> 'q' et 'd'
 * Player 2 ---> 'k' et 'm'
 */
void catchNextEvents( int vectPlayer1[3], int vectPlayer2[3])
{
    int i;
    char saveCarac1 = ' '; // dernier ordre donné pour joueur 1
    char saveCarac2 = ' '; // dernier ordre donné pour joueur 2
    char resultKeyPress[10];
    
    for (i = 0; i < 10; i++)
        resultKeyPress[i] =  key_pressed();
    for (i = 0; i < 10; i++)
    {
        if ((resultKeyPress[i] == 'q') || (resultKeyPress[i] == 'd'))
            saveCarac1 = resultKeyPress[i];
        else if ((resultKeyPress[i] == 'k') || (resultKeyPress[i] == 'm'))
            saveCarac2 = resultKeyPress[i];
    }
    // Si ordre de mouvement Player 1
    if (saveCarac1 != ' ')
    {
        if (vectPlayer1[2] == haut)
        {
            if (saveCarac1 == 'q')
                vectPlayer1[2] = gauche;
            else if (saveCarac1 == 'd')
                vectPlayer1[2] = droite;
        }
        else if (vectPlayer1[2] == droite)
        {
            if (saveCarac1 == 'q')
                vectPlayer1[2] = haut;
            else if (saveCarac1 == 'd')
                vectPlayer1[2] = bas;
        }
        else if (vectPlayer1[2] == bas)
        {
            if (saveCarac1 == 'q')
                vectPlayer1[2] = droite;
            else if (saveCarac1 == 'd')
                vectPlayer1[2] = gauche;
        }
        else if (vectPlayer1[2] == gauche)
        {
            if (saveCarac1 == 'q')
                vectPlayer1[2] = bas;
            else if (saveCarac1 == 'd')
                vectPlayer1[2] = haut;
        }
    }
    // Si ordre de mouvement Player 2
    if (saveCarac2 != ' ')
    {
        if (vectPlayer2[2] == haut)
        {
            if (saveCarac2 == 'k')
                vectPlayer2[2] = gauche;
            else if (saveCarac2 == 'm')
                vectPlayer2[2] = droite;
        }
        else if (vectPlayer2[2] == droite)
        {
            if (saveCarac2 == 'k')
                vectPlayer2[2] = haut;
            else if (saveCarac2 == 'm')
                vectPlayer2[2] = bas;
        }
        else if (vectPlayer2[2] == bas)
        {
            if (saveCarac2 == 'k')
                vectPlayer2[2] = droite;
            else if (saveCarac2 == 'm')
                vectPlayer2[2] = gauche;
        }
        else if (vectPlayer2[2] == gauche)
        {
            if (saveCarac2 == 'k')
                vectPlayer2[2] = bas;
            else if (saveCarac2 == 'm')
                vectPlayer2[2] = haut;
        }
    }
}

// Fonction qui calcule la prochaine position de la moto
void calculateNextPosition(int vecPlayer[3], int nextPosPlayer[2])
{
    if (vecPlayer[2] == haut)
    {
        nextPosPlayer[0] = vecPlayer[0] - 1;
        nextPosPlayer[1] = vecPlayer[1];
    }
    else if (vecPlayer[2] == droite)
    {
        nextPosPlayer[0] = vecPlayer[0];
        nextPosPlayer[1] = vecPlayer[1] + 1;
    }
    else if(vecPlayer[2] == bas)
    {
        nextPosPlayer[0] = vecPlayer[0] + 1;
        nextPosPlayer[1] = vecPlayer[1];
    }
    else if (vecPlayer[2] == gauche)
    {
        nextPosPlayer[0] = vecPlayer[0];
        nextPosPlayer[1] = vecPlayer[1] - 1;
    }
}

// Fonction pour bouger la moto
void moveVehicle(int nbLin, int nbCol, int board[][NBCOL], int numPlayer, int vectPlayer[3], int nextPosPlayer[2])
{
    int nextLin = nextPosPlayer[0];
    int nextCol = nextPosPlayer[1];
    
    // marquer la nouvelle position sur le plateau de jeux
    board[nextLin][nextCol] = numPlayer;
    // mettre à jour le vecteur du joueur
    vectPlayer[0] = nextLin;
    vectPlayer[1] =  nextCol;
}

bool checkForWallCollision(int nbLin, int nbCol, int nextPosPlayer[])
{
    bool wallCollision = false;
    
    // vrai s'il y a collision
    if (nextPosPlayer[0] < 0)
        wallCollision = true;
    else if (nextPosPlayer[0] >= nbLin)
        wallCollision = true;
    else if (nextPosPlayer[1] < 0)
        wallCollision = true;
    else if (nextPosPlayer[1] >= nbCol)
        wallCollision = true;
    // faux s'il n'y a pas
    else
        wallCollision = false;
    return wallCollision;
}

// Fonction pour vérifier collision avec les traces
bool checkForBeamCollision( int nbLin, int nbCol, int board[][NBCOL], int nextPosPlayer[])
{
    bool beamCollision = false;
    int nextLin = nextPosPlayer[0];
    int nextCol = nextPosPlayer[1];
    
    if (board[nextLin][nextCol] != 0)
        beamCollision = true;
    else
        beamCollision = false;
    return beamCollision;
}

// Fonction qui nous permettra d'effacer l'écran du terminal et de replacer le courseur en haut
void clearScreen()
{
    printf("%c[2J", 0x1B);
    printf("%c[%d;%dH", 0x1B, 1, 1);
}

void	showBoard( int nbLin, int nbCol, int board[][NBCOL])
{
    int i , j;

    clearScreen();
    usleep(100000);
    for (i = 0; i < nbLin + 2; i++)
	{
		for (j = 0; j < nbCol + 2; j++)
		{
			    
            if (i != 0 && i != nbLin +1 && j != 0 && j != nbCol + 1)
			{
				if (board[i - 1][j - 1] == 0)
					printf(" ");
				else if (board[i - 1][j - 1] == 1)
					printf("\033[31;01mo\033[00m");
				else if (board[i - 1][j - 1] == 2)
					printf("\033[36;01mo\033[00m");
				else if (board[i - 1][j - 1] == -1)
					printf("\033[33;01mx\033[00m");
			}
			else if ((i == 0 ||i == nbLin + 1) && (j == 0 || j == nbCol + 1))
                printf(" ");
			else if (i == 0 || i == nbLin + 1)
                printf("+");
			else if (j == 0 || j == nbCol + 1)
                printf("|");
		}
		printf("\n");
	}
}

void	endOfGame(int numWinner)
{
    printf("\n\n#### THE GAME IS OVER ####\n");
    if (numWinner == 0)
        printf("\nIt's a draw!\n");
    else if (numWinner == 1)
        printf("\nAnd the winner is Player %d!\n", numWinner);
    else if (numWinner == 2)
        printf("\nAnd the winner is Player %d!\n", numWinner);
    
}
