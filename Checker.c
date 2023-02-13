/* Program to print and play checker games.

  Skeleton program written by Artem Polyvyanyy, artem.polyvyanyy@unimelb.edu.au,
  September 2021, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.

  Student Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: Zheqi Shen 1254834
  Dated:     4/10/2021

*/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>

/* some #define's from my sample solution ------------------------------------*/
#define BOARD_SIZE          8       // board size
#define ROWS_WITH_PIECES    3       // number of initial rows with pieces
#define BPIECE         'b'     // black piece character
#define WPIECE         'w'     // white piece character
#define BTOWER         'B'     // black tower character
#define WTOWER         'W'     // white tower character
#define COST_PIECE          1       // one piece cost
#define COST_TOWER          3       // one tower cost
#define TREE_DEPTH          3       // minimax tree depth
#define COMP_ACTIONS        10      // number of computed actions
#define FIRST_ROW           "     A   B   C   D   E   F   G   H"
#define ROW_SEPERATOR       "\n   +---+---+---+---+---+---+---+---+\n"
#define ERROR               1
#define ALLGOOD             0
#define BOARD_SEPERATOR     "=====================================\n"
#define EMPTY_CELL          '.'
#define NORTH               -1
#define EAST                1
#define SOUTH               1
#define WEST                -1
#define VALID               1
#define INVALID             0
#define MOVE_ELEMENTS       4
#define MAX_ACTION_NUM 10
#define SIDE_DIVIDER 2
#define ROW_1 0
#define ROW_2 1
#define ROW_3 2
#define ODD_COL 1
#define EVEN_COL 0
#define ROW_6 5
#define ROW_7 6
#define ROW_8 7
#define FIRST_COL 'A'
#define LEFT_BORDER 0
#define INDEX_DEDUCTION 1
#define BLACK 1
#define WHITE 0
#define CAPTURE_DIVIDER 2
#define MOVE 1
#define CAPTURE 2
#define CAPTURE_FACTOR 2
#define ADD_CHILD 1
#define EXPAND_FACTOR 2
#define TOP_ROW 0
#define BOTTOM_ROW 7
#define START 0
#define INITIAL_CAPACITY 2
#define NEXT_ACTION 1
#define NEXT_TWO_ACTION 2
#define NO_CHILD 0
#define EVEN_DEPTH 0
#define DEPTH_1 1
#define DEPTH_2 2


/* one type definition from my sample solution -------------------------------*/
typedef unsigned char board_t[BOARD_SIZE][BOARD_SIZE];  // board type
typedef struct node node_t;
struct node {
    board_t board;
    node_t* children, * parent;
    int cost, children_num;
};

typedef struct {
    int from_row, to_row;
    char from_col, to_col;
} move_t;


void initialise_board(board_t);
void error_report(board_t, move_t, int);
int board_cost(board_t);
void print_board(board_t, move_t, int);
void capture_action(board_t, move_t, char, int, int);
int manipulate_board(board_t, move_t, int action_num);
int black_move(board_t, char, int, int, int, int);
int white_move(board_t, char, int, int, int, int);
void copy_board(board_t, board_t, int);
node_t* create_tree(board_t, int);
int cell_validity(int, int);
void create_children_node(node_t*, int, int);
int next_action(node_t*, int, board_t);
move_t calculate_move(board_t, board_t, int);
void fill_node(node_t*, board_t, int*, int*);
int run_next_action(node_t* root, int* action_num, board_t board);
void free_memory(node_t*);
void calculate_depthn_cost(node_t*, int, int, int*, int*);
int depthn_cost_ifWithNoChildren(node_t*, int, int);
void fill_move_from(move_t*, int, char);
void fill_move_to(move_t*, int, char);

int
main(int argc, char* argv[]) {
    // YOUR IMPLEMENTATION OF STAGES 0-2   
    /* Initialise the board */
    board_t board;
    /* Define the coordinators of the start point and the destination of the
    checker on the designed board, where 'num_index' is x coordinator and
    'char_index' is y coordinator */
    char command = 0;
    int action_num = 0;
    move_t move = { 0,0,0,0 };
    node_t* root = { 0 };
    initialise_board(board);
    print_board(board, move, action_num);
    while (scanf("%c%d-%c%d", &move.from_col, &move.from_row, &move.to_col,
        &move.to_row) == MOVE_ELEMENTS) {
        /* Jump to the next line after newline character */
        getchar();
        action_num++;
        /* Check the validity of input move or capture, if valid, move the checker and print the new board */
        if (!manipulate_board(board, move, action_num)) {
            error_report(board, move, action_num);
            return 0;
        }
        /* Print the board */
        printf(BOARD_SEPERATOR);
        print_board(board, move, action_num);
    }

    action_num++;
    /* Read the command character 'A' or 'P' */
    command = move.from_col;
    /* Run stage1 if the command is A */
    if (command == 'A') {
        root = create_tree(board, action_num);
        if (!run_next_action(root, &action_num, board)) {
            return 0;
        }
    }

    /* Run stage1 if the command is P */
    if (command == 'P') {
        for (int i = 0; i < MAX_ACTION_NUM; i++) {
            root = create_tree(board, action_num);
            if (!run_next_action(root, &action_num, board)) {
                return 0;
            }
        }
    }

    return EXIT_SUCCESS;            // exit program with the success code
}

/* The function initialize the board */
void initialise_board(board_t board) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            /* Fill white pieces on the board */
            if (((row == ROW_1 || row == ROW_3) && (col % SIDE_DIVIDER ==
                ODD_COL)) || ((row == ROW_2) && (col % SIDE_DIVIDER ==
                    EVEN_COL))) {
                board[row][col] = WPIECE;
            }
            /* Fill black pieces on the board */
            else if (((row == ROW_6 || row == ROW_8) && (col % SIDE_DIVIDER
                == EVEN_COL)) || ((row == ROW_7)
                    && (col % SIDE_DIVIDER == ODD_COL))) {
                board[row][col] = BPIECE;
            }
            /* Fill empty cell */
            else {
                board[row][col] = EMPTY_CELL;
            }
        }
    }
    printf("BOARD SIZE: 8x8\n#BLACK PIECES: 12\n#WHITE PIECES: 12\n");
}

/* The function report the type of error if there is a illegal move on the board */
void error_report(board_t board, move_t move, int action_num) {
    int from_col_index = move.from_col - FIRST_COL,
        to_col_index = move.to_col - FIRST_COL,
        from_row_index = move.from_row - INDEX_DEDUCTION,
        to_row_index = move.to_row - INDEX_DEDUCTION;
    /* Source cell is outside of the board */
    if ((from_col_index < LEFT_BORDER || from_col_index >= BOARD_SIZE) ||
        (from_row_index < LEFT_BORDER || from_row_index >= BOARD_SIZE)) {
        printf("ERROR: Source cell is outside of the board.\n");
    }
    /* Target cell is outside of the board */
    else if ((to_col_index < LEFT_BORDER || to_col_index >= BOARD_SIZE) ||
        (to_row_index < LEFT_BORDER || to_row_index >= BOARD_SIZE)) {
        printf("ERROR: Target cell is outside of the board.\n");
    }
    /* Source cell is empty */
    else if (board[from_row_index][from_col_index] == EMPTY_CELL) {
        printf("ERROR: Source cell is empty.\n");
    }
    /* Target cell is not empty */
    else if (board[to_row_index][to_col_index] != EMPTY_CELL) {
        printf("ERROR: Target cell is not empty.\n");
    }
    /* Source cell holds opponent's piece/tower. */
    else if (((action_num % SIDE_DIVIDER == BLACK &&
        board[from_row_index][from_col_index] == WPIECE) ||
        (action_num % SIDE_DIVIDER == BLACK &&
            board[from_row_index][from_col_index] == WTOWER)) ||
        ((action_num % SIDE_DIVIDER == WHITE &&
            board[from_row_index][from_col_index] == BPIECE) ||
            (action_num % SIDE_DIVIDER == WHITE &&
                board[from_row_index][from_col_index] == BTOWER))) {
        printf("ERROR: Source cell holds opponent's piece/tower.\n");
    }
    /* All other illegal actions */
    else {
        printf("ERROR: Illegal action.\n");
    }
}

/* The function manipulate the board after a capture is made */
void capture_action(board_t board, move_t move, char checker_type,
    int capture_row, int capture_col) {
    int from_col_index = move.from_col - FIRST_COL,
        to_col_index = move.to_col - FIRST_COL,
        from_row_index = move.from_row - INDEX_DEDUCTION,
        to_row_index = move.to_row - INDEX_DEDUCTION;
    /* Manipulate the board */
    board[from_row_index][from_col_index] = EMPTY_CELL;
    board[to_row_index][to_col_index] = checker_type;
    board[capture_row][capture_col] = EMPTY_CELL;
}

/* The function manipulate the board after a move instruction is made */
int manipulate_board(board_t board, move_t move, int action_num) {
    int from_col_index = move.from_col - FIRST_COL,
        to_col_index = move.to_col - FIRST_COL,
        from_row_index = move.from_row - INDEX_DEDUCTION,
        to_row_index = move.to_row - INDEX_DEDUCTION,
        capture_row = (from_row_index + to_row_index) / CAPTURE_DIVIDER,
        capture_col = (from_col_index + to_col_index) / CAPTURE_DIVIDER;
    char checker_type = board[from_row_index][from_col_index];
    /* Check is the movement is a valid move */
    if ((cell_validity(from_row_index, from_col_index) &&
        cell_validity(to_row_index, to_col_index))
        && board[to_row_index][to_col_index] == EMPTY_CELL) {
        if (action_num % SIDE_DIVIDER == BLACK) {
            /* Manipulate the board if black piece/tower move */
            if (abs(from_col_index - to_col_index) == MOVE) {
                if ((checker_type == BPIECE && from_row_index -
                    to_row_index == MOVE) || (checker_type == BTOWER &&
                        abs(from_row_index - to_row_index) == MOVE)) {
                    board[from_row_index][from_col_index] = EMPTY_CELL;
                    board[to_row_index][to_col_index] = checker_type;
                    if (to_row_index == TOP_ROW && checker_type ==
                        BPIECE) {
                        board[to_row_index][to_col_index] = BTOWER;
                    }

                    return VALID;
                }
            }

            /* Manipulate the board is black piece/tower capture white piece/tower */
            else if (abs(from_col_index - to_col_index) == CAPTURE) {
                if (((checker_type == BPIECE && from_row_index - to_row_index
                    == CAPTURE) || ((checker_type == BTOWER &&
                        abs(from_row_index - to_row_index) == CAPTURE))) &&
                    (board[capture_row][capture_col] == WPIECE ||
                        board[capture_row][capture_col] == WTOWER)) {
                    capture_action(board, move, checker_type, capture_row,
                        capture_col);
                    if (to_row_index == TOP_ROW && checker_type == BPIECE) {
                        board[to_row_index][to_col_index] = BTOWER;
                    }

                    return VALID;
                }
            }
        }

        /* Do the same thing if it is a white piece/tower */
        if (action_num % SIDE_DIVIDER == WHITE) {
            if (abs(from_col_index - to_col_index) == MOVE) {
                if ((checker_type == WPIECE && from_row_index - to_row_index ==
                    -MOVE) || (checker_type == WTOWER &&
                        abs(from_row_index - to_row_index) == MOVE)) {
                    board[from_row_index][from_col_index] = EMPTY_CELL;
                    board[to_row_index][to_col_index] = checker_type;
                    if (to_row_index == BOTTOM_ROW && checker_type == WPIECE) {
                        board[to_row_index][to_col_index] = WTOWER;
                    }

                    return VALID;
                }
            }

            else if (abs(from_col_index - to_col_index) == CAPTURE) {
                if (((checker_type == WPIECE && from_row_index - to_row_index
                    == -CAPTURE) || (checker_type == WTOWER &&
                        abs(from_row_index - to_row_index) == CAPTURE))
                    && (board[capture_row][capture_col] == BPIECE ||
                        board[capture_row][capture_col] == BTOWER)) {
                    capture_action(board, move, checker_type, capture_row,
                        capture_col);
                    if (to_row_index == BOTTOM_ROW && checker_type == WPIECE) {
                        board[to_row_index][to_col_index] = WTOWER;
                    }

                    return VALID;
                }
            }
        }
    }
    return INVALID;
}

/* The function calculate the cost of current board */
int board_cost(board_t board) {
    int cost_value = 0;
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col] == BPIECE) {
                cost_value = cost_value + COST_PIECE;
            }

            else if (board[row][col] == BTOWER) {
                cost_value = cost_value + COST_TOWER;
            }

            else if (board[row][col] == WPIECE) {
                cost_value = cost_value - COST_PIECE;
            }

            else if (board[row][col] == WTOWER) {
                cost_value = cost_value - COST_TOWER;
            }
        }
    }

    return cost_value;
}


void print_board(board_t board, move_t move, int action_num) {
    int cost_value = 0;
    cost_value = board_cost(board);
    // do nothing if action number is 0
    if (action_num == START) {}
    /* Print the move information */
    else {
        if (action_num % SIDE_DIVIDER == BLACK) {
            printf("BLACK ACTION #%d: %c%d-%c%d\n", action_num, move.from_col,
                move.from_row, move.to_col, move.to_row);
        }

        else {
            printf("WHITE ACTION #%d: %c%d-%c%d\n", action_num, move.from_col,
                move.from_row, move.to_col, move.to_row);
        }

        printf("BOARD COST: %d\n", cost_value);
    }

    /* Print the board */
    printf(FIRST_ROW);
    for (int row = 0; row < BOARD_SIZE; row++) {
        printf(ROW_SEPERATOR);
        printf(" %d |", row + INDEX_DEDUCTION);
        for (int col = 0; col < BOARD_SIZE; col++) {
            printf(" %c |", board[row][col]);
        }
    }

    printf(ROW_SEPERATOR);
}

/* The function move the particular black piece/tower on a given direction and manipulate the board if it is a valid move/capture */
int black_move(board_t board, char checker_type, int row,
    int col, int row_direction, int col_direction) {
    int tar_row_move = row + row_direction, tar_col_move = col + col_direction,
        tar_row_capture = row + 2 * row_direction,
        tar_col_capture = col + 2 * col_direction;
    /* Check the validity for a black move */
    if (cell_validity(tar_row_move, tar_col_move) &&
        (board[tar_row_move][tar_col_move] == EMPTY_CELL)) {
        /* manipulate the board for a black move */
        board[row][col] = EMPTY_CELL;
        if (tar_row_move == TOP_ROW) {
            board[tar_row_move][tar_col_move] = BTOWER;
        }

        else {
            board[tar_row_move][tar_col_move] = checker_type;
        }

        return VALID;
    }

    /* Check the validity for a black capture */
    else if (((board[tar_row_move][tar_col_move] == WPIECE ||
        board[tar_row_move][tar_col_move] == WTOWER) &&
        board[tar_row_capture][tar_col_capture] == EMPTY_CELL)
        && cell_validity(tar_row_capture, tar_col_capture)) {
        /* manipulate the board for a black capture */
        board[row][col] = EMPTY_CELL;
        board[tar_row_move][tar_col_move] = EMPTY_CELL;
        if (tar_row_capture == TOP_ROW) {
            board[tar_row_capture][tar_col_capture] = BTOWER;
        }

        else {
            board[tar_row_capture][tar_col_capture] = checker_type;
        }

        return VALID;
    }

    return INVALID;
}

/* The function move the particular white piece/tower on a given direction and manipulate the board if it is a valid move/capture */
int white_move(board_t board, char checker_type, int row,
    int col, int row_direction, int col_direction) {
    /* Check the validity for a white move */
    int tar_row_move = row + row_direction, tar_col_move = col + col_direction,
        tar_row_capture = row + CAPTURE_FACTOR * row_direction,
        tar_col_capture = col + CAPTURE_FACTOR * col_direction;
    if (cell_validity(tar_row_move, tar_col_move) &&
        (board[tar_row_move][tar_col_move] == EMPTY_CELL)) {
        /* manipulate the board for a white move */
        board[row][col] = EMPTY_CELL;
        if (tar_row_move == BOTTOM_ROW) {
            board[tar_row_move][tar_col_move] = WTOWER;
        }

        else {
            board[tar_row_move][tar_col_move] = checker_type;
        }

        return VALID;
    }

    /* Check the validity for a white capture */
    else if (((board[tar_row_move][tar_col_move] == BPIECE ||
        board[tar_row_move][tar_col_move] == BTOWER) &&
        board[tar_row_capture][tar_col_capture] == EMPTY_CELL)
        && cell_validity(tar_row_capture, tar_col_capture)) {
        board[row][col] = EMPTY_CELL;
        board[tar_row_move][tar_col_move] = EMPTY_CELL;
        if (tar_row_capture == BOTTOM_ROW) {
            board[tar_row_capture][tar_col_capture] = WTOWER;
        }
        /* manipulate the board for a white capture */
        else {
            board[tar_row_capture][tar_col_capture] = checker_type;
        }

        return VALID;
    }

    return INVALID;
}

/* The function check if a cell is located within the board*/
int cell_validity(int row, int col) {
    if (row >= 0 && row < BOARD_SIZE) {
        if (col >= 0 && col < BOARD_SIZE) {
            return VALID;
        }
    }
    return INVALID;
}

/* The function copy the board to the target board*/
void copy_board(board_t sou_board, board_t tar_board, int size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            tar_board[row][col] = sou_board[row][col];
        }
    }
}

/* The function fill the attributes of a given node */
void fill_node(node_t* parent_node, board_t parent_board, int* nth_children,
    int* capacity) {
    node_t* node = (node_t*)malloc(sizeof(node_t));
    assert(node != NULL);
    copy_board(parent_board, node->board, BOARD_SIZE);
    node->parent = parent_node;
    node->cost = board_cost(node->board);
    parent_node->children[*nth_children] = *node;
    *nth_children = *nth_children + ADD_CHILD;
    copy_board(parent_node->board, parent_board, BOARD_SIZE);
    free(node);
    node = NULL;
    /* Increase the size of children if the capacity is full */
    if (*nth_children >= *capacity) {
        *capacity = (*capacity) * EXPAND_FACTOR;
        parent_node->children = (node_t*)realloc(parent_node->children,
            (*capacity) * sizeof(node_t));
    }
}

/* The function create all its children node for a given parent node */
void create_children_node(node_t* parent_node,
    int capacity, int action_num) {

    board_t parent_board;
    copy_board(parent_node->board, parent_board, BOARD_SIZE);
    int nth_children = NO_CHILD;
    node_t* offsprings = (node_t*)malloc(capacity * sizeof(node_t));
    parent_node->children = offsprings;
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if ((parent_board[row][col] == WPIECE || parent_board[row][col] ==
                WTOWER) && action_num % SIDE_DIVIDER == WHITE) {
                if (parent_board[row][col] == WTOWER) {
                    /* Check if tower can go north east */
                    if (white_move(parent_board, parent_board[row][col],
                        row, col, NORTH, EAST)) {
                        fill_node(parent_node, parent_board, &nth_children,
                            &capacity);
                    }
                }
                /* Check if piece/tower can go south east */
                if (white_move(parent_board, parent_board[row][col],
                    row, col, SOUTH, EAST)) {
                    fill_node(parent_node, parent_board, &nth_children,
                        &capacity);
                }
                /* Check if piece/tower can go south west */
                if (white_move(parent_board, parent_board[row][col],
                    row, col, SOUTH, WEST)) {
                    fill_node(parent_node, parent_board, &nth_children,
                        &capacity);
                }
                /* Check if tower can go north west */
                if (parent_board[row][col] == WTOWER) {
                    if (white_move(parent_board, parent_board[row][col],
                        row, col, NORTH, WEST)) {
                        fill_node(parent_node, parent_board, &nth_children,
                            &capacity);
                    }
                }
            }

            // Black
            if ((parent_board[row][col] == BPIECE || parent_board[row][col] ==
                BTOWER) && action_num % SIDE_DIVIDER == BLACK) {
                /* Check if piece/tower can go north east */
                if (black_move(parent_board, parent_board[row][col],
                    row, col, NORTH, EAST)) {
                    fill_node(parent_node, parent_board, &nth_children,
                        &capacity);
                }

                if (parent_board[row][col] == BTOWER) {
                    /* Check if tower can go south east */
                    if (black_move(parent_board, parent_board[row][col],
                        row, col, SOUTH, EAST)) {
                        fill_node(parent_node, parent_board, &nth_children,
                            &capacity);
                    }
                    /* Check if tower can go south west */
                    if (black_move(parent_board, parent_board[row][col],
                        row, col, SOUTH, WEST)) {
                        fill_node(parent_node, parent_board, &nth_children,
                            &capacity);
                    }
                }
                /* Check if tower can go north west */
                if (black_move(parent_board, parent_board[row][col],
                    row, col, NORTH, WEST)) {
                    fill_node(parent_node, parent_board, &nth_children,
                        &capacity);
                }
            }
        }
    }
    parent_node->children_num = nth_children;
}

/* The function create the three-depth tree */
node_t* create_tree(board_t board, int action_num) {
    /* Initialize the root node */
    int cap = INITIAL_CAPACITY;
    node_t* child_node_depth1, * child_node_depth2;
    node_t* root = (node_t*)malloc(sizeof(node_t));
    assert(root != NULL);
    copy_board(board, root->board, BOARD_SIZE);
    root->parent = NULL;
    /* Create tree depth 1 */
    create_children_node(root, cap, action_num);

    /* Create tree depth 2 */
    for (int i = 0; i < root->children_num; i++) {
        child_node_depth1 = (root->children) + i;
        create_children_node(child_node_depth1, cap, action_num + NEXT_ACTION);
        /* Create tree depth 3 */
        for (int j = 0; j < child_node_depth1->children_num; j++) {
            child_node_depth2 = (child_node_depth1->children) + j;
            create_children_node(child_node_depth2, cap, action_num +
                NEXT_TWO_ACTION);
        }
    }
    return root;
}

/* The function fill the from_row and from_col attribute of a move object */
void fill_move_from(move_t* move, int row, char col) {
    int row_display;
    row_display = row + INDEX_DEDUCTION;
    move->from_row = row_display;
    move->from_col = FIRST_COL + col;
}

/* The function fill the to_row and to_col attribute of a move object */
void fill_move_to(move_t* move, int row, char col) {
    int row_display;
    row_display = row + INDEX_DEDUCTION;
    move->to_row = row_display;
    move->to_col = FIRST_COL + col;
}

/* The function calculate the move being conducted by given two boards */
move_t calculate_move(board_t from_board, board_t to_board, int action_num) {
    move_t move;
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (action_num % SIDE_DIVIDER == BLACK) {
                /* If the cell is occupied at first and empty at last, the piece move from the cell*/
                if ((from_board[row][col] == BPIECE || from_board[row][col]
                    == BTOWER) && (to_board[row][col] == EMPTY_CELL)) {
                    fill_move_from(&move, row, col);
                }
                /* If the cell is empty at first and get occupied at last, the piece move to the cell*/
                else if ((from_board[row][col] == EMPTY_CELL) &&
                    (to_board[row][col] == BPIECE || to_board[row][col] ==
                        BTOWER)) {
                    fill_move_to(&move, row, col);
                }
            }

            /* Same logic for white piece/tower */
            if (action_num % SIDE_DIVIDER == WHITE) {
                if ((from_board[row][col] == WPIECE || from_board[row][col]
                    == WTOWER) && (to_board[row][col] == EMPTY_CELL)) {
                    fill_move_from(&move, row, col);
                }

                else if ((from_board[row][col] == EMPTY_CELL) &&
                    (to_board[row][col] == WPIECE || to_board[row][col] ==
                        WTOWER)) {
                    fill_move_to(&move, row, col);
                }
            }
        }
    }
    return move;
}

/* The function calculate the best next move for the current board */
int next_action(node_t* root, int action_num, board_t board) {
    node_t* child_node_depth1, * child_node_depth2;
    int depth1_max_cost = INT_MIN, max_cost = INT_MIN,
        min_cost = INT_MAX, depth1_min_cost = INT_MAX;
    board_t next_move = {{0}};
    move_t move;
    if (root->children_num != NO_CHILD) {
        for (int i = 0; i < root->children_num; i++) {
            child_node_depth1 = (root->children) + i;
            /* If depth 1 node have no child, fill the node cost with the corresponding INT_MAX or INT_MIN value */
            if (depthn_cost_ifWithNoChildren(child_node_depth1, DEPTH_1,
                action_num)) {
            }

            /* Otherwise, find the node cost for all depth 2 nodes */
            else {
                for (int j = 0; j < child_node_depth1->children_num; j++) {
                    child_node_depth2 = (child_node_depth1->children) + j;
                    max_cost = INT_MIN;
                    min_cost = INT_MAX;
                    /* If depth 2 node have no child, fill the node cost with the corresponding INT_MAX or INT_MIN value */
                    if (depthn_cost_ifWithNoChildren(child_node_depth1,
                        DEPTH_1, action_num)) {
                    }

                    /* Otherwise, fill the depth 2 node cost with the largest depth 3 node cost if it's black turn,
                       and smallest depth 3 node cost if it's white turn*/
                    else {
                        calculate_depthn_cost(child_node_depth2, DEPTH_2,
                            action_num, &min_cost, &max_cost);
                    }
                    free_memory(child_node_depth2->children);
                }
            }
        }

        for (int i = 0; i < root->children_num; i++) {
            child_node_depth1 = (root->children) + i;
            min_cost = INT_MAX;
            max_cost = INT_MIN;
            /* If depth 1 node have children, fill fill the depth 1 node cost with the smallest depth 2 node cost if it's black turn,
                       and largest depth 2 node cost if it's white turn */
            if (child_node_depth1->children_num != NO_CHILD) {
                calculate_depthn_cost(child_node_depth1, DEPTH_1, action_num,
                    &min_cost, &max_cost);
            }
            free_memory(child_node_depth1->children);
        }

        /* Find the the best next move by camparing costs in depth 1 node, store the board into next_move */
        for (int i = 0; i < root->children_num; i++) {
            child_node_depth1 = (root->children) + i;
            if (action_num % SIDE_DIVIDER == BLACK) {
                if (child_node_depth1->cost > depth1_max_cost) {
                    depth1_max_cost = child_node_depth1->cost;
                    copy_board(child_node_depth1->board, next_move, BOARD_SIZE);
                }
            }

            else {
                if (child_node_depth1->cost < depth1_min_cost) {
                    depth1_min_cost = child_node_depth1->cost;
                    copy_board(child_node_depth1->board, next_move, BOARD_SIZE);
                }
            }
        }

        /* Free memory and print the next move */
        free_memory(root->children);
        move = calculate_move(board, next_move, action_num);
        copy_board(next_move, board, BOARD_SIZE);
        printf(BOARD_SEPERATOR);
        printf("*** ");
        print_board(board, move, action_num);
        free_memory(root);
        return VALID;
    }

    free_memory(root);
    return INVALID;
}

/* The function implement next move function and check if the winner is decided */
int run_next_action(node_t* root, int* action_num, board_t board) {
    if (!next_action(root, *action_num, board)) {
        if (*action_num % SIDE_DIVIDER == BLACK) {
            printf("WHITE WIN!\n");
        }
        else {
        }

        return INVALID;
    }

    *action_num = *action_num + NEXT_ACTION;
    return VALID;
}

/* The function fill the node cost of a certain depth if the node has no child*/
int depthn_cost_ifWithNoChildren(node_t* node_depthn, int depth_num,
    int action_num) {
    if (node_depthn->children_num == NO_CHILD) {
        /* When it is depth 2 node */
        if (action_num % SIDE_DIVIDER == BLACK) {
            /* Fill the node cost with the INT_MIN when it is black turn and INT_MAX when it is white turn */
            if (depth_num % SIDE_DIVIDER == EVEN_DEPTH) {
                node_depthn->cost = INT_MIN;
            }
            else {
                node_depthn->cost = INT_MAX;
            }
        }
        /* When it is depth 1 node */
        else {
            /* Fill the node cost with the INT_MIN when it is white turn and INT_MAX when it is black turn */
            if (depth_num % SIDE_DIVIDER == EVEN_DEPTH) {
                node_depthn->cost = INT_MAX;
            }
            else {
                node_depthn->cost = INT_MIN;
            }
        }
        return VALID;
    }
    return INVALID;
}

/* The function fill the node cost of a certain depth if the node has children*/
void calculate_depthn_cost(node_t* node_depthn, int depth_num, int action_num,
    int* min_cost, int* max_cost) {
    node_t* child_node;
    for (int i = 0; i < node_depthn->children_num; i++) {
        child_node = (node_depthn->children) + i;
        /* When it is black turn*/
        if (action_num % SIDE_DIVIDER == BLACK) {
            /* For node in depth 2, fill the depth node cost with the biggest node cost in the next depth */
            if (depth_num % SIDE_DIVIDER == EVEN_DEPTH) {
                if (child_node->cost > *max_cost) {
                    *max_cost = child_node->cost;
                    node_depthn->cost = *max_cost;
                }
            }
            /* For node in depth 1, fill the depth node cost with the smallest node cost in the next depth */
            else {
                if (child_node->cost < *min_cost) {
                    *min_cost = child_node->cost;
                    node_depthn->cost = *min_cost;
                }
            }
        }
        /* When it is white turn */
        else {
            /* For node in depth 2, fill the depth node cost with the smallest node cost in the next depth */
            if (depth_num % SIDE_DIVIDER == EVEN_DEPTH) {
                if (child_node->cost < *min_cost) {
                    *min_cost = child_node->cost;
                    node_depthn->cost = *min_cost;
                }
            }
            /* For node in depth 2, fill the depth node cost with the biggest node cost in the next depth */
            else {
                if (child_node->cost > *max_cost) {
                    *max_cost = child_node->cost;
                    node_depthn->cost = *max_cost;
                }
            }
        }
    }
}

/* The function free memory for a node */
void free_memory(node_t* node) {
    free(node);
    node = NULL;
}

/* algorithms are fun! */

/* THE END -------------------------------------------------------------------*/
