// Rajani Khatri
// CSC 362 Programming Assignment #4


/*
  Chutes and Ladders Game Simulation
 This program implements a variation of the classic board game "Chutes and Ladders."
  The game is played on a virtual "board" represented as a string of characters. Each
  position on the board has specific rules that dictate how players can move. Two players
  take turns moving across the board, each moving a randomly generated number of spaces
  (between 1 and 6) each turn. The first player to reach the end of the board wins.
 
  Board Representation:
  The board is represented by a 100-character string, where each character represents a
  different type of space with its own effects on the players. Key spaces include:
 
    - ' ' (space): A normal square with no special effects.
    - 'B': A space that moves the player back to the nearest "haven" ('H') found earlier
           on the board. If no previous havens exist, the player returns to the beginning.
    - 'F': A space that moves the player forward to the next haven ('H') found ahead.
           If there are no more havens, the player stays on this square.
    - 'H': A haven square that acts as a safe spot for players to return to or advance
           to when landing on a 'B' or 'F'.
    - 'a' - 'm': A chute, moving the player backward by a specified number of squares.
                 Each letter between 'a' and 'm' corresponds to a unique number of spaces
                 to move back.
    - 'o' - 'z': A ladder, moving the player forward by a specified number of squares.
                 Each letter between 'o' and 'z' corresponds to a unique number of spaces
                 to move forward.
 
  Gameplay Rules:
  1. Each player rolls a virtual dice (randomly generated number between 1 and 6) and
    moves forward by that many spaces on the board.
 
 2. After landing, players encounter various board effects based on the character at
     their new position:
    - Chute ('a' to 'm'): The player slides backward. After using the chute, it is
        removed and replaced by a '-' character so it cannot be reused.
    - Ladder ('o' to 'z'): The player climbs forward. After using the ladder, it is
        removed and replaced by a '-' character so it cannot be reused.
    - Backward Haven ('B'): The player moves back to the nearest haven ('H') they
       passed. If there is no prior haven, the player returns to the start.
    - Forward Haven ('F'): The player moves to the next haven ('H') ahead on the
        board. If there are no more havens ahead, the player remains on the current space.
    - Haven ('H'): A safe spot for players, but landing directly on an 'H' by normal
        dice roll does not remove it from the board.
 
  3. If a player lands on a space occupied by the other player, a "collision" occurs.
     The player who lands on the other must move back by one space. If this causes them
    to land on another special square (chute, ladder, 'B', or 'F'), they must perform the
    corresponding move effect as well.
 
  Board Symbols:
  - ' ' (blank): Normal square with no movement effect.
  - 'B': Moves the player back to the previous haven.
  - 'F': Moves the player forward to the next haven.
  - 'H': Haven that serves as a safe spot for players.
  - 'a' - 'm': Chutes (slide backward).
  - 'o' - 'z': Ladders (climb forward).
 
  Key Functions:
  This program is organized into several functions to handle different parts of the game:
 
    - main: Sets up the game, including initializing the board and players, starting the
                game loop, and managing turns. It calls the move and printBoard functions
                each turn, recording the board state, and announces the winner when a player
                reaches the end.
 
    - printBoard: Outputs the current game board to an external file, showing the exact
                      position of each player as well as any special squares they land on.
 
    - move: Handles the movement logic for each player’s turn. It moves the player by
                their dice roll, checks for collisions, and handles special board effects
                (chutes, ladders, and havens). It also prints out each move's result to the
                console.
 
    - findHaven: Finds the nearest haven ('H') from the player's position. If the player
                     landed on a 'B', it searches backward; if on an 'F', it searches forward.
                     When a haven is used, it is marked with '*' so it cannot be used again.
 
    - chuteLadder: Calculates a new position for the player based on the chute or ladder
                       they land on and removes the chute or ladder from the board by marking
                       it with '-'.
 
  Program Setup:
  - The board is defined as a string of 100 characters. The last character is `'\0'` to
    signify the end of the string.
 
  - `#define SIZE 100` is used to represent the board's length, allowing for easier reference
    to the end of the board throughout the program.
 
  - Players are represented by pointers that reference their position on the board. Pointer
    arithmetic is used to move them across the board.
 
  - Each turn is logged to an output file named "output.txt", showing each player’s position
    and any changes to the board layout after special moves.
 
 */







// These lines tell the computer what built-in tools we need to use
#include <stdio.h>    // This lets us print things and work with files
#include <stdlib.h>   // This gives us tools like random numbers
#include <time.h>     // This lets us use the current time to make random numbers more random
#include <ctype.h>    // This helps us work with letters

// This sets how big our game board is - 100 spaces
#define SIZE 100

// Function prototypes
char *findHaven(char *me, char *board, char direction);    // This will help find safe spots
char *chuteLadder(char *me);                              // This handles slides and climbs
void printBoard(FILE *fp, char *board, char *p1, char *p2); // This writes down what the board                                                               looks like
char *move(char *me, char *you, int playerNum, char *board); // This handles player movements


void main() {
    // The "board" is just a string of characters that represents the game's path.
    // Each spot has different letters that do special things in the game.
    char board[] = " mHk nH l B He Flq p H hByHlho H B jr H B ir j H F ku gd H pjB mH x BF i H m oB HlHF hoH B ";

    // We’re setting up two players, Player 1 and Player 2, who will both start at the very beginning of the board.
    char *p1 = board;  // Player 1 starts here
    char *p2 = board;  // Player 2 starts here too

    // We want to save the game's progress to a file called "output.txt".
    // So we try to open that file for writing. If something goes wrong, we’ll let the user know.
    FILE *fp;
    if ((fp = fopen("output.txt", "w")) == NULL) {
        printf("Error opening output file\n");
    }

    // This line makes sure the random numbers (like dice rolls) will be different each time we play,
    // using the current time to "seed" the randomness.
    srand(time(NULL));

    // Now we start the game! The loop will keep running until one of the players reaches the end of the board.
    while (p1 < board + SIZE && p2 < board + SIZE) {
        
        // Player 1 rolls the dice and moves. The "move" function does the heavy lifting,
        // deciding where Player 1 goes and if they hit any special spots or collide with Player 2.
        p1 = move(p1, p2, 1, board);

        // Now it’s Player 2’s turn to roll and move. The "move" function checks all the same things for Player 2.
        p2 = move(p2, p1, 2, board);

        // After both players have moved, we print the board's current state to the file
        // so we can look back and see where each player was after each round.
        printBoard(fp, board, p1, p2);
    }

    // Once the game is over, we close the file so everything we've recorded is saved properly.
    fclose(fp);

    // Finally, we check who got further on the board.
    // If Player 1 went farther, Player 1 wins; otherwise, Player 2 wins. We print out the winner.
    if (p1 > p2) {
        printf("Player 1 wins\n");
    } else {
        printf("Player 2 wins\n");
    }
}



char *findHaven(char *me, char *board, char direction) {
    // This pointer will store the location of the nearest "haven" ('H' character) if we find one.
    char *haven = NULL;

    // Check if the direction is 'B' (which means "go backward").
    if (direction == 'B') {
        // Start from the current position and look backward, one spot at a time.
        char *current = me - 1;
        while (current >= board) { // Keep going until we reach the start of the board.
            // If we find an 'H', that means we've found a "haven" we can go to.
            if (*current == 'H') {
                haven = current; // Set "haven" to this position.
                break; // Stop searching since we found what we needed.
            }
            current--; // Move one step back on the board.
        }
        // If we found a haven, mark it with '*' so we know it's been used.
        if (haven != NULL) {
            *haven = '*';
            return haven; // Return the haven location.
        }
        return board; // If we didn’t find any haven, go to the start of the board instead.
    }
    else {
        // If the direction is not 'B', assume we’re going forward (direction 'F').
        char *current = me + 1; // Start looking one step forward.
        while (current < board + SIZE) { // Keep going until the end of the board.
            // If we find an 'H', we've found a haven to go to.
            if (*current == 'H') {
                haven = current; // Set "haven" to this location.
                break; // Stop searching since we found it.
            }
            current++; // Move one step forward on the board.
        }
        // If we found a haven, mark it with '*' to show it’s been used.
        if (haven != NULL) {
            *haven = '*';
            return haven; // Return the haven location.
        }
        return me; // If no haven is found, just stay at the current position.
    }
}


char *chuteLadder(char *me) {
    // Save the current position of the player in a temporary pointer.
    // This will be useful to mark the position after moving.
    char *chute_ladder_pos = me;

    // Calculate the new position based on the character at the player's current spot.
    // 'n' is used as a reference point:
    // If *me is 'n', new_pos will be 0, meaning no movement.
    // If *me is 'o', new_pos will be 1, moving forward by 1.
    // If *me is 'm', new_pos will be -1, moving back by 1.
    int new_pos = (int)(*me - 'n');

    // Update the player's position by moving forward or backward by "new_pos" spaces.
    me = me + new_pos;

    // Mark the original position with '-' to indicate that the player has already moved from here.
    *chute_ladder_pos = '-';

    // Return the updated position of the player.
    return me;
}

char *move(char *me, char *you, int playerNum, char *board) {
    // Roll a random number between 1 and 6 to simulate a dice roll.
    int roll = (rand() % 6) + 1;

    // Move the player forward by the number of spaces rolled.
    me += roll;

    // Print a message showing which player moved, by how much, and their new position on the board.
    // 'me - board' gives the position as an index from the start of the board array.
    printf("Player %d moves %d to %ld", playerNum, roll, me - board);

    // Check if the player's new position is still within the board's boundaries.
    // 'board + SIZE' is the end of the board, so we make sure 'me' doesn't go beyond it.
    if (me < board + SIZE) {

        // Check if the player landed on the same space as the other player (a "collision").
        if (me == you) {
            // If there’s a collision, move the player back by one space.
            me--;

            // If moving back would take the player off the board, reset them to the start.
            if (me < board) me = board;

            // Print a message showing the new position after moving back due to collision.
            printf(" collision, moving back to %ld", me - board);

            // After handling the collision, check if the new spot has any special effects.
            if (me < board + SIZE) {
                // Get the character at the new position to see if it triggers any effects.
                char square = *me;

                // If the square is 'B' (backward haven), move the player to the nearest 'H' going backward.
                if (square == 'B') {
                    me = findHaven(me, board, 'B'); // Call findHaven to get the closest backward 'H'
                    printf(" and lands on a 'B', moving back to %ld", me - board); // Show the result
                }

                // If the square is 'F' (forward haven), move the player to the nearest 'H' going forward.
                else if (square == 'F') {
                    char* old_pos = me; // Save current position before moving
                    me = findHaven(me, board, 'F'); // Call findHaven to get the closest forward 'H'
                    if (me != old_pos) { // Check if the position actually changed
                        printf(" and lands on a 'F', moving forward to %ld", me - board);
                    }
                }

                // If the square is a chute ('a' to 'm') or ladder ('o' to 'z'), use chuteLadder to move accordingly.
                else if ((square >= 'a' && square <= 'm') || (square >= 'o' && square <= 'z')) {
                    me = chuteLadder(me); // Call chuteLadder to calculate the new position
                    if (square >= 'a' && square <= 'm') {
                        // 'a' to 'm' means chute, so the player slides down.
                        printf(" and lands on a chute, sliding down to %ld", me - board);
                    } else {
                        // 'o' to 'z' means ladder, so the player climbs up.
                        printf(" and lands on a ladder, climbing up to %ld", me - board);
                    }
                }
            }
        }

        // If there was no initial collision, check if the square has a special effect.
        else {
            char square = *me; // Get the character at the player's position.

            // 'B' means a backward haven, so we move the player to the nearest 'H' going backward.
            if (square == 'B') {
                me = findHaven(me, board, 'B'); // Find the closest backward 'H'
                printf(" and lands on a 'B', moving back to %ld", me - board);
            }

            // 'F' means a forward haven, so we move the player to the nearest 'H' going forward.
            else if (square == 'F') {
                char* old_pos = me; // Store current position
                me = findHaven(me, board, 'F'); // Find the closest forward 'H'
                if (me != old_pos) { // Check if the player actually moved
                    printf(" and lands on a 'F', moving forward to %ld", me - board);
                }
            }

            // 'H' is treated as a forward haven, so it moves the player forward to the closest 'H'.
            else if (square == 'H') {
                char* old_pos = me; // Save the current position
                me = findHaven(me, board, 'F'); // Find the next 'H' going forward
                if (me != old_pos) { // Check if there was movement
                    printf(" and lands on a 'H', and moves forward to %ld", me - board);
                }
            }

            // Chutes ('a' to 'm') move the player backward, while ladders ('o' to 'z') move forward.
            else if ((square >= 'a' && square <= 'm') || (square >= 'o' && square <= 'z')) {
                me = chuteLadder(me); // Use chuteLadder to adjust the position
                if (square >= 'a' && square <= 'm') {
                    printf(" and lands on a chute, sliding down to %ld", me - board); // Sliding down
                } else {
                    printf(" and lands on a ladder, climbing up to %ld", me - board); // Climbing up
                }
            }
        }

        // After all special moves, check again if there's a collision with the other player.
        if (me == you) {
            me--;  // If so, move the player back by one space to avoid the collision.
            
            // Ensure the player doesn't go off the board by accident.
            if (me < board) me = board;
            printf(" collision, moving back to %ld", me - board); // Print the adjusted position
        }
    }

    // Print a newline to separate turns for readability.
    printf("\n");

    // Return the updated position of the player.
    return me;
}

void printBoard(FILE *fp, char *board, char *p1, char *p2) {
    // Start at the beginning of the board.
    char* current = board;

    // Loop through each position on the board until the end.
    while (current < board + SIZE) {

        // If the current position matches Player 1's position, write '1' to the file.
        if (current == p1) {
            putc('1', fp);
        }

        // If the current position matches Player 2's position, write '2' to the file.
        else if (current == p2) {
            putc('2', fp);
        }

        // If neither player is at this position, write the character that's already on the board.
        else {
            putc(*current, fp);
        }

        // Move to the next position on the board.
        current++;
    }

    // After printing all board positions for this turn, add a newline to separate turns.
    putc('\n', fp);
}



/*
 #console-output
 Player 1 moves 4 to 4
 Player 2 moves 4 to 4 collision, moving back to 3 and lands on a chute, sliding down to 0
 Player 1 moves 3 to 7
 Player 2 moves 3 to 3
 Player 1 moves 6 to 13 and lands on a chute, sliding down to 4
 Player 2 moves 2 to 5
 Player 1 moves 6 to 10 and lands on a 'B', moving back to 6
 Player 2 moves 4 to 9
 Player 1 moves 1 to 7
 Player 2 moves 4 to 13
 Player 1 moves 5 to 12 and lands on a 'H', and moves forward to 21
 Player 2 moves 3 to 16 and lands on a chute, sliding down to 14
 Player 1 moves 1 to 22
 Player 2 moves 1 to 15 and lands on a 'F', moving forward to 26
 Player 1 moves 2 to 24 and lands on a 'B', moving back to 12
 Player 2 moves 2 to 28 and lands on a chute, sliding down to 22
 Player 1 moves 4 to 16
 Player 2 moves 1 to 23 and lands on a chute, sliding down to 17
 Player 1 moves 5 to 21
 Player 2 moves 1 to 18
 Player 1 moves 1 to 22
 Player 2 moves 2 to 20
 Player 1 moves 5 to 27 and lands on a chute, sliding down to 25
 Player 2 moves 5 to 25 collision, moving back to 24 and lands on a 'B', moving back to 2
 Player 1 moves 1 to 26
 Player 2 moves 4 to 6
 Player 1 moves 5 to 31 and lands on a 'H', and moves forward to 38
 Player 2 moves 3 to 9
 Player 1 moves 3 to 41
 Player 2 moves 4 to 13
 Player 1 moves 3 to 44
 Player 2 moves 3 to 16
 Player 1 moves 6 to 50
 Player 2 moves 4 to 20
 Player 1 moves 3 to 53
 Player 2 moves 6 to 26
 Player 1 moves 3 to 56
 Player 2 moves 1 to 27
 Player 1 moves 4 to 60 and lands on a chute, sliding down to 56
 Player 2 moves 2 to 29 and lands on a ladder, climbing up to 30
 Player 1 moves 1 to 57 and lands on a 'H', and moves forward to 64
 Player 2 moves 1 to 31 and lands on a 'H', and moves forward to 47
 Player 1 moves 2 to 66 and lands on a ladder, climbing up to 76
 Player 2 moves 2 to 49 and lands on a 'F', moving forward to 57
 Player 1 moves 3 to 79
 Player 2 moves 3 to 60
 Player 1 moves 2 to 81 and lands on a chute, sliding down to 79
 Player 2 moves 5 to 65
 Player 1 moves 6 to 85 and lands on a chute, sliding down to 79
 Player 2 moves 6 to 71 and lands on a chute, sliding down to 66
 Player 1 moves 1 to 80 and lands on a 'H', and moves forward to 82
 Player 2 moves 3 to 69 and lands on a 'F', moving forward to 73
 Player 1 moves 2 to 84
 Player 2 moves 2 to 75 and lands on a chute, sliding down to 74
 Player 1 moves 4 to 88
 Player 2 moves 4 to 78 and lands on a 'B', moving back to 31
 Player 1 moves 6 to 94
 Player 2 moves 5 to 36 and lands on a ladder, climbing up to 40
 Player 1 moves 6 to 100
 Player 2 moves 5 to 45 and lands on a chute, sliding down to 41
 Player 1 wins
 
 #output.txt
 2mH-1nH l B He Flq p H hByHlho H B jr H B ir j H F ku gd H pjB mH x BF i H m oB HlHF hoH B
  mH2 nH1l B He Flq p H hByHlho H B jr H B ir j H F ku gd H pjB mH x BF i H m oB HlHF hoH B  
  mH-12H l B H- Flq p H hByHlho H B jr H B ir j H F ku gd H pjB mH x BF i H m oB HlHF hoH B  
  mH- n1 l2B H- Flq p H hByHlho H B jr H B ir j H F ku gd H pjB mH x BF i H m oB HlHF hoH B  
  mH- n*1l B H2 Flq p H hByHlho H B jr H B ir j H F ku gd H pjB mH x BF i H m oB HlHF hoH B  
  mH- n* l B H-2F-q p 1 hByHlho H B jr H B ir j H F ku gd H pjB mH x BF i H m oB HlHF hoH B  
  mH- n* l B H- F-q p *1hBy2lho H B jr H B ir j H F ku gd H pjB mH x BF i H m oB HlHF hoH B  
  mH- n* l B 1- F-q p *2hBy*l-o H B jr H B ir j H F ku gd H pjB mH x BF i H m oB HlHF hoH B  
  mH- n* l B *- F12 p * -By*l-o H B jr H B ir j H F ku gd H pjB mH x BF i H m oB HlHF hoH B  
  mH- n* l B *- F-q2p 1 -By*l-o H B jr H B ir j H F ku gd H pjB mH x BF i H m oB HlHF hoH B  
  mH- n* l B *- F-q p2*1-By*l-o H B jr H B ir j H F ku gd H pjB mH x BF i H m oB HlHF hoH B  
  m2- n* l B *- F-q p * -B1*--o H B jr H B ir j H F ku gd H pjB mH x BF i H m oB HlHF hoH B  
  m*- n2 l B *- F-q p * -By1--o H B jr H B ir j H F ku gd H pjB mH x BF i H m oB HlHF hoH B  
  m*- n* l2B *- F-q p * -By*--o H B jr 1 B ir j H F ku gd H pjB mH x BF i H m oB HlHF hoH B  
  m*- n* l B *2 F-q p * -By*--o H B jr * B1ir j H F ku gd H pjB mH x BF i H m oB HlHF hoH B  
  m*- n* l B *- F2q p * -By*--o H B jr * B ir1j H F ku gd H pjB mH x BF i H m oB HlHF hoH B  
  m*- n* l B *- F-q p2* -By*--o H B jr * B ir j H F1ku gd H pjB mH x BF i H m oB HlHF hoH B  
  m*- n* l B *- F-q p * -By2--o H B jr * B ir j H F ku1gd H pjB mH x BF i H m oB HlHF hoH B  
  m*- n* l B *- F-q p * -By*2-o H B jr * B ir j H F ku gd1H pjB mH x BF i H m oB HlHF hoH B  
  m*- n* l B *- F-q p * -By*---2H B jr * B ir j H F ku gd1H p-B mH x BF i H m oB HlHF hoH B  
  m*- n* l B *- F-q p * -By*--- H B jr * B ir j 2 F ku gd H p-B m1 x BF i H m oB HlHF hoH B  
  m*- n* l B *- F-q p * -By*--- H B jr * B ir j * F ku gd 2 p-B m* - BF i H m1oB HlHF hoH B  
  m*- n* l B *- F-q p * -By*--- H B jr * B ir j * F ku gd * p2B m* - BF i H m oB1HlHF hoH B  
  m*- n* l B *- F-q p * -By*--- H B jr * B ir j * F ku gd * p-B m*2- BF i H m oB1H-HF hoH B  
  m*- n* l B *- F-q p * -By*--- H B jr * B ir j * F ku gd * p-B m* 2 BF - H m oB1H-HF -oH B  
  m*- n* l B *- F-q p * -By*--- H B jr * B ir j * F ku gd * p-B m* - BF - 2 m oB H-1F -oH B  
  m*- n* l B *- F-q p * -By*--- H B jr * B ir j * F ku gd * p-B m* - BF - *2- oB H-*F1-oH B  
  m*- n* l B *- F-q p * -By*--- 2 B jr * B ir j * F ku gd * p-B m* - BF - * - oB H-*F -oH1B  
  m*- n* l B *- F-q p * -By*--- * B j- * 2 ir j * F ku gd * p-B m* - BF - * - oB H-*F -oH B  
  m*- n* l B *- F-q p * -By*--- * B j- * B2ir - * F ku gd * p-B m* - BF - * - oB H-*F -oH B  
*/

 



