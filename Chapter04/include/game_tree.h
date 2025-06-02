// All codes for this part are representation of Tic Tac Toe
// as to not leave it empty
#ifndef GAME_TREE_H
#define GAME_TREE_H

#include <vector>

struct GameState {
    enum class SquareState {
        Empty,
        X,
        O
    };
    SquareState board[3][3];
};

struct GTNode {
    // Children nodes
    std::vector<GTNode*> children;

    // State of game at this node
    GameState state;
};

float getScore(const GameState state);

// Minimax
float maxPlayer(const GTNode* node);
float minPlayer(const GTNode* node);

const GTNode* minimaxDecide(const GTNode* root);

#endif