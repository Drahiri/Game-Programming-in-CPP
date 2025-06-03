// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Board.h"

#include "Random.h"

BoardState::BoardState() {
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 7; j++) {
            mBoard[i][j] = Empty;
        }
    }
}

std::vector<BoardState*> BoardState::GetPossibleMoves(SquareState player) const {
    std::vector<BoardState*> retVal;

    // For each column, find if a move is possible
    for(int col = 0; col < 7; col++) {
        for(int row = 5; row >= 0; row--) {
            if(mBoard[row][col] == BoardState::Empty) {
                retVal.emplace_back(new BoardState(*this));
                retVal.back()->mBoard[row][col] = player;
                break;
            }
        }
    }

    return retVal;
}

bool BoardState::IsTerminal() const {
    // Is the board full?
    if(IsFull()) {
        return true;
    }

    // Is there a four-in-a-row?
    int fourInRow = GetFourInARow();
    if(fourInRow != 0) {
        return true;
    }

    return false;
}

float BoardState::GetScore() const {
    // If the board is full, the score is 0
    if(IsFull()) {
        return 0.0f;
    }

    // Is there a four-in-a-row?
    int fourInRow = GetFourInARow();
    if(fourInRow != 0) {
        return static_cast<float>(fourInRow);
    }

    return CalculateHeuristic();
}

bool BoardState::IsFull() const {
    bool isFull = true;
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 7; j++) {
            if(mBoard[i][j] == Empty) {
                isFull = false;
            }
        }
    }

    return isFull;
}

int BoardState::GetFourInARow() const {
    // Returns -1 if yellow wins, 1 if red wins, 0 otherwise

    // Check if there's a row with four in a row
    for(int row = 0; row < 6; row++) {
        for(int col = 0; col < 4; col++) {
            if(mBoard[row][col] == mBoard[row][col + 1] && mBoard[row][col] == mBoard[row][col + 2]
                  && mBoard[row][col] == mBoard[row][col + 3]) {
                if(mBoard[row][col] == BoardState::Yellow) {
                    return -1;
                } else if(mBoard[row][col] == BoardState::Red) {
                    return 1;
                }
            }
        }
    }

    // Check if there's a column with four in a row
    for(int col = 0; col < 7; col++) {
        for(int row = 0; row < 3; row++) {
            if(mBoard[row][col] == mBoard[row + 1][col] && mBoard[row][col] == mBoard[row + 2][col]
                  && mBoard[row][col] == mBoard[row + 3][col]) {
                if(mBoard[row][col] == BoardState::Yellow) {
                    return -1;
                } else if(mBoard[row][col] == BoardState::Red) {
                    return 1;
                }
            }
        }
    }

    // Check if there's a right diagonal four in a row
    for(int col = 0; col < 4; col++) {
        for(int row = 0; row < 3; row++) {
            if(mBoard[row][col] == mBoard[row + 1][col + 1]
                  && mBoard[row][col] == mBoard[row + 2][col + 2]
                  && mBoard[row][col] == mBoard[row + 3][col + 3]) {
                if(mBoard[row][col] == BoardState::Yellow) {
                    return -1;
                } else if(mBoard[row][col] == BoardState::Red) {
                    return 1;
                }
            }
        }
    }

    // Check if there's a left diagonal for in a row
    for(int col = 0; col < 4; col++) {
        for(int row = 3; row < 6; row++) {
            if(mBoard[row][col] == mBoard[row - 1][col + 1]
                  && mBoard[row][col] == mBoard[row - 2][col + 2]
                  && mBoard[row][col] == mBoard[row - 3][col + 3]) {
                if(mBoard[row][col] == BoardState::Yellow) {
                    return -1;
                } else if(mBoard[row][col] == BoardState::Red) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

float BoardState::CalculateHeuristic() const {
    // Checks how many same colors are in line
    float heuristic = 0.0f;

    // Check rows
    for(int row = 0; row < 6; row++) {
        for(int col = 0; col < 4; col++) {
            float currentVal = 0.0f;
            for(int offset = 0; offset < 3; offset++) {
                switch(mBoard[row][col + offset]) {
                case SquareState::Red:
                    currentVal += 1;
                    break;
                case SquareState::Yellow:
                    currentVal -= 1;
                    break;
                default:
                    break;
                }
            }
            heuristic += currentVal;
        }
    }

    // Check colums
    for(int col = 0; col < 7; col++) {
        for(int row = 0; row < 3; row++) {
            float currentVal = 0.0f;
            for(int offset = 0; offset < 3; offset++) {
                switch(mBoard[row + offset][col]) {
                case SquareState::Red:
                    currentVal += 1;
                    break;
                case SquareState::Yellow:
                    currentVal -= 1;
                    break;
                default:
                    break;
                }
            }
            heuristic += currentVal;
        }
    }

    // Check right diagonal
    for(int col = 0; col < 4; col++) {
        for(int row = 0; row < 3; row++) {
            float currentVal = 0.0f;
            for(int offset = 0; offset < 3; offset++) {
                switch(mBoard[row + offset][col + offset]) {
                case SquareState::Red:
                    currentVal += 1;
                    break;
                case SquareState::Yellow:
                    currentVal -= 1;
                    break;
                default:
                    break;
                }
            }
            heuristic += currentVal;
        }
    }

    // Check left diagonal
    for(int col = 0; col < 4; col++) {
        for(int row = 3; row < 6; row++) {
            float currentVal = 0.0f;
            for(int offset = 0; offset < 3; offset++) {
                switch(mBoard[row - offset][col - offset]) {
                case SquareState::Red:
                    currentVal += 1;
                    break;
                case SquareState::Yellow:
                    currentVal -= 1;
                    break;
                default:
                    break;
                }
            }
            heuristic += currentVal;
        }
    }

    return heuristic;
}

bool TryPlayerMove(BoardState* state, int column) {
    // Find the first row in that column that's available
    // (if any)
    for(int row = 5; row >= 0; row--) {
        if(state->mBoard[row][column] == BoardState::Empty) {
            state->mBoard[row][column] = BoardState::Yellow;
            return true;
        }
    }

    return false;
}

const BoardState* alphaBetaDecide(const BoardState* root, int maxDepth);

void CPUMove(BoardState* state) {
    // // For now, this just randomly picks one of the possible moves
    // std::vector<BoardState*> moves = state->GetPossibleMoves(BoardState::Red);

    // int index = Random::GetIntRange(0, moves.size() - 1);

    // *state = *moves[index];

    // // Clear up memory from possible moves
    // for(auto state: moves) {
    //     delete state;
    // }

    *state = *alphaBetaDecide(state, 5);
}

float alphaBetaMin(const BoardState* node, int depth, float alpha, float beta);

const BoardState* alphaBetaDecide(const BoardState* root, int maxDepth) {
    const BoardState* choice = nullptr;

    float maxValue = -std::numeric_limits<float>::infinity();
    float beta = std::numeric_limits<float>::infinity();
    for(const BoardState* child: root->GetPossibleMoves(BoardState::Red)) {
        float v = alphaBetaMin(child, maxDepth - 1, maxValue, beta);
        if(v > maxValue) {
            maxValue = v;
            choice = child;
        }
    }
    return choice;
}

float alphaBetaMax(const BoardState* node, int depth, float alpha, float beta) {
    if(depth == 0 || node->IsTerminal()) {
        return node->GetScore();
    }
    float maxValue = -std::numeric_limits<float>::infinity();
    for(const BoardState* child: node->GetPossibleMoves(BoardState::Yellow)) {
        maxValue = std::max(maxValue, alphaBetaMin(child, depth - 1, alpha, beta));
        if(maxValue >= beta) {
            break; // Beta prune
        }
        alpha = std::max(maxValue, alpha); // Increase lower bound
    }
    return maxValue;
}

float alphaBetaMin(const BoardState* node, int depth, float alpha, float beta) {
    if(depth == 0 || node->IsTerminal()) {
        return node->GetScore();
    }

    float minValue = std::numeric_limits<float>::infinity();
    for(const BoardState* child: node->GetPossibleMoves(BoardState::Red)) {
        minValue = std::min(minValue, alphaBetaMax(child, depth - 1, alpha, beta));
        if(minValue <= alpha) {
            break; // Alpha prune
        }
        beta = std::min(minValue, beta); // Decrease upper bound
    }
    return minValue;
}