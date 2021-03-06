/* CS 305 HW5
 * Spring 2016
 * Author: Jonathan Liao
 * tic tac toe game loop
 * gametree.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>  // for random #s
#include "gametree.h"


/* in gametree.c -- students define */
GameNode * createNode(char b[3][3], int row, int col, char player) {
  if (b[row][col] != ' ') {
    return NULL;
  }

  GameNode * new_Node = malloc(sizeof(GameNode)); 
  new_Node->board[3][3]; 
  int i;
  int j;

  for(i = 0; i < 3; i++) {
    for(j = 0; j < 3; j++) {
      new_Node->board[i][j] = b[i][j];
    }
  }

  //value declarations for struct Node
  new_Node->board[row][col] = player;
  new_Node->score = '#';
  new_Node->player = player;

  if(winner(new_Node->board, new_Node->player) == 1) { //winner scenario. No more children
    for(i = 0; i < 3; i++) {
      for(j = 0; j < 3; j++) {
	new_Node->children[i][j] = NULL;
      }
    }
  }
  else { //not a winning scenario. Add more children/possible outcomes
    for(i = 0; i < 3; i++) {
      for(j = 0; j < 3; j++) {
	if(new_Node->player == 'X') { //if it was X's turn, it's now O's
	  new_Node->children[i][j] = createNode(new_Node->board, i, j, 'O');
	}  

	if(new_Node->player == 'O') { //if it was O's turn, it's now X's
	  new_Node->children[i][j] = createNode(new_Node->board, i, j, 'X');
	}
      }
    }

  }
  return new_Node;
}

GameNode * computerMove(GameNode * tree) {
  if(tree == NULL) {	
    return NULL; //there's nothing in this tree
  }

  //find best scoring tree
  //first iteration to find an 'X' which is best option for computer
  int i;
  int j;

  for(i = 0; i < 3; i++) {
    for(j = 0; j < 3; j++) {
      if(tree->children[i][j] != NULL && tree->children[i][j]->score == 'X') { //found a child with a score of X
	return tree->children[i][j];
      }
    }
  }

  //if we made it here, the first search failed to find a child with 'X'
  //second interation to find a 'T' which is second best option for computer
  for(i = 0; i < 3; i++) {
    for(j = 0; j < 3; j++) {
      if(tree->children[i][j] != NULL && tree->children[i][j]->score == 'T') { //found a child with a score of T
	return tree->children[i][j];
      }
    }
  }

  //if we made it here, the first and second search failed to find a child with a 'X' or 'T' respetively
  //third and last interation to find an 'O' which is last option for computer
  for(i = 0; i < 3; i++) {
    for(j = 0; j < 3; j++) {
      if(tree->children[i][j] != NULL && tree->children[i][j]->score == 'O') { //found a child with a score of O
	return tree->children[i][j];
      }
    }
  }
}

void calculateScores(GameNode * tree) { //takes the completed tree that has all board game combinations and gives each node a score.

  if(tree == NULL) {	
    return; //there's nothing in this tree
  }
  if(isLeaf(tree) == 1) { //is a leaf
    //these two if cases test for which player won each case
    if(winner(tree->board, tree->player) == 1) {
      tree->score = tree->player; //current player won
      return;
    }
    if(winner(tree->board, tree->player) == 0) {
      tree->score = 'T'; //no winner here, it's a tie
      return;
    }

  }
  else {//not a leaf
    int i;
    int j;
    for(i = 0; i < 3; i++) {
      for(j = 0; j < 3; j++) {
	if(tree->children[i][j] != NULL) {
	  calculateScores(tree->children[i][j]); //calculate on next children
	}
      }
    }
    ////counters for the case statements
    int x;
    int y;
    int child;
    int playerS = 0; 
    int playerO = 0;
    int playerT = 0;
    char oppositePlayer;

    for(x = 0; x < 3; x++) {
      for(y = 0; y < 3; y++) {
	if(tree->children[x][y] != NULL) {

	  if(tree->children[x][y]->score != tree->player) {
	    //case 1:child has opposite player score 
	    playerO++;
	    oppositePlayer = tree->children[x][y]->score;
	  }
	  if(tree->children[x][y]->score == tree->player) { 
	    //case 2:child has the same score as the player
	    playerS++;
	  }
	  else {
	    //case 3:child has T
	    playerT++;
	  }
	}
      }
    }
    //actual assignments of scores occurs here from counters
    if(playerO > 0) { //at least one child had an score with opposite player
      tree->score = oppositePlayer;
      return;
    }
    if(playerT == 0 && playerO == 0) { //all children have score of same player
      tree->score = tree->player;
      return;
    }
    else { //there was a mix of T's and current player letter scores. Or it was all T's
      tree->score = 'T';
      return;
    }
  }
}

