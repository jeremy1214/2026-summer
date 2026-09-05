#pragma GCC optimize("Ofast")
#pragma GCC target("avx2")

#include <bits/stdc++.h>
using namespace std;
#define endl "\n"
#define pb push_back
#define f first
#define s second
#define all(x) x.begin(), x.end()
#define cout_line cout<<"line"<<__LINE__<<endl
#define cout_val(x) cout<<#x<<": "<<x<<endl
const long long mod=1e9+7;

struct SegTree {
    int n = 0;
    vector<int>  val;   // 節點負責的整段若同值，那個值是什麼
    vector<char> lazy;  // 這一整段都是 val，但子節點還不知道

    // 建立長度 n 的時間軸，所有格子初始為 0。
    void init(int size) {
        n = size;
        val.assign(4 * n + 4, 0);
        lazy.assign(4 * n + 4, 0);
        // TODO: 如果你的作法需要額外初始化，寫在這裡。
        //       用上面這種「全部歸零」的表示法時，通常不需要建樹。
    }

    // 把 val[node] 推給兩個子節點，並清掉自己的標記。
    void pushDown(int node) {
        // TODO: 實作推標記
        //   1. 若 lazy[node] 為假，直接返回
        //   2. 把 val[node] 複製給 2*node 與 2*node+1，並把它們的 lazy 設為真
        //   3. 清掉 lazy[node]
    }

    // 內部遞迴：node 負責 [l, r)，要把 [ql, qr) 設成 c。
    void assignImpl(int node, int l, int r, int ql, int qr, int c) {
        // TODO: 實作區間賦值
        //   1. 不相交（qr <= l 或 r <= ql）→ 返回
        //   2. 完整包含（ql <= l 且 r <= qr）→ 設 val 與 lazy，返回（不要往下走）
        //   3. 其餘 → pushDown 之後遞迴左右兩邊
    }

    // 內部遞迴：node 負責 [l, r)，查第 pos 格。
    int queryImpl(int node, int l, int r, int pos) {
        // TODO: 實作單點查詢
        //   1. 走到葉子（r - l == 1）→ 回傳 val[node]
        //   2. 否則 pushDown，再依 pos 與 mid 的關係往左或往右
        return 0;
    }

    // ---- 對外介面，測試只會呼叫這兩個，不需要修改 ----
    void assign(int l, int r, int c) {
        if (l >= r) return;
        assignImpl(1, 0, n, l, r, c);
    }
    int query(int pos) {
        return queryImpl(1, 0, n, pos);
    }
};




// ============================================================================
//  測試區 —— 以下請不要修改
// ============================================================================

static int g_failed = 0;
static int g_passed = 0;

#define CHECK(cond, ...)                                    \
    do {                                                    \
        if (cond) {                                         \
            ++g_passed;                                     \
        } else {                                            \
            ++g_failed;                                     \
            printf("  [FAIL] line %d: ", __LINE__);    \
            printf(__VA_ARGS__);                       \
            printf("\n");                              \
        }                                                   \
    } while (0)

// 樸素解：每格存一個值，用來當作正確答案的基準。
struct BruteForce {
    vector<int> a;
    void init(int n) { a.assign(n, 0); }
    void assign(int l, int r, int c) {
        for (int i = l; i < r; ++i) a[i] = c;
    }
    int query(int pos) { return a[pos]; }
};

// ---------------------------------------------------------------------------
//  階段一：固定測試
// ---------------------------------------------------------------------------
static void fixedTests() {
    printf("階段一：固定測試\n");

    {   // 1. 初始值全為 0
        SegTree t; t.init(8);
        bool ok = true;
        for (int i = 0; i < 8; ++i) if (t.query(i) != 0) ok = false;
        CHECK(ok, "初始值應該全部為 0");
    }
    {   // 2. 單一格子
        SegTree t; t.init(8);
        t.assign(3, 4, 42);
        CHECK(t.query(3) == 42, "query(3) 應為 42，實際為 %d", t.query(3));
        CHECK(t.query(2) == 0,  "query(2) 應為 0，實際為 %d", t.query(2));
        CHECK(t.query(4) == 0,  "query(4) 應為 0，實際為 %d", t.query(4));
    }
    {   // 3. 一般區間，確認左閉右開
        SegTree t; t.init(8);
        t.assign(2, 5, 7);
        CHECK(t.query(1) == 0, "左界外應為 0");
        CHECK(t.query(2) == 7 && t.query(3) == 7 && t.query(4) == 7, "[2,5) 內應全為 7");
        CHECK(t.query(5) == 0, "右界是開的，query(5) 應為 0，實際為 %d", t.query(5));
    }
    {   // 4. 空區間不應有任何作用
        SegTree t; t.init(8);
        t.assign(0, 8, 1);
        t.assign(4, 4, 9);
        CHECK(t.query(4) == 1, "空區間不應改變任何值，實際為 %d", t.query(4));
    }
    {   // 5. 覆蓋整條時間軸
        SegTree t; t.init(8);
        t.assign(0, 8, 5);
        bool ok = true;
        for (int i = 0; i < 8; ++i) if (t.query(i) != 5) ok = false;
        CHECK(ok, "assign(0, 8, 5) 之後應該全部為 5");
    }
    {   // 6. 後蓋的覆寫先蓋的
        SegTree t; t.init(8);
        t.assign(0, 8, 1);
        t.assign(2, 6, 2);
        t.assign(4, 5, 3);
        CHECK(t.query(1) == 1, "query(1) 應為 1，實際為 %d", t.query(1));
        CHECK(t.query(3) == 2, "query(3) 應為 2，實際為 %d", t.query(3));
        CHECK(t.query(4) == 3, "query(4) 應為 3，實際為 %d", t.query(4));
        CHECK(t.query(5) == 2, "query(5) 應為 2，實際為 %d", t.query(5));
        CHECK(t.query(7) == 1, "query(7) 應為 1，實際為 %d", t.query(7));
    }
    {   // 7. 貼齊左界
        SegTree t; t.init(8);
        t.assign(0, 3, 9);
        CHECK(t.query(0) == 9 && t.query(2) == 9, "[0,3) 內應為 9");
        CHECK(t.query(3) == 0, "query(3) 應為 0");
    }
    {   // 8. 貼齊右界
        SegTree t; t.init(8);
        t.assign(5, 8, 9);
        CHECK(t.query(7) == 9, "query(7) 應為 9，實際為 %d", t.query(7));
        CHECK(t.query(4) == 0, "query(4) 應為 0");
    }
    {   // 9. n 為 1
        SegTree t; t.init(1);
        CHECK(t.query(0) == 0, "單格初始應為 0");
        t.assign(0, 1, 6);
        CHECK(t.query(0) == 6, "單格 assign 之後應為 6，實際為 %d", t.query(0));
    }
    {   // 10. n 不是二的冪次
        SegTree t; t.init(5);
        t.assign(1, 4, 3);
        CHECK(t.query(0) == 0 && t.query(1) == 3 && t.query(3) == 3 && t.query(4) == 0,
              "n = 5 時 [1,4) 應為 3，其餘為 0");
    }
    {   // 11. 標記疊在同一段上
        SegTree t; t.init(8);
        t.assign(0, 8, 1);
        t.assign(0, 8, 2);
        t.assign(0, 8, 3);
        CHECK(t.query(0) == 3 && t.query(7) == 3, "連續覆蓋整段，最後一次應生效");
    }
    {   // 12. assign 與 query 交錯，確認標記有被正確推下去
        SegTree t; t.init(8);
        t.assign(0, 8, 1);
        CHECK(t.query(3) == 1, "第一次查詢");
        t.assign(2, 4, 2);
        CHECK(t.query(3) == 2, "查詢應反映後來的覆蓋，實際為 %d", t.query(3));
        CHECK(t.query(4) == 1, "未被覆蓋處應維持原值，實際為 %d", t.query(4));
        t.assign(3, 8, 5);
        CHECK(t.query(2) == 2 && t.query(3) == 5 && t.query(7) == 5, "三次覆蓋後的結果");
    }

    printf("  通過 %d 項，失敗 %d 項\n\n", g_passed, g_failed);
}

// ---------------------------------------------------------------------------
//  階段二：隨機對拍
// ---------------------------------------------------------------------------
static void stressTest() {
    printf("階段二：隨機對拍（與樸素解比對）\n");
    const int ROUNDS = 2000;
    mt19937 rng(20260904);
    int before = g_failed;

    for (int round = 0; round < ROUNDS; ++round) {
        int n = 1 + rng() % 40;
        SegTree    t;  t.init(n);
        BruteForce b;  b.init(n);

        vector<string> log;
        int ops = 1 + rng() % 30;
        bool bad = false;

        for (int k = 0; k < ops && !bad; ++k) {
            if (rng() % 2) {
                int l = rng() % (n + 1);
                int r = rng() % (n + 1);
                if (l > r) swap(l, r);
                int c = 1 + rng() % 100;
                t.assign(l, r, c);
                b.assign(l, r, c);
                char buf[64];
                snprintf(buf, sizeof buf, "assign(%d, %d, %d)", l, r, c);
                log.pb(buf);
            } else {
                int p = rng() % n;
                int got = t.query(p), want = b.query(p);
                char buf[64];
                snprintf(buf, sizeof buf, "query(%d)", p);
                log.pb(buf);
                if (got != want) {
                    ++g_failed;
                    printf("  [FAIL] 第 %d 輪，n = %d，query(%d) 得到 %d，應為 %d\n",
                                round, n, p, got, want);
                    printf("  重現用的操作序列：\n");
                    for (const auto& line : log) printf("    %s\n", line.c_str());
                    bad = true;
                }
            }
        }
        // 每一輪結束再全面比對一次
        for (int i = 0; i < n && !bad; ++i) {
            if (t.query(i) != b.query(i)) {
                ++g_failed;
                printf("  [FAIL] 第 %d 輪收尾比對，n = %d，第 %d 格得到 %d，應為 %d\n",
                            round, n, i, t.query(i), b.query(i));
                bad = true;
            }
        }
        if (bad) break;
    }

    if (g_failed == before) {
        ++g_passed;
        printf("  %d 輪全數一致\n\n", ROUNDS);
    } else {
        printf("  對拍失敗，先修好上面那一組再往下\n\n");
    }
}

// ---------------------------------------------------------------------------
//  階段三：效能測試
// ---------------------------------------------------------------------------
static void perfTest() {
    printf("階段三：效能測試\n");
    if (g_failed > 0) {
        printf("  前面尚有失敗項目，略過效能測試\n\n");
        return;
    }

    const int N   = 100000;
    const int OPS = 100000;
    mt19937 rng(12345);

    SegTree t;
    t.init(N);

    auto t0 = chrono::steady_clock::now();
    long long sink = 0;
    for (int k = 0; k < OPS; ++k) {
        if (k % 2 == 0) {
            int l = rng() % N;
            int r = l + 1 + rng() % (N - l);
            t.assign(l, r, (int)(rng() % 1000));
        } else {
            sink += t.query(rng() % N);
        }
    }
    auto t1 = chrono::steady_clock::now();
    double ms = chrono::duration<double, milli>(t1 - t0).count();

    printf("  %d 格、%d 次操作，耗時 %.1f ms（checksum %lld）\n", N, OPS, ms, sink);
    if (ms < 1000.0) {
        ++g_passed;
        printf("  通過\n\n");
    } else {
        ++g_failed;
        printf("  [FAIL] 超過一秒，某處可能退化成線性\n");
        printf("  最常見的原因：完整覆蓋時沒有及早返回，仍然遞迴到葉子\n\n");
    }
}

int main() {
    printf("========================================\n");
    printf(" 線段樹練習：區間染色與單點查詢\n");
    printf("========================================\n\n");

    fixedTests();
    stressTest();
    perfTest();

    printf("========================================\n");
    if (g_failed == 0) {
        printf(" 全部通過（%d 項）\n", g_passed);
    } else {
        printf(" 通過 %d 項，失敗 %d 項\n", g_passed, g_failed);
    }
    printf("========================================\n");
    return g_failed == 0 ? 0 : 1;
}