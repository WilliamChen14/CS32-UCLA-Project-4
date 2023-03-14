#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED
#include <vector>


template <typename KeyType, typename ValueType>
class TreeMultimap
{
    struct Node {
        KeyType keyVal;
        std::vector<ValueType> val; 
        Node* left;
        Node* right; 
    };
    
  public:

    class Iterator
    {
      public:
        Iterator()
        {
            p = nullptr; 
        }

        Iterator(Node* ptr) {
            p = ptr; 
            it = p->val.begin(); 
        }

        ValueType& get_value() const
        {
            if (!is_valid()) {
                exit(1); 
            }
            return *it; 
        }

        bool is_valid() const
        {
            if (p != nullptr && it >= p->val.begin() && it < p->val.end()) {
                return true;
            }
            return false; 
        }

        void advance()
        {
            if (!is_valid()) {
                return; 
            }
            it++;
        }

      private:
          Node* p; 
          typename std::vector<ValueType>::iterator it; 
    };

    TreeMultimap()
    {
        root = nullptr; 
    }

    ~TreeMultimap()
    {
        deleteNodes(root);
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        if (root == nullptr) {
            root = new Node(); 
            root->keyVal = key; 
            root->left = nullptr;
            root->right = nullptr;
            root->val.push_back(value); 
            return;
        }

        Node* curr = root; 
        for (;;) {
            if (curr->keyVal == key) {
                curr->val.push_back(value); 
                return; 
            }

            else if (curr->keyVal < key) {
                if (curr->left != nullptr) {
                    curr = curr->left; 
                }

                else {
                    curr->left = new Node(); 
                    curr->left->left = nullptr;
                    curr->left->right = nullptr;
                    curr->left->keyVal = key; 
                    curr->left->val.push_back(value);
                    return; 
                }
            }

            else if (curr->keyVal > key) {
                if (curr->right != nullptr) {
                    curr = curr->right;
                }

                else {
                    curr->right = new Node();
                    curr->right->left = nullptr;
                    curr->right->right = nullptr;
                    curr->right->keyVal = key; 
                    curr->right->val.push_back(value);
                    return; 
                } 
            }
        }
    }

    Iterator find(const KeyType& key) const
    {
        Iterator it;
        Node* curr = root;
        if (Search(key, curr)) {
            it = Iterator(curr);
        }
        return it; 
    }

  private:

      Node* root; 

      void deleteNodes(Node* curr) {
          if (curr == nullptr) {
              return;
          }
          deleteNodes(curr->left); 
          deleteNodes(curr->right);
          delete curr; 
      }

      bool Search(KeyType key, Node*& curr) const {
          if (curr == nullptr) {
              return false;
          }

          else if (curr->keyVal == key) {
              return true;
          }
          
          else if (curr->keyVal < key) {
              curr = curr->left;
              return Search(key, curr);
          }

          else {
              curr = curr->right; 
              return Search(key, curr); 
          }
      }
};

#endif // TREEMULTIMAP_INCLUDED
