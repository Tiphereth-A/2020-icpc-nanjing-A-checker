#include <bits/stdc++.h>
using namespace std;

namespace _Checker_basic {
typedef pair<ptrdiff_t, ptrdiff_t> pii;
typedef long long i64;

constexpr size_t ARRAY_OFFSET = 5;
constexpr char operators[4] = {'U', 'D', 'L', 'R'};
constexpr pii diff[4] = {make_pair(0, 1), make_pair(0, -1), make_pair(-1, 0), make_pair(1, 0)};

pii operator+(const pii& lhs, const pii& rhs) {
    pii ret = lhs;
    ret.first += rhs.first;
    ret.second += rhs.second;
    return ret;
}

size_t getID_from_operators(char op) {
    for (size_t idx = 0; idx < sizeof(operators); ++idx)
        if (operators[idx] == op) return idx;
    cout << "Invalid operators" << endl;
    exit(1);
}
}  // namespace _Checker_basic
using namespace _Checker_basic;

namespace _Config {
#define __STAT_DETAIL__
#define __REC_TIME__
#define __PRINT_HACK_DATA__

constexpr size_t MAP_MAX = 20;
constexpr size_t SEQ_MAX = 6e4;
constexpr size_t DATA_NUM = 500;
constexpr size_t ACCESSABLE_HACK_LEN = 5e4;
constexpr double ACCESSABLE_RATE = 0.25;

const char maps[MAP_MAX + ARRAY_OFFSET][MAP_MAX + ARRAY_OFFSET] = {
  "11111111111111111111\n",
  "10000000000000000001\n",
  "10111111111111111101\n",
  "10100000000000000101\n",
  "10101111111111110101\n",
  "10101000000000010101\n",
  "10101011111111010101\n",
  "10101010000001010101\n",
  "10101010111101010101\n",
  "10101010100101010101\n",
  "10101010100101010101\n",
  "10101010101101010101\n",
  "10101010100001010101\n",
  "10101010111111010101\n",
  "10101010000000010101\n",
  "10101011111111110101\n",
  "10101000000000000101\n",
  "10101111111111111101\n",
  "10100000000000000001\n",
  "10111111111111111111\n"};
}  // namespace _Config
using namespace _Config;

namespace _Check {
char operator_sequence[SEQ_MAX + ARRAY_OFFSET];

uniform_int_distribution<size_t> u(0, 3);
void generate_operator_sequence(size_t seed, size_t len) {
#define _RANDOM(RANDOM_ENGINE_TYPE)                                                \
    {                                                                              \
        RANDOM_ENGINE_TYPE engine(seed);                                           \
        for (int i = 0; i < len; ++i) operator_sequence[i] = operators[u(engine)]; \
    }

    switch (seed % 3) {
        case 0:
            _RANDOM(minstd_rand)
            break;
        case 1:
            _RANDOM(mt19937)
            break;
        default:
            _RANDOM(ranlux24)
            break;
    }
    // or simply
    // _RANDOM(default_random_engine)
#undef _
}

bool is_valid(const pii& point, size_t map_max) {
    return point.first >= 0 && point.first < map_max && point.second >= 0 && point.second < map_max;
}
bool is_empty(const pii& point) { return maps[point.first][point.second] == '1'; }

bool vis[MAP_MAX + ARRAY_OFFSET][MAP_MAX + ARRAY_OFFSET];

namespace DSU {
size_t dsu_len;
size_t fa[(MAP_MAX + ARRAY_OFFSET) * (MAP_MAX + ARRAY_OFFSET) + ARRAY_OFFSET];

size_t _find(size_t x) { return x == fa[x] ? fa[x] : fa[x] = _find(fa[x]); }
void _merge(size_t x, size_t y) { fa[_find(x)] = _find(y); }

size_t getID(const pii& point, size_t map_max) { return point.first * map_max + point.second; }

void init(size_t _len) {
    dsu_len = _len;
    for (size_t i = 1; i < dsu_len; ++i) fa[i] = i;
}
size_t find(const pii& x, size_t map_max) { return _find(getID(x, map_max)); }
void merge(const pii& x, const pii& y, size_t map_max) { _merge(getID(x, map_max), getID(y, map_max)); }
}  // namespace DSU

namespace _Debug {
void print_vis(size_t map_max) {
    cerr << "vis:" << endl;
    for (size_t i = 0; i < map_max; ++i) {
        for (size_t j = 0; j < map_max; ++j) cerr << (vis[i][j] ? '1' : '0');
        cerr << endl;
    }
    cerr << endl;
}
void print_operator_sequence(size_t len) {
    for (size_t i = 0; i < len; ++i) cerr << operator_sequence[i];
    cerr << endl;
}
void print_DSU(size_t len) {
    for (size_t i = 0; i < len; ++i) cerr << DSU::fa[i] << " \n"[i == len - 1];
}
}  // namespace _Debug

void map_first_check(size_t map_max) {
    cout << "Start checking map..." << endl;
    size_t cnt = 0;

    for (size_t i = 0; i < map_max; ++i)
        for (size_t j = 0; j < map_max; ++j) cnt += maps[i][j] == '1';
    if (cnt < 2) throw invalid_argument("The number of empty cell is less than 2");

    cnt = 0;
    memset(vis, 0, sizeof(vis));
    for (size_t i = 0; i < map_max; ++i)
        for (size_t j = 0; j < map_max; ++j) {
            if (vis[i][j] || maps[i][j] != '1') continue;
            if ((++cnt) > 1)
                throw invalid_argument("Exist one empty cell that is not reachable from any empty cell");

            vis[i][j] = 1;
            queue<pii> q;
            q.push(make_pair(i, j));
            DSU::init(map_max * map_max);
            while (!q.empty()) {
                pii front = q.front();
                q.pop();
                for (const pii& d : diff) {
                    pii now = front + d;
                    if (!is_valid(now, map_max) || vis[now.first][now.second] || !is_empty(now)) continue;
                    vis[now.first][now.second] = 1;
                    q.push(now);

                    if (DSU::find(front, map_max) == DSU::find(now, map_max))
                        throw invalid_argument("Exist cycle(s)");

                    DSU::merge(front, now, map_max);
                }
            }
        }

    cout << "OK" << endl;
    return;
}

size_t hack_main(size_t operator_sequence_len, size_t map_max) {
    vector<pii> kangaroos;
    for (size_t i = 0; i < map_max; ++i)
        for (size_t j = 0; j < map_max; ++j)
            if (maps[i][j] == '1') kangaroos.push_back(make_pair(i, j));

    size_t step;
    for (step = 0; step < operator_sequence_len; ++step) {
        pii now_point;
        size_t idx;
        for (pii& pre_point : kangaroos) {

            try {
                idx = getID_from_operators(operator_sequence[step]);
            } catch (const runtime_error& e) {
                cerr << e.what() << endl;
            }

            now_point = pre_point + diff[idx];
            if (!is_valid(now_point, map_max) || !is_empty(now_point)) continue;
            pre_point = now_point;
        }
        pii tmp = *kangaroos.begin();
        auto it = kangaroos.begin() + 1;
        for (; it != kangaroos.end(); ++it)
            if (*it != tmp) break;
        if (it == kangaroos.end()) break;
    }
    return step;
}

void main(size_t* steps, size_t tot_data_num = DATA_NUM, size_t operator_sequence_len = SEQ_MAX, size_t map_max = MAP_MAX) {
    try {
        map_first_check(map_max);
    } catch (const invalid_argument& e) {
        throw invalid_argument(string("Invalid map: ") + string(e.what()));
    }

    cout << "Start hacking..." << endl;
    for (size_t i = 0; i < tot_data_num; ++i) {
        generate_operator_sequence(time(nullptr) + i, operator_sequence_len);

#ifdef __PRINT_HACK_DATA__
        _Debug::print_operator_sequence(operator_sequence_len);
#endif

        steps[i] = hack_main(operator_sequence_len, map_max);
    }
    cout << "OK" << endl;
}
}  // namespace _Check

namespace _Statistics {
size_t steps[DATA_NUM + ARRAY_OFFSET];

void main(size_t data_len = DATA_NUM) {
    int accessed = 0;
    for (size_t i = 0; i < data_len; ++i) accessed += (steps[i] > ACCESSABLE_HACK_LEN);
    double rate = 1.0 * accessed / data_len;
    cout << "Successful hack: " << accessed << endl
         << "Rate: " << rate << endl;

    cout << "Result:" << endl
         << (rate > ACCESSABLE_RATE ? "Passed" : "Too weak") << endl;

#ifdef __STAT_DETAIL__
    cout << "---" << endl;
    cout << "Details:" << endl;

    size_t _max = 0, _min = -1;
    for (size_t i = 0; i < data_len; ++i) _max = max(_max, steps[i]);
    for (size_t i = 0; i < data_len; ++i) _min = min(_min, steps[i]);
    cout << "- Max: " << _max << endl;
    cout << "- Min: " << _min << endl;

    cout << "- All results:" << endl
         << endl
         << "  ";
    for (size_t i = 0; i < data_len; ++i) cout << steps[i] << (i == data_len - 1 ? "\n" : ", ");

    cout << "---" << endl;
#endif
}
}  // namespace _Statistics

int main() {
#ifdef __REC_TIME__
    clock_t _CLOCK_ST = clock();
#endif

    try {
        _Check::main((size_t*)_Statistics::steps);
    } catch (const invalid_argument& e) {
        cout << e.what() << endl;
        return 0;
    }
    _Statistics::main();

#ifdef __REC_TIME__
    cerr << "\n---\n"
         << "Time used: " << clock() - _CLOCK_ST << endl;
#endif
    return 0;
}
