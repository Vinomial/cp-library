#pragma once

#include <algorithm>
#include <cassert>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

namespace graph_lib
{
    // 最大流(Dinic法)を計算する有向グラフ
    template <class Cap = long long>
    class mf_graph
    {
    public:
        mf_graph() = default;
        explicit mf_graph(int n) : n_(n), g_(n) {}

        // 有向辺 from -> to (容量 cap, 逆辺の容量 0) を追加し、辺番号を返す
        int add_edge(int from, int to, Cap cap)
        {
            assert(0 <= from && from < n_);
            assert(0 <= to && to < n_);
            assert(0 <= cap);
            const int m = static_cast<int>(pos_.size());
            pos_.push_back({from, static_cast<int>(g_[from].size())});
            int from_id = static_cast<int>(g_[from].size());
            int to_id = static_cast<int>(g_[to].size());
            if (from == to)
                ++to_id;
            g_[from].push_back(raw_edge{to, to_id, cap});
            g_[to].push_back(raw_edge{from, from_id, Cap(0)});
            return m;
        }

        struct edge
        {
            int from, to;
            Cap cap, flow;
        };

        // i番目に追加した辺の現在の状態を返す
        edge get_edge(int i) const
        {
            const int m = static_cast<int>(pos_.size());
            assert(0 <= i && i < m);
            const auto &e = g_[pos_[i].first][pos_[i].second];
            const auto &re = g_[e.to][e.rev];
            return edge{pos_[i].first, e.to, e.cap + re.cap, re.cap};
        }

        // 追加した辺すべての現在の状態を、add_edgeで追加した順に返す
        std::vector<edge> edges() const
        {
            const int m = static_cast<int>(pos_.size());
            std::vector<edge> result;
            result.reserve(m);
            for (int i = 0; i < m; ++i)
                result.push_back(get_edge(i));
            return result;
        }

        // i番目の辺の容量・流量を書き換える（他の辺には影響しない）
        void change_edge(int i, Cap new_cap, Cap new_flow)
        {
            const int m = static_cast<int>(pos_.size());
            assert(0 <= i && i < m);
            assert(0 <= new_flow && new_flow <= new_cap);
            auto &e = g_[pos_[i].first][pos_[i].second];
            auto &re = g_[e.to][e.rev];
            e.cap = new_cap - new_flow;
            re.cap = new_flow;
        }

        // sからtへ流せるだけ流し、流量を返す
        Cap flow(int s, int t)
        {
            return flow(s, t, std::numeric_limits<Cap>::max());
        }

        // sからtへ、流量がflow_limitに達するまで流せるだけ流し、流量を返す（Dinic法）
        Cap flow(int s, int t, Cap flow_limit)
        {
            assert(0 <= s && s < n_);
            assert(0 <= t && t < n_);
            assert(s != t);

            std::vector<int> level(n_), iter(n_);

            auto bfs = [&]()
            {
                std::fill(level.begin(), level.end(), -1);
                level[s] = 0;
                std::queue<int> que;
                que.push(s);
                while (!que.empty())
                {
                    const int v = que.front();
                    que.pop();
                    for (const auto &e : g_[v])
                    {
                        if (e.cap == 0 || level[e.to] >= 0)
                            continue;
                        level[e.to] = level[v] + 1;
                        if (e.to == t)
                            return;
                        que.push(e.to);
                    }
                }
            };

            // 増加パスをDFSで探しつつ、その場で流量を流し込む
            auto dfs = [&](auto self, int v, Cap up) -> Cap
            {
                if (v == s)
                    return up;
                Cap res = 0;
                const int level_v = level[v];
                for (int &i = iter[v]; i < static_cast<int>(g_[v].size()); ++i)
                {
                    raw_edge &e = g_[v][i];
                    if (level_v <= level[e.to] || g_[e.to][e.rev].cap == 0)
                        continue;
                    const Cap d = self(self, e.to, std::min(up - res, g_[e.to][e.rev].cap));
                    if (d <= 0)
                        continue;
                    g_[v][i].cap += d;
                    g_[e.to][e.rev].cap -= d;
                    res += d;
                    if (res == up)
                        return res;
                }
                level[v] = n_;
                return res;
            };

            Cap total = 0;
            while (total < flow_limit)
            {
                bfs();
                if (level[t] == -1)
                    break;
                std::fill(iter.begin(), iter.end(), 0);
                const Cap f = dfs(dfs, t, flow_limit - total);
                if (!f)
                    break;
                total += f;
            }
            return total;
        }

        // 最後に呼んだflowの残余グラフで、sから到達可能な頂点集合を返す（s-t最小カットのs側）
        std::vector<bool> min_cut(int s) const
        {
            std::vector<bool> visited(n_);
            std::queue<int> que;
            visited[s] = true;
            que.push(s);
            while (!que.empty())
            {
                const int v = que.front();
                que.pop();
                for (const auto &e : g_[v])
                {
                    if (e.cap && !visited[e.to])
                    {
                        visited[e.to] = true;
                        que.push(e.to);
                    }
                }
            }
            return visited;
        }

    private:
        struct raw_edge
        {
            int to, rev;
            Cap cap;
        };

        int n_ = 0;
        std::vector<std::pair<int, int>> pos_;
        std::vector<std::vector<raw_edge>> g_;
    };
} // namespace graph_lib
