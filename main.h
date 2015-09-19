/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jichen-m <jichen-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/19 15:34:57 by jichen-m          #+#    #+#             */
/*   Updated: 2015/09/19 15:40:04 by jichen-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_Main_h
# define GAME_Main_h
# define NBLIN 80
# define NBCOL 80
# include <stdio.h>
# include <termios.h>
# include <fcntl.h>
# include <unistd.h>

#endif


int		main(int, const char*[]);
void	initArray(int, int, int[][NBCOL], int);
void	showArray(int, int, int[][NBCOL]);
int		runGame(int, int, int[][NBCOL]);
void	initVectPlayer(int, int, int[][NBCOL], int *, int);
int		runAStep(int , int, int[][NBCOL],int[],int[]);
char	key_pressed();
void	catchNextEvents( int[], int[]);
void	calculateNextPosition(int[], int[]);
void	moveVehicle(int, int, int[][NBCOL], int, int[], int[]);
void	clearScreen();
void	showBoard( int, int, int[][NBCOL]);
void	endOfGame(int);
enum
{
	gauche = 1,
	haut = 2,
	droite = 3,
	bas = 4
};
typedef enum
{
	false,
	true
}		bool;
bool	checkForWallCollision(int, int, int[]);
bool	checkForBeamCollision( int, int, int[][NBCOL], int[]);
