#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip> 

using namespace std;

template <class T>
void Permute(vector<T>& a, int n) {
    for (int i = n - 1; i >= 1; i--) {
        int j = rand() % (i + 1); // 隨機挑選 0 到 i 之間的索引
        swap(a[j], a[i]);         // 交換元素位置
    }
}

/**
 * (a)Insertion Sort
 */
void InsertionSort(vector<int>& a) {
    int n = a.size();
    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        // 將大於 key 的元素向後移動，為 key 騰出插入空間
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

/**
 * (b) Quick Sort - 三數取中法 (Median-of-Three)
 * 藉由挑選 low, mid, high 三個位置的中位數作為 Pivot，來避免極端數列導致的退化。
 */
int MedianOfThree(vector<int>& a, int low, int high) {
    int mid = low + (high - low) / 2;
    if (a[low] > a[mid]) swap(a[low], a[mid]);
    if (a[low] > a[high]) swap(a[low], a[high]);
    if (a[mid] > a[high]) swap(a[mid], a[high]);

    // 將選出的 Pivot 藏在 high - 1 的位置，方便後續分割指標移動
    swap(a[mid], a[high - 1]);
    return a[high - 1]; // 回傳 Pivot 值
}

void QuickSortHelper(vector<int>& a, int low, int high) {
    // 當子陣列長度大於 2 時，才使用三數取中法與雙指標分割
    if (low + 2 <= high) {
        int pivot = MedianOfThree(a, low, high);
        int i = low;
        int j = high - 1;
        while (true) {
            while (a[++i] < pivot); // 左指標向右找比 Pivot 大的數
            while (a[--j] > pivot); // 右指標向左找比 Pivot 小的數
            if (i < j) swap(a[i], a[j]); // 若兩指標未相遇，則進行交換
            else break;
        }
        // 將 Pivot 放回正確的分割點位置
        swap(a[i], a[high - 1]);

        // 遞迴處理左右子陣列
        QuickSortHelper(a, low, i - 1);
        QuickSortHelper(a, i + 1, high);
    }
    else {
        // 如果子陣列長度小於等於 2，直接用最簡單的比較交換處理
        if (low < high && a[low] > a[high]) swap(a[low], a[high]);
    }
}

void QuickSort(vector<int>& a) {
    if (a.empty()) return;
    QuickSortHelper(a, 0, a.size() - 1);
}

/**
 * (c) Merge Sort - 反覆向上的迭代實作法 (Iterative Method)
 */
void Merge(vector<int>& a, vector<int>& b, int l, int m, int r) {
    int i = l, j = m + 1, k = l;
    // 雙指標進行兩個已排序子陣列的合併
    while (i <= m && j <= r) {
        if (a[i] <= a[j]) b[k++] = a[i++];
        else b[k++] = a[j++];
    }
    // 將剩餘沒走完的元素依序搬移進暫存陣列
    while (i <= m) b[k++] = a[i++];
    while (j <= r) b[k++] = a[j++];

    // 將合併好的資料複製回原陣列 a
    for (i = l; i <= r; i++) a[i] = b[i];
}

void MergeSort(vector<int>& a) {
    int n = a.size();
    if (n <= 1) return;
    vector<int> b(n); // 配置與原陣列等大的暫存輔助空間

    // size 代表每次合併的子陣列長度 (以 1, 2, 4, 8... 的倍數增長)
    for (int size = 1; size < n; size *= 2) {
        for (int l = 0; l < n - 1; l += 2 * size) {
            int m = min(l + size - 1, n - 1);
            int r = min(l + 2 * size - 1, n - 1);
            Merge(a, b, l, m, r);
        }
    }
}

/**
 * (d) Heap Sort
 */
void MaxHeapify(vector<int>& a, int n, int i) {
    int largest = i;     // 先假設目前的根節點最大
    int l = 2 * i + 1;   // 左子節點索引
    int r = 2 * i + 2;   // 右子節點索引

    if (l < n && a[l] > a[largest]) largest = l;
    if (r < n && a[r] > a[largest]) largest = r;

    // 如果最大值不是根節點，則將其下移，並調整受影響的子樹
    if (largest != i) {
        swap(a[i], a[largest]);
        MaxHeapify(a, n, largest);
    }
}

void HeapSort(vector<int>& a) {
    int n = a.size();
    // 步驟 1：建構最大堆積 (Build Max Heap)
    for (int i = n / 2 - 1; i >= 0; i--) MaxHeapify(a, n, i);

    // 步驟 2：反覆將堆積頂端最大值移到陣列末尾，並重新調整剩餘堆積
    for (int i = n - 1; i > 0; i--) {
        swap(a[0], a[i]);
        MaxHeapify(a, i, 0); // 縮減堆積大小為 i，並從頂部往下調整
    }
}


// 3. 最壞情況（Worst-case）測試資料生成器

/**
 * 生成 Insertion Sort 的最壞情況資料
 */
vector<int> GenerateInsertionWorst(int n) {
    vector<int> a(n);
    for (int i = 0; i < n; i++) a[i] = n - i;
    return a;
}

/**
 * 輔助 Merge Sort 最壞情況數列的逆向拆分函數
 */
void Separate(vector<int>& a, vector<int>& tmp, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    int k = 0;

    // 將目前已排序好的子數列，奇數項與偶數項分流，破壞其平滑合併結構
    for (int i = l; i <= r; i += 2) tmp[k++] = a[i];
    for (int i = l + 1; i <= r; i += 2) tmp[k++] = a[i];
    for (int i = l; i <= r; i++) a[i] = tmp[i - l];

    // 持續往下逆向拆解
    Separate(a, tmp, l, m);
    Separate(a, tmp, m + 1, r);
}

/**
 * Merge Sort 的最壞情況資料
 */
vector<int> GenerateMergeWorst(int n) {
    vector<int> a(n);
    for (int i = 0; i < n; i++) a[i] = i + 1;
    vector<int> tmp(n);
    Separate(a, tmp, 0, n - 1); // 開始逆向打亂
    return a;
}

// 4. 高精度重複測試計時核心
/**
 * @brief 取得四種基本排序演算法的最壞情況時間
 * @param repetitions 高精度重複實驗次數，用於放大短時間排序以降低時鐘誤差
 */
double GetWorstTime(string sortType, int n, int repetitions) {

    // 類別 A：擁有固定且可直接建構的最壞資料 (Insertion & Merge)
    if (sortType == "Insertion" || sortType == "Merge") {
        vector<int> baseData = (sortType == "Insertion") ? GenerateInsertionWorst(n) : GenerateMergeWorst(n);

        auto start = chrono::high_resolution_clock::now();
        for (int r = 0; r < repetitions; r++) {
            vector<int> testData = baseData; // 每次排序前都要還原資料
            if (sortType == "Insertion") InsertionSort(testData);
            else MergeSort(testData);
        }
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duration = end - start;
        return duration.count() / repetitions; // 回傳平均每次執行的毫秒數 (ms)
    }

    // 類別 B：生成 15 次隨機排列，提取其「最大執行時間」
    if (sortType == "Quick" || sortType == "Heap") {
        double maxTime = 0.0;
        int permutationsCount = 15; // 超過投影片要求至少跑 10 次的規定

        for (int p = 0; p < permutationsCount; p++) {
            vector<int> baseData(n);
            for (int i = 0; i < n; i++) baseData[i] = i;

            // 使用 Knuth Shuffle 機制隨機打亂陣列
            Permute(baseData, n);

            auto start = chrono::high_resolution_clock::now();
            for (int r = 0; r < repetitions; r++) {
                vector<int> testData = baseData; // 每次計時還原資料
                if (sortType == "Quick") QuickSort(testData);
                else HeapSort(testData);
            }
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> duration = end - start;
            double currentAvgTime = duration.count() / repetitions;

            // 捕捉這 15 次排列中，最慢、耗時最長的數據（以此逼近最壞情況時間）
            if (currentAvgTime > maxTime) maxTime = currentAvgTime;
        }
        return maxTime;
    }
    return 0.0;
}

/*
 * Composite Sort：本作業的最終目標核心。
 * 規則：在最壞情況考量下，小於臨界值 20 使用 Insertion Sort，大於等於 20 則使用 Heap Sort。
 */
void CompositeSort(vector<int>& a) {
    int n = a.size();
    if (n < 20) {
        InsertionSort(a); // 小資料集下，Insertion Sort 的常數小、免額外記憶體，效率更佳
    }
    else {
        HeapSort(a);      // 大資料集下，死咬住 O(n log n) 邊界防止退化
    }
}

/*
 * 量測 Composite Sort 的最壞情況執行時間
 */
double GetCompositeTime(int n, int repetitions) {
    // 為了考量可能觸發 Insertion 的平方代價，我們基於完全逆序作為極端資料的測試
    vector<int> baseData = GenerateInsertionWorst(n);
    auto start = chrono::high_resolution_clock::now();
    for (int r = 0; r < repetitions; r++) {
        vector<int> testData = baseData;
        CompositeSort(testData);
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;
    return duration.count() / repetitions;
}

// 5. 主程式：支援批次多重輸入，集中輸出精準報表
int main() {
    srand(time(0)); // 初始化隨機數種子
    int count;

    cout << "幾個 n ";
    if (!(cin >> count) || count <= 0) return 0;

    vector<int> n_values(count);
    cout << "一次輸入這 " << count << " 個 n 值 : \n";
    for (int i = 0; i < count; i++) {
        cin >> n_values[i];
    }

    cout << "\n執行中\n\n";

    // 定義儲存表格每一橫列資料的結構體
    struct RowData {
        int n;
        double insertion, quick, merge, heap, composite;
    };
    vector<RowData> resultTable;

    for (int n : n_values) {
        // 智慧型重複次數微調：
        int reps = 20;
        if (n <= 100) reps = 500;
        else if (n <= 1000) reps = 100;
        else if (n <= 3000) reps = 40;

        RowData data;
        data.n = n;
        data.insertion = GetWorstTime("Insertion", n, reps);
        data.quick = GetWorstTime("Quick", n, reps);
        data.merge = GetWorstTime("Merge", n, reps);
        data.heap = GetWorstTime("Heap", n, reps);
        data.composite = GetCompositeTime(n, reps);

        resultTable.push_back(data); // 丟進記憶庫
    }
    cout << "========================================================================================\n";
    cout << "                    演算法實驗：最壞情況執行時間（單位：毫秒）\n";
    cout << "========================================================================================\n";

    cout << left << setw(10) << "n"
        << setw(18) << "Insertion"
        << setw(18) << "Quick"
        << setw(18) << "Merge"
        << setw(18) << "Heap"
        << setw(18) << "Composite" << "\n";
    cout << "----------------------------------------------------------------------------------------\n";

    // 依序倒出記憶庫裡的成果數據
    for (const auto& row : resultTable) {
        cout << left << setw(10) << row.n
            << setw(18) << row.insertion
            << setw(18) << row.quick
            << setw(18) << row.merge
            << setw(18) << row.heap
            << setw(18) << row.composite << "\n";
    }
    cout << "========================================================================================\n";
  

    return 0;
}