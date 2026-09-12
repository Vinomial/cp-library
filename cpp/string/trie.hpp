#pragma once

#include <array>
#include <cassert>
#include <string>
#include <vector>

namespace trie_lib
{
    // ALPHABET: 分岐の数、BASE: 添字への変換に使う基準文字（既定は英小文字26種）
    template <int ALPHABET = 26, char BASE = 'a'>
    class Trie
    {
    public:
        struct Node
        {
            std::array<int, ALPHABET> children;
            int end_count = 0;   // このノードが終端である文字列が現在何本挿入されているか
            long long prefix_count = 0; // このノードを通過した文字列の本数

            Node() { children.fill(-1); }
        };

        // nodes_[0] を根として初期化する
        Trie() { nodes_.push_back(Node()); }

        // ノード数の目安（挿入する文字列の長さの総和+1）が事前に分かる場合に呼ぶ
        void reserve(int n) { nodes_.reserve(n + 1); }

        // 文字列 s を挿入し、終端ノードの番号を返す
        int insert(const std::string &s)
        {
            int cur = 0;
            for (char c : s)
            {
                const int idx = static_cast<int>(c - BASE);
                if (nodes_[cur].children[idx] == -1)
                {
                    nodes_[cur].children[idx] = static_cast<int>(nodes_.size());
                    nodes_.push_back(Node());
                }
                cur = nodes_[cur].children[idx];
                ++nodes_[cur].prefix_count;
            }
            ++nodes_[cur].end_count;
            return cur;
        }

        // 文字列 s を1つ削除する（s が現在挿入されている前提。破っていれば assert で落ちる）
        void erase(const std::string &s)
        {
            int cur = 0;
            for (char c : s)
            {
                const int idx = static_cast<int>(c - BASE);
                const int nxt = nodes_[cur].children[idx];
                assert(nxt != -1);
                cur = nxt;
                --nodes_[cur].prefix_count;
            }
            assert(nodes_[cur].end_count > 0);
            --nodes_[cur].end_count;
        }

        // 文字列 s を辿った先のノード番号を返す（辿れなければ -1）
        int find_node(const std::string &s) const
        {
            int cur = 0;
            for (char c : s)
            {
                const int idx = static_cast<int>(c - BASE);
                if (nodes_[cur].children[idx] == -1)
                    return -1;
                cur = nodes_[cur].children[idx];
            }
            return cur;
        }

        // s がちょうど挿入されているか
        bool contains(const std::string &s) const
        {
            const int cur = find_node(s);
            return cur != -1 && nodes_[cur].end_count > 0;
        }

        // s を接頭辞に持つ文字列が 1 つ以上挿入されているか
        bool has_prefix(const std::string &s) const
        {
            const int cur = find_node(s);
            return cur != -1 && nodes_[cur].prefix_count > 0;
        }

        // ノード番号からノードの中身を参照する（問題ごとの探索ロジックはこれで書く）
        const Node &node(int i) const { return nodes_[i]; }

        int root() const { return 0; }
        int size() const { return static_cast<int>(nodes_.size()); }

    private:
        std::vector<Node> nodes_;
    };
} // namespace trie_lib
