#include "game_tree.h"

#include <limits>

float getScore(const GameState state) {
    // Are any of the rows the same?
    for(int i = 0; i < 3; i++) {
        bool same = true;
        GameState::SquareState v = state.board[i][0];
        for(int j = 1; j < 3; j++) {
            if(state.board[i][j] != v) {
                same = false;
            }
        }

        if(same) {
            if(v == GameState::SquareState::X) {
                return 1.0f;
            } else {
                return -1.0f;
            }
        }
    }

    // Are any of the columns the same?
    for(int j = 0; j < 3; j++) {
        bool same = true;
        GameState::SquareState v = state.board[0][j];
        for(int i = 1; i < 3; i++) {
            if(state.board[i][j] != v) {
                same = false;
            }
        }

        if(same) {
            if(v == GameState::SquareState::X) {
                return 1.0f;
            } else {
                return -1.0f;
            }
        }
    }

    // What about diagonal?
    if(((state.board[0][0] == state.board[1][1]) && (state.board[1][1] == state.board[2][2]))
          || ((state.board[2][0] == state.board[1][1])
                && (state.board[1][1] == state.board[0][2]))) {
        if(state.board[1][1] == GameState::SquareState::X) {
            return 1.0f;
        } else {
            return -1.0f;
        }
    }

    // We tied
    return 0.0f;
}

float maxPlayer(const GTNode* node) {
    // If this is a leaf, return score
    if(node->children.empty()) {
        return getScore(node->state);
    }

    // Find the subtree with the maximum value
    float maxValue = -std::numeric_limits<float>::infinity();
    for(const GTNode* child: node->children) {
        maxValue = std::max(maxValue, minPlayer(child));
    }
    return maxValue;
}

float minPlayer(const GTNode* node) {
    // If this is a leaf, return score
    if(node->children.empty()) {
        return getScore(node->state);
    }

    // Find the subtree with the minimum value
    float minValue = std::numeric_limits<float>::infinity();
    for(const GTNode* child: node->children) {
        minValue = std::min(minValue, maxPlayer(child));
    }
    return minValue;
}

const GTNode* minimaxDecide(const GTNode* root) {
    // Fid the subtree with the maximum value, and save the choice
    const GTNode* choice = nullptr;

    float maxValue = -std::numeric_limits<float>::infinity();
    for(const GTNode* child: root->children) {
        float v = minPlayer(child);
        if(v > maxValue) {
            maxValue = v;
            choice = child;
        }
    }

    return choice;
}
