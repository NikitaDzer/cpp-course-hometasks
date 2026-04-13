#include <iostream>
#include <memory>
#include <queue>
#include <stack>

class Tree {
public:
    struct Node {
        int data;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::weak_ptr<Node> parent;

        Node(int val) : data(val) {
            std::cout << "Node created: " << data << std::endl;
        }

        ~Node() {
            std::cout << "Node deleted: " << data << std::endl;
        }
    };

    std::shared_ptr<Node> root;

    void traverse_v1() {
        if (!root) return;
        std::queue<std::shared_ptr<Node>> q;
        q.push(root);
        std::cout << "BFS (v1): ";
        while (!q.empty()) {
            std::shared_ptr<Node> current = q.front();
            q.pop();
            std::cout << current->data << " ";
            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
        std::cout << std::endl;
    }

    void traverse_v2() {
        if (!root) return;
        std::stack<std::shared_ptr<Node>> s;
        s.push(root);
        std::cout << "DFS (v2): ";
        while (!s.empty()) {
            std::shared_ptr<Node> current = s.top();
            s.pop();
            std::cout << current->data << " ";
            if (current->right) s.push(current->right);
            if (current->left) s.push(current->left);
        }
        std::cout << std::endl;
    }
};

int main() {
    Tree tree;
    
    tree.root = std::make_shared<Tree::Node>(1);
    
    tree.root->left = std::make_shared<Tree::Node>(2);
    tree.root->right = std::make_shared<Tree::Node>(3);
    tree.root->left->parent = tree.root;
    tree.root->right->parent = tree.root;

    tree.root->left->left = std::make_shared<Tree::Node>(4);
    tree.root->left->right = std::make_shared<Tree::Node>(5);
    tree.root->left->left->parent = tree.root->left;
    tree.root->left->right->parent = tree.root->left;

    tree.root->right->left = std::make_shared<Tree::Node>(6);
    tree.root->right->right = std::make_shared<Tree::Node>(7);
    tree.root->right->left->parent = tree.root->right;
    tree.root->right->right->parent = tree.root->right;

    tree.traverse_v1();
    tree.traverse_v2();

    return 0;
}