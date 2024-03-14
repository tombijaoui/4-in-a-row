#include <stdio.h>
#include <stdbool.h>

#define BOARD_MIN_SIDE 3
#define BOARD_MAX_SIDE 9
#define NUM_MOVES_MAX 81
#define MIN_TOKENS 3

#define RED_SLOT_SYMBOL ('R')
#define YELLOW_SLOT_SYMBOL ('Y')
#define EMPTY_SLOT_SYMBOL (' ')

#define MSG_GET_BOARD_ROWS   0
#define MSG_GET_BOARD_COLS   1
#define MSG_GET_NUMBER_TOKENS 2

#define UNDO -1
#define REDO -2

#define BOARD_VERT_SEP  '|'
#define BOARD_LEFT_ANG '\\'
#define BOARD_RIGHT_ANG '/'
#define BOARD_BOTTOM  '-'

void print_welcome_message();
void print_chose_color_message();
char ColorInitialization();
void print_read_game_params_message(int param);
int NumberRows();
int NumberColumns(int num_rows);
int NumberTokens(int num_rows);
int VerificationParameters(int param,int number,int num_rows);
void initialize_board(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int num_rows,int num_columns);
void initialize_move(int move[NUM_MOVES_MAX]);
void print_board(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int num_rows,int num_columns);
void print_chose_move_message(int player);
void print_enter_column_message();
int chose_player(int player);
char ColorPlayer(int player, char color);
int FullColumn(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int column, int num_columns);
int CheckColumn(int column, int num_columns);
int CorrectColumn(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int column, int num_columns);
void SaveMove(int column, int move[NUM_MOVES_MAX]);
int put_token(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int move[NUM_MOVES_MAX],char color,int num_rows,
               int num_columns,int column,int count);
void print_unavailable_undo_redo_message();
int NumberEmptyCases(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int column,int num_rows,int num_columns);
int ValidityOfUndoPart_1(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int column,int num_columns);
int ValidityOfUndoPart_2(int move[NUM_MOVES_MAX],int column, int count);
int UndoMove(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int move[NUM_MOVES_MAX],int num_rows, int count, int column);
int RedoMove(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int move[NUM_MOVES_MAX],char color, int count,
             int column, int num_rows);
int ValidityOfRedo(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int move[NUM_MOVES_MAX],int count, int column,
                   int num_columns);
int ActionOnBoard(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int move[NUM_MOVES_MAX],char color,
                  int num_rows,int num_columns,int count);
bool CheckWinHorizontal(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int num_rows, int num_columns, int num_tokens);
bool CheckWinVertical(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int num_rows, int num_columns, int num_tokens);
bool CheckWinDiagonalUp(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int num_rows, int num_columns, int num_tokens);
bool CheckWinDiagonalDown(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int num_rows, int num_columns, int num_tokens);
bool CheckWin(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int num_rows, int num_columns, int num_tokens);
bool CheckTie(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int num_columns);
void print_winner(int player, bool victory);
void game(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int move[NUM_MOVES_MAX],char first_color,int player,
          int M,int N,int T,int count);


//12 lines.//
int main() {
    print_welcome_message();
    int player = 1;
    char first_color = ColorInitialization(), board[BOARD_MAX_SIDE][BOARD_MAX_SIDE];
    int M = NumberRows(), N = NumberColumns(M), T = NumberTokens(M), move[NUM_MOVES_MAX], count = 0;
    initialize_board(board,M,N);
    initialize_move(move);
    print_board(board,M,N);
    game(board,move,first_color,player,M,N,T,count);
    return 0;
}

//This function prints the welcome message : 1 line.//
void print_welcome_message(){
    printf("Welcome to 4-in-a-row game! \n");
}

//This function prints the message to choose user's token's color : 1 line.//
void print_chose_color_message(){
    printf("Please choose starting color (Y)ellow or (R)ed: ");
}

//This function asks player 1 to choose a color of token and checks its correctness : 7 lines.//
char ColorInitialization(){
    char color;
    print_chose_color_message();
    scanf(" %c",&color);
    while(color != RED_SLOT_SYMBOL && color != YELLOW_SLOT_SYMBOL){
        print_chose_color_message();
        scanf(" %c",&color);
    }
    return color;
}

//This function prints the message to choose game's parameters : 2 lines.//
void print_read_game_params_message(int param){
    char const* const possible_params[] = {"rows", "columns", "tokens"};
    printf("Please enter number of %s: ",possible_params[param]);
}

//This function asks the user the number of rows : 5 lines.//
int NumberRows(){
    int param = MSG_GET_BOARD_ROWS,M;
    print_read_game_params_message(param);
    scanf("%d",&M);
    M = VerificationParameters(param,M,M);
    return M;
}

//This function asks the user the number of columns : 5 lines.//
int NumberColumns(int num_rows){
    int param = MSG_GET_BOARD_COLS, N;
    print_read_game_params_message(param);
    scanf("%d",&N);
    N = VerificationParameters(param,N,num_rows);
    return N;
}

//This function asks the user the number of tokens in order to win : 6 lines.//
int NumberTokens(int num_rows){
    int param = MSG_GET_NUMBER_TOKENS, T;
    print_read_game_params_message(param);
    scanf("%d",&T);
    T = VerificationParameters(param,T,num_rows);
    printf("\n");
    return T;
}

//This function checks the correctness of the parameters of the game : 13 lines.//
int VerificationParameters(int param,int number, int num_rows){
    if (param == MSG_GET_BOARD_ROWS){
        while (number > BOARD_MAX_SIDE || number < BOARD_MIN_SIDE){
            print_read_game_params_message(param);
            scanf("%d",&number);
        }
    }
    else if (param == MSG_GET_BOARD_COLS){
        while(number > BOARD_MAX_SIDE || number < num_rows){
            print_read_game_params_message(param);
            scanf("%d",&number);
        }
    }
    else {
        while(number < MIN_TOKENS || number > num_rows){
            print_read_game_params_message(param);
            scanf("%d",&number);
        }
    }
    return number;
}

//This function puts the character ' ' in all the pertinent cases : 3 lines.//
void initialize_board(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int num_rows,int num_columns) {
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_columns; j++) {
            board[i][j] = EMPTY_SLOT_SYMBOL;
        }
    }
}

//This function puts the value -5 in all the cases of the array : 2 lines.//
void initialize_move(int move[NUM_MOVES_MAX]){
    for(int i = 0; i < NUM_MOVES_MAX; i++){
        move[i] = -5;
    }
}

//This function prints the board : 12 lines.//
void print_board(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int num_rows,int num_columns){
    for(int i = 0 ; i < num_rows ; i++){
        for (int j = 0 ; j < num_columns + 1 ; j++){
            if(j != num_columns) {
                printf("%c%c", BOARD_VERT_SEP, board[i][j]);
            }
            else {
                printf("%c",BOARD_VERT_SEP);
            }
        }
        printf("\n");
    }
    printf("%c",BOARD_LEFT_ANG);
    for(int i = 1 ; i < 2*num_columns ; i++){
        printf("%c",BOARD_BOTTOM);
    }
    printf("%c\n",BOARD_RIGHT_ANG);
}

//This function prints the message before the player's move : 1 line.//
void print_chose_move_message(int player){
    printf("Your move, player %d. ", player);
}

//This function prints the message before that the user enter number of column : 1 line.//
void print_enter_column_message(){
    printf("Please enter column: ");
}

//This function returns player's number in an alternated way : 5 lines.//
int chose_player(int player){
    if(player == 1){
        player = 2;
    }
    else {
        player = 1;
    }
    return player;
}

//This function changes the color of tokens and matches it to the correct player in each round : 6 lines.//
char ColorPlayer(int player, char color){
    if(player == 2){
        if(color == RED_SLOT_SYMBOL){
            color = YELLOW_SLOT_SYMBOL;
        }
        else{
            color = RED_SLOT_SYMBOL;
        }
    }
    return color;
}

//This function checks that the users didn't choose a full column and ask him to choose another one : 11 lines.//
int FullColumn(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int column, int num_columns){
    while(board[0][column-1] != EMPTY_SLOT_SYMBOL) {
        if(column == UNDO || column == REDO){
            break;
        }
        printf("Column full. ");
        print_enter_column_message();
        scanf("%d", &column);
        while (column < REDO || column > num_columns || column == 0) {
            column = CheckColumn(column, num_columns);
            if(column == UNDO || column == REDO){
                break;
            }
        }
    }
    return column;
}

//This function checks if the user enters a correct number of column : 4 lines.//
int CheckColumn(int column, int num_columns){
    while(column < REDO || column > num_columns || column == 0){
        print_enter_column_message();
        scanf("%d",&column);
    }
    return column;
}

//This function checks that the column isn't full and the user enters a correct number column : 11 lines.//
int CorrectColumn(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int column, int num_columns){
    if(column > 0 && column <= num_columns){
        column = FullColumn(board,column,num_columns);
        column = CheckColumn(column,num_columns);
        return column;
    }
    else if(column < REDO || column > num_columns || column == 0){
        column = CheckColumn(column,num_columns);
        if(column <= 1 || column >= num_columns){
            column = FullColumn(board, column, num_columns);
        }
        return column;
    }
    else {
        return column;
    }
}

//This function saves the column entered by the player in each round : 7 lines.//
void SaveMove(int column, int move[NUM_MOVES_MAX]) {
    if (column != UNDO && column != REDO) {
        for (int i = 0; i < NUM_MOVES_MAX ; i++) {
            if (move[i] != -5) {
                continue;
            }
            else {
                move[i] = column - 1;
                break;
            }
        }
    }
}

//This function put the token of the color's player in the wanted column : 10 lines.//
int put_token(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int move[NUM_MOVES_MAX],char color,int num_rows,
               int num_columns,int column,int count){
    if(column >= 1 && column <= num_columns) {
        SaveMove(column,move);
        for (int i = num_rows - 1; i >= 0; i--) {
            if (board[i][column - 1] == RED_SLOT_SYMBOL || board[i][column - 1] == YELLOW_SLOT_SYMBOL) {
                continue;
            }
            else {
                board[i][column - 1] = color;
                count = 0;
                break;
            }
        }
    }
    return count;
}

//This function prints the message when it is impossible to do undo or redo move : 1 line.//
void print_unavailable_undo_redo_message(){
    printf("No moves to undo/redo. ");
}

//This function checks if all the cases of the last row are empty: 6 lines.//
int NumberEmptyCases(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int column,int num_rows,int num_columns) {
    int num_empty = 0;
    if (column == UNDO){
        for (int j = 0; j < num_columns; j++) {
            if (board[num_rows - 1][j] == EMPTY_SLOT_SYMBOL) {
                num_empty++;
            }
        }
    }
    return num_empty;
}

//This function requires to enter a correct value of column when it is impossible to do undo : 5 lines,//
int ValidityOfUndoPart_1(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int column,int num_columns){
    print_unavailable_undo_redo_message();
    print_enter_column_message();
    scanf("%d", &column);
    column = CorrectColumn(board, column, num_columns);
    return column;
}

//This function initializes the counter to 0 and the array which memories the columns entered by the players : 4 lines//
int ValidityOfUndoPart_2(int move[NUM_MOVES_MAX],int column, int count){
    initialize_move(move);
    SaveMove(column,move);
    count = 0;
    return count;
}

//This function replaced the last put token by the empty slot symbol if the player enter the value -1 : 14 lines.//
int UndoMove(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int move[NUM_MOVES_MAX],int num_rows, int count, int column){
   if(column == UNDO){
       for (int i = 0; i < NUM_MOVES_MAX; i++){
            if(move[i] != -5){
                continue;
            }
            else {
                for(int j = 0; j < num_rows; j++){
                    if(board[j][move[i + count - 1]] == EMPTY_SLOT_SYMBOL){
                        continue;
                    }
                    else {
                        board[j][move[i + count - 1]] = EMPTY_SLOT_SYMBOL;
                        count--;
                        break;
                    }
                }
                break;
            }
       }
   }
   return count;
}

//This function replaced the empty case by the put token in the last round : 15 lines.//
int RedoMove(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int move[NUM_MOVES_MAX],char color, int count,
             int column, int num_rows) {
    if (column == REDO) {
        count++;
        for (int i = 0; i < NUM_MOVES_MAX; i++) {
            if (move[i] != -5) {
                continue;
            }
            else {
                for (int j = num_rows - 1; j >= 0; j--) {
                    if (board[j][move[i + count - 1]] == RED_SLOT_SYMBOL ||
                        board[j][move[i + count - 1]] == YELLOW_SLOT_SYMBOL) {
                        continue;
                    }
                    else {
                        board[j][move[i + count - 1]] = color;
                        break;
                    }
                }
                break;
            }
        }
    }
    return count;
}

//This function checks if the redo move is possible : 15 lines.//
int ValidityOfRedo(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int move[NUM_MOVES_MAX],int count, int column,
                   int num_columns){
    if(column == REDO){
        for (int i = 0; i < NUM_MOVES_MAX; i++){
            if(move[i] != -5){
                continue;
            }
            else {
                if(move[i + count] == -5) {
                    while (column == REDO) {
                        print_unavailable_undo_redo_message();
                        print_enter_column_message();
                        scanf("%d", &column);
                        column = CorrectColumn(board, column, num_columns);
                        SaveMove(column, move);
                    }
                }
                else {
                    break;
                }
            }
        }
    }
    return column;
}

//This function gathers all the possible moves that can happen in one round : 16 lines.//
int ActionOnBoard(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int move[NUM_MOVES_MAX],char color,
                  int num_rows,int num_columns,int count){
    int column,num_empty;
    scanf("%d",&column);
    column = CorrectColumn(board,column,num_columns);
    column = ValidityOfRedo(board,move,count,column,num_columns);
    num_empty = NumberEmptyCases(board,column,num_rows,num_columns);
    if (num_empty == num_columns){
        while (column == UNDO) {
            column = ValidityOfUndoPart_1(board,column,num_columns);
            if(column != REDO){
                count = ValidityOfUndoPart_2(move,column,count);
            }
            else {
                column = ValidityOfRedo(board,move,count,column,num_columns);
            }
        }
    }
    count = put_token(board,move,color,num_rows,num_columns,column,count);
    count = UndoMove(board,move,num_rows,count,column);
    count = RedoMove(board,move,color,count,column,num_rows);
    return count;
}

//This function checks if one of the players aligned number of tokens selected horizontally : 16 lines.//
bool CheckWinHorizontal(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int num_rows, int num_columns, int num_tokens) {
    int count = 1;
    bool T_in_a_row = false;
    for (int i = num_rows - 1; i >= 0; i--) {
        for (int j = 0; j <= num_columns - num_tokens; j++) {
            if(board[i][j] == EMPTY_SLOT_SYMBOL){
                continue;
            }
            for (int k = 1; k <= num_tokens; k++) {
                if (board[i][j] != board[i][j + k]){
                    break;
                }
                else {
                    count++;
                }
            }
            if(count == num_tokens){
                T_in_a_row = true;
            }
            else {
                count = 1;
            }
        }
    }
    return T_in_a_row;
}

//This function checks if one of the players aligned number of tokens selected vertically : 16 lines.//
bool CheckWinVertical(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int num_rows, int num_columns, int num_tokens){
    int count = 1;
    bool T_in_a_column = false;
    for (int j = 0; j <= num_columns - 1 ; j++) {
        for (int i = num_rows - 1; i >= num_rows - num_tokens; i--) {
            if(board[i][j] == EMPTY_SLOT_SYMBOL){
                continue;
            }
            for (int k = 1; k <= num_tokens; k++) {
                if (board[i][j] != board[i - k][j]){
                    break;
                }
                else {
                    count++;
                }
            }
            if(count == num_tokens){
                T_in_a_column = true;
            }
            else {
                count = 1;
            }
        }
    }
    return T_in_a_column;
}

//This function checks if one of the players aligned number of tokens selected diagonally to up and right : 16 lines.//
bool CheckWinDiagonalUp(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int num_rows, int num_columns, int num_tokens){
    int count = 1;
    bool T_in_a_diagonal = false;
    for (int i = num_rows - 1; i >= num_tokens - 1; i--) {
        for (int j = 0; j <= num_columns - num_tokens + 1; j++) {
            if(board[i][j] == EMPTY_SLOT_SYMBOL){
                continue;
            }
            for (int k = 1; k <= num_tokens - 1; k++) {
                if (board[i][j] != board[i - k][j + k]){
                    break;
                }
                else {
                    count++;
                }
            }
            if(count == num_tokens){
                T_in_a_diagonal = true;
            }
            else {
                count = 1;
            }
        }
    }
    return T_in_a_diagonal;
}

//This function checks if one of the players aligned number of tokens selected diagonally to down and right : 16 lines.//
bool CheckWinDiagonalDown(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int num_rows, int num_columns, int num_tokens){
    int count = 1;
    bool T_in_a_diagonal = false;
    for (int i = 0; i <= num_rows - num_tokens + 1 ; i++) {
        for (int j = 0; j <= num_columns - num_tokens + 1 ; j++) {
            if(board[i][j] == EMPTY_SLOT_SYMBOL){
                continue;
            }
            for (int k = 1; k <= num_tokens -1; k++) {
                if (board[i][j] != board[i + k][j + k]){
                    break;
                }
                else {
                    count++;
                }
            }
            if(count == num_tokens){
                T_in_a_diagonal = true;
            }
            else {
                count = 1;
            }
        }
    }
    return T_in_a_diagonal;
}

//This function checks if one of the players aligned number of tokens selected in all the directions : 5 lines.//
bool CheckWin(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int num_rows, int num_columns, int num_tokens){
    bool vertical = CheckWinVertical(board,num_rows,num_columns,num_tokens);
    bool horizontal = CheckWinHorizontal(board,num_rows,num_columns,num_tokens);
    bool diagonal_up = CheckWinDiagonalUp(board,num_rows,num_columns,num_tokens);
    bool diagonal_down = CheckWinDiagonalDown(board,num_rows,num_columns,num_tokens);
    return vertical||horizontal||diagonal_up||diagonal_down;
}

//This function checks if there is a tie : 8 lines.//
bool CheckTie(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int num_columns){
    bool tie = false;
    int check_case = 0;
    for(int i = 0; i < num_columns; i++){
        if(board[0][i] == RED_SLOT_SYMBOL || board[0][i] == YELLOW_SLOT_SYMBOL){
            check_case++;
        }
    }
    if(check_case == num_columns){
        tie = true;
    }
    return tie;
}

//This function prints the message if a player won else it prints the message if there is a tie : 4 lines.//
void print_winner(int player, bool victory){
    if (victory == true) {
        printf("Player %d won! \n", player);
    }
    else {
        printf("That's a tie. \n");
    }
}

//This function prints the board after each step and checks if a player win or if there is a tie : 15 lines.//
void game(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int move [NUM_MOVES_MAX],char first_color,int player,
           int M,int N,int T,int count) {
    bool victory = false, tie = false;
    while (victory == false) {
        char color = ColorPlayer(player, first_color);
        print_chose_move_message(player);
        print_enter_column_message();
        count = ActionOnBoard(board,move,color,M,N,count);
        printf("\n");
        print_board(board, M, N);
        player = chose_player(player);
        tie = CheckTie(board, N);
        victory = CheckWin(board, M, N, T);
        if (tie == true) {
            break;
        }
    }
    player = chose_player(player);
    print_winner(player, victory);
}
