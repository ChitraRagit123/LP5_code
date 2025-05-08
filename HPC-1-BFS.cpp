// bfs.cpp
#include <iostream>
#include <queue>
#include <omp.h>
using namespace std;

class Node {
public:
    Node *left, *right;
    int data;
};

Node* insert(Node* root, int data) {
    if (!root) {
        root = new Node;
        root->left = root->right = NULL;
        root->data = data;
        return root;
    }

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* temp = q.front();
        q.pop();

        if (temp->left == NULL) {
            temp->left = new Node;
            temp->left->left = temp->left->right = NULL;
            temp->left->data = data;
            return root;
        } else {
            q.push(temp->left);
        }

        if (temp->right == NULL) {
            temp->right = new Node;
            temp->right->left = temp->right->right = NULL;
            temp->right->data = data;
            return root;
        } else {
            q.push(temp->right);
        }
    }
    return root;
}

void bfs(Node* head) {
    queue<Node*> q;
    q.push(head);

    while (!q.empty()) {
        int qSize = q.size();

        #pragma omp parallel for
        for (int i = 0; i < qSize; i++) {
            Node* currNode;

            #pragma omp critical
            {
                currNode = q.front();
                q.pop();
                cout << "\t" << currNode->data;
            }

            #pragma omp critical
            {
                if (currNode->left)
                    q.push(currNode->left);
                if (currNode->right)
                    q.push(currNode->right);
            }
        }
    }
}

int main() {
    Node *root = NULL;
    int data;
    char ans;

    do {
        cout << "Enter data => ";
        cin >> data;
        root = insert(root, data);
        cout << "Do you want to insert one more node? (y/n): ";
        cin >> ans;
    } while (ans == 'y' || ans == 'Y');

    cout << "\nBFS Traversal: ";
    bfs(root);
    cout << endl;

    return 0;
}

