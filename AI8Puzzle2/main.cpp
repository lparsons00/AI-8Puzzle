/*
8 Puzzle -  creates a node structure for the actual puzzle layout, with parent info 
 * for tracking, and the heuristic cost (how many tiles are placed wrong), also need
 * the distance from the original 'root' of puzzle

 */
#include <bits/stdc++.h>

#include <stdio.h>

using namespace std;

struct Node {
    Node * parent;
    int matrix[3][3];
    int x, y;
    int cost;
    int level;
};

//creates new node from struct above
Node * newNode(int matrix[3][3], int x, int y, int newX, int newY, int level, Node * parent) {
    Node * node = new Node;
    node -> parent = parent;
    memcpy(node -> matrix, matrix, sizeof node -> matrix);
    swap(node -> matrix[x][y], node -> matrix[newX][newY]);
    node -> cost = INT_MAX;
    node -> level = level;
    node -> x = newX;
    node -> y = newY;

    return node;
}

//heap
struct comp {
    bool operator()(const Node * lhs,
        const Node * rhs) const {
        return (lhs -> cost + lhs -> level) > (rhs -> cost + rhs -> level);
    }
};

// Function to calculate the number of misplaced tiles 
// ie. number of non-blank tiles not in their goal position 
int totalCost(int first[3][3], int goal[3][3]) {
    int count = 0;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (first[i][j] && first[i][j] != goal[i][j]){
                count++;
            }
        }
    }
    return count;
}

int inside(int x, int y) {
    return (x >= 0 && x < 3 && y >= 0 && y < 3);
}


//simple print code, calls itself to ensure all matrix are printed
void print(int matrix[3][3]) {
    for (int i = 0; i < 3; i++) {
        printf("  ");
        for (int j = 0; j < 3; j++) {
            //t = ;
            if (matrix[i][j] == 0) {
                printf("_ ");
            } else {
                printf("%d ", matrix[i][j]);
            }
        }
        printf("  \n");
    }
    printf("\n    |    \n");
    printf("  \\ | /  \n");
    printf("   \\ /    \n");
}


void printPath(Node * root) {
    if (root == NULL) {
        return;
    }
    printPath(root -> parent);
    print(root -> matrix);
    printf("\n");

}



//set basic default values for 'moving' around the puzzle
int leftright[] = {1,0,-1,0};
int updown[] = {0,-1,0,1};

void solve(int first[3][3], int x, int y, int goal[3][3]) {
    priority_queue < Node * , vector < Node * > , comp > que;

    //checks cost of root (checks total distance)
    Node * root = newNode(first, x, y, x, y, 0, NULL);
    root -> cost = totalCost(first, goal);
    que.push(root);

    while (!que.empty()) {
        Node * bestMatch = que.top();
        //printf("testline1\n");
        //print(bestMatch->matrix);
        //int test = 0;
        //test = bestMatch ->cost;
        //printf("test: %d\n", test);
        que.pop();
        if (bestMatch -> cost == 0) {
            printPath(bestMatch);
            break;
        }
        for (int i = 0; i < 4; i++) {
            if (inside(bestMatch -> x + leftright[i], bestMatch -> y + updown[i])) {
                Node * child = newNode(bestMatch -> matrix, bestMatch -> x, bestMatch -> y, bestMatch -> x + leftright[i], bestMatch -> y + updown[i], bestMatch -> level + 1, bestMatch);
                child -> cost = totalCost(child -> matrix, goal);
                que.push(child);
            }
        }
    }
}

int main() {
    int first[3][3] = {
        {2,1,6},
        {4,0,8},
        {7,5,3}
    };
    int goal[3][3] = {
        {1,2,3},
        {8,0,4},
        {7,6,5}
    };
    int x = 1, y = 1;

    solve(first, x, y, goal);
    return 0;
}