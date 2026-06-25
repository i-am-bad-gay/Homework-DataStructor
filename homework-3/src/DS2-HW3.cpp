// ============================================================
// 排序演算法實驗程式
// 功能：測量 Insertion / Quick / Merge / Heap / Composite
//       五種排序在「最壞情況」下的執行時間
// ============================================================

#include <iostream>   // 提供 cin（輸入）、cout（輸出）功能
#include <vector>     // 提供 vector 動態陣列（大小可變的陣列）
#include <algorithm>  // 提供 swap()（交換）、min()（取較小值）等工具
#include <chrono>     // 提供高精度計時器，用來量測排序花多少時間
#include <cstdlib>    // 提供 rand()（產生隨機數）、srand()（設定亂數種子）
#include <ctime>      // 提供 time()，讓亂數種子根據當前時間而不同
#include <iomanip>    // 提供 setw()（設定輸出欄位寬度），讓輸出排版對齊

using namespace std;  // 讓我們可以直接寫 cout 而不用寫 std::cout


// ============================================================
// 【工具函式】Permute：Fisher-Yates 洗牌演算法
// 功能：把陣列 a 的元素順序完全隨機打亂
// 參數：a = 要打亂的陣列，n = 陣列長度
// ============================================================
template <class T>              // template 讓這個函式可以接受任何型態（int、double 都行）
void Permute(vector<T>& a, int n) {
    for (int i = n - 1; i >= 1; i--) {         // 從最後一個元素往前跑，每輪縮小「還沒固定」的範圍
        int j = rand() % (i + 1);              // 從 0 到 i 之間隨機抽一個索引 j
        swap(a[j], a[i]);                       // 把 a[j] 和 a[i] 互換位置
    }
    // 執行完後，陣列裡每個元素出現在任何位置的機率都相同（真正的均勻亂序）
}


// ============================================================
// 【排序 (a)】Insertion Sort（插入排序）
// 直覺：像整理撲克牌——每次從牌堆抽一張，插入左邊已排好的正確位置
// 時間複雜度：平均 O(n²)，最壞（完全逆序）也是 O(n²)
// ============================================================
void InsertionSort(vector<int>& a) {
    int n = a.size();               // 取得陣列的總長度

    for (int i = 1; i < n; i++) {  // 從第 2 個元素開始，逐一往前插入（第 1 個視為已排好）
        int key = a[i];             // 把當前這張「牌」抽出來暫存，key 就是要插入的值
        int j = i - 1;             // j 指向已排好的最右邊那張牌

        // 只要左邊的牌比 key 大，就把那張牌往右推一格，空出插入位置
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];       // 把 a[j] 往右移一格（覆蓋掉原來 a[j+1] 的位置）
            j--;                   // j 繼續往左看下一張牌
        }

        a[j + 1] = key;            // 找到正確位置了，把 key 塞進去
    }
}


// ============================================================
// 【排序 (b)】Quick Sort 輔助函式：三數取中（Median-of-Three）
// 功能：從 a[low]、a[mid]、a[high] 三個位置挑出中位數當 pivot
//       這樣可以避免選到最大或最小值，防止 Quick Sort 退化成 O(n²)
// 回傳：pivot 的值（已被藏在 a[high-1] 位置）
// ============================================================
int MedianOfThree(vector<int>& a, int low, int high) {
    int mid = low + (high - low) / 2;  // 計算中間位置（這樣寫不會整數溢位）

    // 用三次比較把 a[low]、a[mid]、a[high] 排成由小到大的順序
    if (a[low] > a[mid])  swap(a[low], a[mid]);   // 確保 a[low] ≤ a[mid]
    if (a[low] > a[high]) swap(a[low], a[high]);  // 確保 a[low] ≤ a[high]
    if (a[mid] > a[high]) swap(a[mid], a[high]);  // 確保 a[mid] ≤ a[high]
    // 執行完以上三行後：a[low] ≤ a[mid] ≤ a[high]，a[mid] 就是中位數（pivot）

    swap(a[mid], a[high - 1]);  // 把 pivot 藏到 high-1 的位置，讓左右指標從 low+1 和 high-2 出發，不需碰邊界
    return a[high - 1];         // 回傳 pivot 的值
}


// ============================================================
// 【排序 (b)】Quick Sort 遞迴核心
// 功能：對 a[low] 到 a[high] 這個子陣列進行快速排序
// ============================================================
void QuickSortHelper(vector<int>& a, int low, int high) {

    if (low + 2 <= high) {
        // 子陣列長度 ≥ 3，值得用三數取中法 + 雙指標分割

        int pivot = MedianOfThree(a, low, high);  // 選出 pivot，並藏在 a[high-1]
        int i = low;        // 左指標，從 low 出發（等等會先 ++i 才使用）
        int j = high - 1;   // 右指標，從 high-1（pivot 的位置）出發（等等會先 --j 才使用）

        while (true) {
            while (a[++i] < pivot);  // 左指標 i 先往右移一步，再比較：找到第一個「≥ pivot」的元素就停
            while (a[--j] > pivot);  // 右指標 j 先往左移一步，再比較：找到第一個「≤ pivot」的元素就停

            if (i < j)  swap(a[i], a[j]);  // 兩指標還沒交叉：把左邊太大的和右邊太小的互換
            else        break;              // 兩指標已交叉：分割完成，跳出迴圈
        }

        swap(a[i], a[high - 1]);    // 把 pivot 從藏匿處（high-1）放回正確的分割點位置（a[i]）
        // 此時 a[i] 左邊都 ≤ pivot，右邊都 ≥ pivot，pivot 本身在正確位置

        QuickSortHelper(a, low, i - 1);   // 遞迴處理 pivot 左邊的子陣列
        QuickSortHelper(a, i + 1, high);  // 遞迴處理 pivot 右邊的子陣列

    } else {
        // 子陣列長度 ≤ 2，直接用最簡單的一次比較交換處理，不值得再遞迴
        if (low < high && a[low] > a[high]) swap(a[low], a[high]);
    }
}


// ============================================================
// 【排序 (b)】Quick Sort 對外入口
// ============================================================
void QuickSort(vector<int>& a) {
    if (a.empty()) return;                      // 空陣列不需要排序，直接離開
    QuickSortHelper(a, 0, a.size() - 1);        // 對整個陣列呼叫遞迴核心
}


// ============================================================
// 【排序 (c)】Merge Sort 輔助函式：合併兩個已排序子陣列
// 功能：把 a[l..m] 和 a[m+1..r] 這兩段已排好的子陣列合併成一段
// 參數：b 是輔助暫存陣列（合併結果先放這裡，再複製回 a）
// ============================================================
void Merge(vector<int>& a, vector<int>& b, int l, int m, int r) {
    int i = l;      // 左半段的指標，從 l 開始
    int j = m + 1;  // 右半段的指標，從 m+1 開始
    int k = l;      // 輔助陣列 b 的寫入位置，從 l 開始

    // 雙指標：每次從左、右兩段各取一個元素比較，把較小的放進 b
    while (i <= m && j <= r) {
        if (a[i] <= a[j]) b[k++] = a[i++];  // 左邊的比較小（或相等），放左邊的進 b，i 和 k 各往右移一步
        else              b[k++] = a[j++];  // 右邊的比較小，放右邊的進 b，j 和 k 各往右移一步
    }

    // 如果左半段還有剩餘元素（右半段已跑完），全部依序放進 b
    while (i <= m) b[k++] = a[i++];

    // 如果右半段還有剩餘元素（左半段已跑完），全部依序放進 b
    while (j <= r) b[k++] = a[j++];

    // 把合併好的結果從輔助陣列 b 複製回原陣列 a 的對應位置
    for (i = l; i <= r; i++) a[i] = b[i];
}


// ============================================================
// 【排序 (c)】Merge Sort（迭代版，由下而上）
// 直覺：先合併長度 1 的小段 → 再合併長度 2 → 4 → 8 …，不需要遞迴
// 時間複雜度：任何情況都是 O(n log n)
// ============================================================
void MergeSort(vector<int>& a) {
    int n = a.size();
    if (n <= 1) return;          // 只有 0 或 1 個元素，不需要排序

    vector<int> b(n);            // 配置一個和 a 一樣大的輔助陣列 b，只分配一次，重複使用

    // size 是每次合併的子陣列長度，從 1 開始，每輪翻倍（1 → 2 → 4 → 8 …）
    for (int size = 1; size < n; size *= 2) {

        // l 是每一對子陣列的起始位置，每次跳 2*size 個位置（跳過剛合併好的那一對）
        for (int l = 0; l < n - 1; l += 2 * size) {
            int m = min(l + size - 1, n - 1);       // 左半段的結束位置（min 防止超出陣列邊界）
            int r = min(l + 2 * size - 1, n - 1);   // 右半段的結束位置（min 防止超出陣列邊界）
            Merge(a, b, l, m, r);                    // 合併 a[l..m] 和 a[m+1..r]
        }
    }
}


// ============================================================
// 【排序 (d)】Heap Sort 輔助函式：Max-Heapify（向下調整）
// 功能：假設 i 的左右子樹都已是合法的 max-heap，
//       把 a[i] 向下推到正確位置，讓整棵子樹恢復 max-heap 性質
// 參數：n = 目前堆積的有效大小（排序過程中會逐漸縮小）
// ============================================================
void MaxHeapify(vector<int>& a, int n, int i) {
    int largest = i;        // 先假設根節點（i）是這三個（父、左子、右子）中最大的
    int l = 2 * i + 1;     // 左子節點的索引（二元樹用陣列表示時的公式）
    int r = 2 * i + 2;     // 右子節點的索引

    // 如果左子節點存在，且比目前的「最大值」還大，更新 largest
    if (l < n && a[l] > a[largest]) largest = l;

    // 如果右子節點存在，且比目前的「最大值」還大，更新 largest
    if (r < n && a[r] > a[largest]) largest = r;

    if (largest != i) {
        // 根節點不是最大值，把根和最大值的位置互換
        swap(a[i], a[largest]);

        // 交換之後，largest 那個位置的子樹可能被破壞了，遞迴繼續往下調整
        MaxHeapify(a, n, largest);
    }
    // 如果 largest == i，代表根節點已是最大，不需要做任何事
}


// ============================================================
// 【排序 (d)】Heap Sort（堆積排序）
// 直覺：先把陣列建成「最大堆」，再反覆把最大值（堆頂）搬到陣列末尾
// 時間複雜度：任何情況都是 O(n log n)，且不需要額外空間
// ============================================================
void HeapSort(vector<int>& a) {
    int n = a.size();

    // ── 階段 1：建構最大堆（Build Max Heap）──
    // 從最後一個「非葉節點」（索引 n/2-1）開始，往根節點方向逐一做 MaxHeapify
    // 葉節點不需要調整（它們沒有子節點），所以從 n/2-1 開始就夠了
    for (int i = n / 2 - 1; i >= 0; i--) {
        MaxHeapify(a, n, i);    // 對節點 i 進行向下調整，確保以 i 為根的子樹是合法 max-heap
    }
    // 執行完後，a[0] 是整個陣列中的最大值

    // ── 階段 2：反覆取出最大值，縮小堆積 ──
    for (int i = n - 1; i > 0; i--) {
        swap(a[0], a[i]);       // 把目前堆頂（最大值 a[0]）換到陣列末尾（a[i]），這個位置就定案了
        MaxHeapify(a, i, 0);   // 堆積大小縮小為 i（排除已定案的末尾），從根節點重新向下調整
    }
    // 每輪把最大值推到末尾，最終陣列就是由小到大排好的
}


// ============================================================
// 【測資生成】GenerateInsertionWorst：產生 Insertion Sort 的最壞測試資料
// 最壞情況 = 完全逆序陣列 [n, n-1, n-2, ..., 2, 1]
// 因為每個元素都要一路移動到最左端，總共要做 n*(n-1)/2 次比較
// ============================================================
vector<int> GenerateInsertionWorst(int n) {
    vector<int> a(n);                       // 建立長度為 n 的陣列
    for (int i = 0; i < n; i++) a[i] = n - i;  // a[0]=n, a[1]=n-1, ..., a[n-1]=1（完全逆序）
    return a;                               // 回傳這個逆序陣列
}


// ============================================================
// 【測資生成】Separate：Merge Sort 最壞測資的輔助函式（逆向拆分）
// 功能：把一段已排序的子陣列，逆向打亂成「讓 Merge 每次都需要最多比較」的順序
// 做法：把偶數索引的元素排前面，奇數索引的排後面，然後遞迴拆解
// ============================================================
void Separate(vector<int>& a, vector<int>& tmp, int l, int r) {
    if (l >= r) return;                     // 子陣列長度 ≤ 1，不需要再拆，直接結束

    int m = l + (r - l) / 2;               // 計算中間位置
    int k = 0;                              // tmp 陣列的寫入索引

    // 先把偶數位置（l, l+2, l+4, ...）的元素依序搬進 tmp
    for (int i = l; i <= r; i += 2) tmp[k++] = a[i];

    // 再把奇數位置（l+1, l+3, l+5, ...）的元素依序搬進 tmp
    for (int i = l + 1; i <= r; i += 2) tmp[k++] = a[i];

    // 把 tmp 的結果寫回 a 的 [l..r] 範圍
    for (int i = l; i <= r; i++) a[i] = tmp[i - l];

    // 對左右兩半繼續遞迴拆解（逆向模擬合併過程的每一層）
    Separate(a, tmp, l, m);
    Separate(a, tmp, m + 1, r);
}


// ============================================================
// 【測資生成】GenerateMergeWorst：產生 Merge Sort 的最壞測試資料
// 原理：從 [1,2,...,n] 出發，用 Separate 逆向破壞它的「易合併」結構，
//       讓 Merge Sort 每次合併都需要最多次比較（兩邊元素完全交錯）
// ============================================================
vector<int> GenerateMergeWorst(int n) {
    vector<int> a(n);
    for (int i = 0; i < n; i++) a[i] = i + 1;  // 先建立 [1, 2, 3, ..., n] 的有序陣列
    vector<int> tmp(n);                          // 輔助暫存陣列
    Separate(a, tmp, 0, n - 1);                  // 逆向打亂，讓 Merge Sort 做最多工
    return a;                                    // 回傳打亂後的陣列
}


// ============================================================
// 【組合排序】CompositeSort
// 規則：n < 20 用 Insertion Sort；n ≥ 20 用 Heap Sort
// 原因：
//   - 小資料集：Insertion Sort 常數小、沒有遞迴開銷、記憶體存取連續，實際上比 O(n log n) 還快
//   - 大資料集：Heap Sort 保證 O(n log n) 不會退化，不像 Quick Sort 有最壞 O(n²) 的風險
// ============================================================
void CompositeSort(vector<int>& a) {
    int n = a.size();
    if (n < 20) {
        InsertionSort(a);   // 資料量小，Insertion Sort 實際執行更快
    } else {
        HeapSort(a);        // 資料量大，用 Heap Sort 確保穩定的 O(n log n) 上限
    }
}


// ============================================================
// 【計時核心】MeasureWorstTime：量測單一排序演算法的最壞情況執行時間
// 參數：
//   sortType   = 排序名稱字串（"Insertion" / "Quick" / "Merge" / "Heap"）
//   n          = 資料筆數
//   repetitions = 重複執行次數（次數越多，時間測量越準確）
// 回傳：以毫秒為單位的最壞情況執行時間
// ============================================================
double MeasureWorstTime(const string& sortType, int n, int repetitions) {

    // ── 準備對應的最壞情況測試資料 ──
    vector<int> baseData;
    if (sortType == "Insertion") {
        baseData = GenerateInsertionWorst(n);   // Insertion 的最壞 = 完全逆序
    } else if (sortType == "Merge") {
        baseData = GenerateMergeWorst(n);       // Merge 的最壞 = 逆向 Separate 打亂
    } else {
        // Quick Sort 和 Heap Sort 沒有固定最壞序列，用隨機資料多次測試取最大值
        baseData.resize(n);
        for (int i = 0; i < n; i++) baseData[i] = i;  // 先建 [0,1,2,...,n-1]
    }

    int permutationsCount = 15;     // 隨機排列測試次數（Quick/Heap 用）
    double totalTimeSum = 0.0;      // 累計所有排列的平均時間
    double maxTime = 0.0;           // 記錄所有排列中最慢的那次（代表最壞情況）

    for (int p = 0; p < permutationsCount; p++) {

        // 如果是 Quick 或 Heap，每輪重新隨機打亂，嘗試找出最壞排列
        vector<int> currentBase = baseData;
        if (sortType == "Quick" || sortType == "Heap") {
            Permute(currentBase, n);    // 隨機打亂，嘗試各種排列
        }

        // ── 高精度計時開始 ──
        auto start = chrono::high_resolution_clock::now();

        for (int r = 0; r < repetitions; r++) {
            vector<int> testData = currentBase;  // 每次重新複製一份乾淨的測試資料（排序會改變陣列）

            // 根據 sortType 呼叫對應的排序函式
            if      (sortType == "Insertion") InsertionSort(testData);
            else if (sortType == "Quick")     QuickSort(testData);
            else if (sortType == "Merge")     MergeSort(testData);
            else if (sortType == "Heap")      HeapSort(testData);
        }

        // ── 高精度計時結束 ──
        auto end = chrono::high_resolution_clock::now();

        // 計算這輪的平均執行時間（毫秒）
        // chrono::duration<double, milli> 把時間差轉換成以毫秒為單位的浮點數
        double currentAvg = chrono::duration<double, milli>(end - start).count() / repetitions;

        totalTimeSum += currentAvg;                  // 累加這輪平均時間
        if (currentAvg > maxTime) maxTime = currentAvg;  // 更新最慢紀錄
    }

    // Quick / Heap 回傳「最大值」（代表最壞情況的上界）
    // Insertion / Merge 回傳「平均值」（因為最壞測資是固定的，多排幾次取平均更準）
    if (sortType == "Quick" || sortType == "Heap") {
        return maxTime;                              // 最壞邊界
    }
    return totalTimeSum / permutationsCount;         // 校準後的平均耗時
}


// ============================================================
// 【計時核心】GetCompositeTime：量測 Composite Sort 的最壞情況執行時間
// 說明：Composite Sort 在 n < 20 時用 Insertion（可能 O(n²)），
//       在 n ≥ 20 時用 Heap（O(n log n)），因此分兩種策略測量
// ============================================================
double GetCompositeTime(int n, int repetitions) {
    double maxTime = 0.0;       // 記錄所有排列中最慢的那次
    int permutationsCount = 15; // 測試 15 種不同排列，取最慢的作為最壞情況

    for (int p = 0; p < permutationsCount; p++) {

        vector<int> testBase(n);

        if (n < 20) {
            // n < 20 時 Composite 會走 Insertion Sort，最壞情況是逆序
            testBase = GenerateInsertionWorst(n);   // 產生完全逆序資料
        } else {
            // n ≥ 20 時 Composite 會走 Heap Sort，用隨機資料多次嘗試
            for (int i = 0; i < n; i++) testBase[i] = i;  // 建立 [0,1,...,n-1]
            Permute(testBase, n);                           // 隨機打亂
        }

        // ── 高精度計時開始 ──
        auto start = chrono::high_resolution_clock::now();

        for (int r = 0; r < repetitions; r++) {
            vector<int> testData = testBase;    // 每次重新複製，避免排序後的有序資料影響下次計時
            CompositeSort(testData);            // 執行組合排序
        }

        // ── 高精度計時結束 ──
        auto end = chrono::high_resolution_clock::now();

        // 計算這輪的平均執行時間（毫秒）
        double currentAvg = chrono::duration<double, milli>(end - start).count() / repetitions;

        if (currentAvg > maxTime) maxTime = currentAvg;  // 只保留最慢的那次（最壞情況）
    }

    return maxTime;     // 回傳最壞情況執行時間
}


// ============================================================
// 【主程式】main
// 流程：讀入多個 n 值 → 對每個 n 計時 → 最後一次性輸出格式化報表
// ============================================================
int main() {
    srand(time(0));     // 用當前時間當亂數種子，確保每次執行的隨機結果不同

    int count;          // 使用者要輸入幾個 n 值

    cout << "幾個 n ";
    if (!(cin >> count) || count <= 0) return 0;    // 輸入不合法就直接結束程式

    vector<int> n_values(count);    // 建立一個陣列來儲存所有 n 值
    cout << "一次輸入這 " << count << " 個 n 值 : \n";
    for (int i = 0; i < count; i++) {
        cin >> n_values[i];         // 逐一讀入每個 n 值
    }

    cout << "\n執行中\n\n";

    // ── 定義儲存每一列報表資料的結構體 ──
    struct RowData {
        int n;              // 資料筆數
        double insertion;   // Insertion Sort 的最壞時間（毫秒）
        double quick;       // Quick Sort 的最壞時間（毫秒）
        double merge;       // Merge Sort 的最壞時間（毫秒）
        double heap;        // Heap Sort 的最壞時間（毫秒）
        double composite;   // Composite Sort 的最壞時間（毫秒）
    };
    vector<RowData> resultTable;    // 把每個 n 的結果存進這裡，最後統一輸出

    // ── 對每個 n 值進行計時 ──
    for (int n : n_values) {

        // 根據 n 的大小調整重複次數：
        //   n 越小 → 排序極快 → 需要重複很多次才能量到有意義的時間
        //   n 越大 → 排序較慢 → 重複少一點，避免整體執行時間過長
        int reps = 20;
        if      (n <= 100)  reps = 500;   // 極小資料：重複 500 次
        else if (n <= 1000) reps = 100;   // 小資料：重複 100 次
        else if (n <= 3000) reps = 40;    // 中等資料：重複 40 次
        // n > 3000 維持 reps = 20

        RowData data;                                           // 這個 n 值的結果
        data.n         = n;
        data.insertion = MeasureWorstTime("Insertion", n, reps);  // 量 Insertion 最壞時間
        data.quick     = MeasureWorstTime("Quick",     n, reps);  // 量 Quick 最壞時間
        data.merge     = MeasureWorstTime("Merge",     n, reps);  // 量 Merge 最壞時間
        data.heap      = MeasureWorstTime("Heap",      n, reps);  // 量 Heap 最壞時間
        data.composite = GetCompositeTime(n, reps);               // 量 Composite 最壞時間

        resultTable.push_back(data);    // 把這列結果存進表格
    }

    // ── 輸出格式化報表 ──
    cout << "========================================================================================\n";
    cout << "                    演算法實驗：最壞情況執行時間（單位：毫秒）\n";
    cout << "========================================================================================\n";

    // 輸出表頭，setw(N) 讓每欄固定佔 N 個字元寬，left 讓文字靠左對齊
    cout << left
         << setw(10) << "n"
         << setw(18) << "Insertion"
         << setw(18) << "Quick"
         << setw(18) << "Merge"
         << setw(18) << "Heap"
         << setw(18) << "Composite" << "\n";
    cout << "----------------------------------------------------------------------------------------\n";

    // 依序輸出每一列資料（每個 n 對應一列）
    for (const auto& row : resultTable) {
        cout << left
             << setw(10) << row.n
             << setw(18) << row.insertion
             << setw(18) << row.quick
             << setw(18) << row.merge
             << setw(18) << row.heap
             << setw(18) << row.composite << "\n";
    }

    cout << "========================================================================================\n";

    return 0;   // main 回傳 0 代表程式正常結束
}