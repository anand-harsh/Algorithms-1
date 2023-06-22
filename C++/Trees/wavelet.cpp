#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Node {
    int value;
    vector<int> bitmap;
    Node* left;
    Node* right;
};

class WaveletTree {
private:
    vector<int> arr;
    vector<int> alphabet;
    Node* root;

public:
    WaveletTree(const vector<int>& input) {
        arr = input;
        alphabet = getAlphabet();
        root = buildTree(arr, alphabet.front(), alphabet.back());
    }

    vector<int> getAlphabet() {
        vector<int> alpha = arr;
        sort(alpha.begin(), alpha.end());
        alpha.erase(unique(alpha.begin(), alpha.end()), alpha.end());
        return alpha;
    }

    Node* buildTree(const vector<int>& arr, int low, int high) {
        if (arr.empty()) {
            return nullptr;
        }

        if (low == high) {
            Node* node = new Node();
            node->value = low;
            node->left = nullptr;
            node->right = nullptr;
            return node;
        }

        int mid = (low + high) / 2;
        vector<int> leftArr, rightArr;
        vector<int> bitmap;

        for (int i : arr) {
            if (i <= mid) {
                leftArr.push_back(i);
                bitmap.push_back(0);
            } else {
                rightArr.push_back(i);
                bitmap.push_back(1);
            }
        }

        Node* node = new Node();
        node->value = mid;
        node->bitmap = bitmap;
        node->left = buildTree(leftArr, low, mid);
        node->right = buildTree(rightArr, mid + 1, high);

        return node;
    }

    int rank(int queryChar, int index) {
        int charIndex = distance(alphabet.begin(), find(alphabet.begin(), alphabet.end(), queryChar));
        return rankHelper(root, charIndex, index + 1);
    }

    int rankHelper(Node* node, int charIndex, int index) {
        if (node->value == alphabet[charIndex]) {
            return index;
        }

        if (node->value > alphabet[charIndex]) {
            return rankHelper(node->left, charIndex, index);
        }

        int leftCount = getCount(node->bitmap, index - 1, 0);
        return rankHelper(node->right, charIndex, leftCount + index);
    }

    int getCount(const vector<int>& bitmap, int index, int count) {
        if (index < 0) {
            return count;
        }

        if (bitmap[index] == 1) {
            count += 1;
        }

        return getCount(bitmap, index - 1, count);
    }

    int select(int queryChar, int count) {
        int charIndex = distance(alphabet.begin(), find(alphabet.begin(), alphabet.end(), queryChar));
        return selectHelper(root, charIndex, count);
    }

    int selectHelper(Node* node, int charIndex, int count) {
        if (node->value == alphabet[charIndex]) {
            return count - 1;
        }

        if (node->value > alphabet[charIndex]) {
            return selectHelper(node->left, charIndex, count);
        }

        int leftCount = getCount(node->bitmap, count - 1, 0);
        return selectHelper(node->right, charIndex, leftCount + count);
    }
};

int main() {
    vector<int> arr = {1, 2, 3, 2, 1, 3, 1, 2, 1};
    WaveletTree wt(arr);

    cout << wt.rank(2, 5) << endl;  
    cout << wt.select(2, 2) << endl;  
    return 0;
}
