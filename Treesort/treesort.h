#ifndef TREESORT_H
#define TREESORT_H

#include <algorithm>
#include <iterator>
#include <limits>

template<class T>
class Tree {
private:
    
    class TreeNode {
    public:
        T key;
        size_t count;
        int height;
        
        TreeNode* left;
        TreeNode* right;
        TreeNode* parent;
        
        TreeNode(T _key) : key   {_key},
                           count {1},
                           height{0},
                           left  {nullptr},
                           right {nullptr},
                           parent{nullptr}
        {}
    };
    
    TreeNode* root;
    
public:
    
    Tree(T initial_element)
    {
        root = new TreeNode(initial_element);
    }
    
    void add(T element)
    {
        TreeNode* parent = nullptr;
        TreeNode* node = root;
        
        while (node != nullptr)
        {
            if (element < node->key)
            {
                parent = node;
                node = node->left;
            }
            else if (element > node->key)
            {
                parent = node;
                node = node->right;
            }
            else
            {
                node->count++;
                return;
            }
        }
        
        TreeNode* newnode = new TreeNode(element);
        
        if (element < parent->key)
        {
            parent->left = newnode;
        }
        else
        {
            parent->right = newnode;
        }
        
        newnode->parent = parent;
        fixAfterInsertion(newnode);
    }
    
    void dumpContentsToArray(T* array)
    {
        TreeNode* node = getMinimum(root);
        
        while (node != nullptr)
        {
            size_t count = node->count;
            T element = node->key;
            
            for (size_t i = 0; i < count; ++i)
            {
                *array = element;
                array++;
            }
            
            node = getSuccessor(node);
        }
    }
    
private:
    
    TreeNode* getMinimum(TreeNode* node)
    {
        TreeNode* current = node;
        
        while (current->left != nullptr)
        {
            current = current->left;
        }
        
        return current;
    }
    
    TreeNode* getSuccessor(TreeNode* node) {
        if (node->right)
        {
            return getMinimum(node->right);
        }
        
        TreeNode* parent = node->parent;
        
        while (parent != nullptr && parent->right == node)
        {
            node = parent;
            parent = parent->parent;
        }
        
        return parent;
    }
    
    int getHeight(TreeNode* node)
    {
        if (node == nullptr)
        {
            return -1;
        }
        
        int leftTreeHeight = getHeight(node->left);
        
        if (leftTreeHeight == std::numeric_limits<int>::min())
        {
            return std::numeric_limits<int>::min();
        }
        
        int rightTreeHeight = getHeight(node->right);
        
        if (rightTreeHeight == std::numeric_limits<int>::min())
        {
            return std::numeric_limits<int>::min();
        }
        
        if (node->height == std::max(leftTreeHeight, rightTreeHeight) + 1)
        {
            return node->height;
        }
        
        return std::numeric_limits<int>::min();
    }
    
    bool heightsAreCorrect()
    {
        return getHeight(root) == root->height;
    }
    
    bool isBalanced(TreeNode* node)
    {
        if (node == nullptr)
        {
            return true;
        }
        
        if (!isBalanced(node->left))
        {
            return false;
        }
        
        if (!isBalanced(node->right))
        {
            return false;
        }
        
        int leftHeight  = height(node->left);
        int rightHeight = height(node->right);
        
        return std::abs(leftHeight - rightHeight) < 2;
    }
    
    bool isBalanced()
    {
        return isBalanced(root);
    }
    
public:
    
    bool isHealthy()
    {
        return heightsAreCorrect() && isBalanced();
    }
    
private:
    
    int height(TreeNode* node)
    {
        return node ? node->height : -1;
    }
    
    TreeNode* leftRotate(TreeNode* node1)
    {
        TreeNode* node2 = node1->right;
        node2->parent = node1->parent;
        node1->parent = node2;
        node1->right = node2->left;
        node2->left = node1;
        
        if (node1->right != nullptr)
        {
            node1->right->parent = node1;
        }
        
        node1->height = std::max(height(node1->left), height(node1->right)) + 1;
        node2->height = std::max(height(node2->left), height(node2->right)) + 1;
        return node2;
    }
    
    TreeNode* rightRotate(TreeNode* node1)
    {
        TreeNode* node2 = node1->left;
        node2->parent = node1->parent;
        node1->parent = node2;
        node1->left = node2->right;
        node2->right = node1;
        
        if (node1->left != nullptr)
        {
            node1->left->parent = node1;
        }
        
        node1->height = std::max(height(node1->left), height(node1->right)) + 1;
        node2->height = std::max(height(node2->left), height(node2->right)) + 1;
        return node2;
    }
    
    TreeNode* rightLeftRotate(TreeNode* node1)
    {
        TreeNode* node2 = node1->right;
        node1->right = rightRotate(node2);
        return leftRotate(node1);
    }
    
    TreeNode* leftRightRotate(TreeNode* node1)
    {
        TreeNode* node2 = node1->left;
        node1->left = leftRotate(node2);
        return rightRotate(node1);
    }
    
    void fixAfterInsertion(TreeNode* node)
    {
        TreeNode* parent = node->parent;
        TreeNode* grand_parent = nullptr;
        TreeNode* sub_tree = nullptr;
        
        while (parent != nullptr)
        {
            if (height(parent->left) == height(parent->right) + 2)
            {
                grand_parent = parent->parent;
                
                if (height(parent->left->left) >= height(parent->left->right))
                {
                    sub_tree = rightRotate(parent);
                }
                else
                {
                    sub_tree = leftRightRotate(parent);
                }
                
                if (grand_parent == nullptr)
                {
                    root = sub_tree;
                }
                else if (grand_parent->left == parent)
                {
                    grand_parent->left = sub_tree;
                }
                else
                {
                    grand_parent->right = sub_tree;
                }
                
                if (grand_parent != nullptr)
                {
                    grand_parent->height =
                        std::max(height(grand_parent->left),
                                 height(grand_parent->right)) + 1;
                }
                
                return;
            }
            else if (height(parent->right) == height(parent->left) + 2)
            {
                grand_parent =parent->parent;
                
                if (height(parent->right->right) >= height(parent->right->left))
                {
                    sub_tree = leftRotate(parent);
                }
                else
                {
                    sub_tree = rightLeftRotate(parent);
                }
                
                if (grand_parent == nullptr)
                {
                    root = sub_tree;
                }
                else if (grand_parent->left == parent)
                {
                    grand_parent->left = sub_tree;
                }
                else
                {
                    grand_parent->right = sub_tree;
                }
                
                if (grand_parent != nullptr)
                {
                    grand_parent->height =
                        std::max(height(grand_parent->left),
                                 height(grand_parent->right)) + 1;
                }
                
                return;
            }
            
            parent->height = std::max(height(parent->left),
                                      height(parent->right)) + 1;
            parent = parent->parent;
        }
    }
};

template<class RandomIt>
void tree_sort(RandomIt first, RandomIt last)
{
    RandomIt saveBeginIterator = first;
    auto range_length = std::distance(first, last);
    
    if (range_length < 2)
    {
        return;
    }
    
    typedef typename std::iterator_traits<RandomIt>::value_type value_type;
    
    Tree<value_type> tree(*first);
    first = std::next(first);
    
    while (first != last)
    {
        tree.add(*first);
        first = std::next(first);
    }
    
    tree.dumpContentsToArray(saveBeginIterator);
}

#endif // TREESORT_H
