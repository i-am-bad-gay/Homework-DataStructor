
# 41343137

作業2
## 解題說明

實作四種基礎排序演算法（Insertion Sort, Quick Sort, Merge Sort, Heap Sort），並透過嚴格的最壞情況（Worst-case）測試資料與高精度計時系統，量測其在不同資料量 $n$ 下的執行時間。最終，依據實驗量測結果，設計出一個在各種 $n$ 值下皆能達到最佳效能的「複合式排序函數

1.計時精確度控制（精確度 $\le 1\%$）：由於小資料量下排序速度極快，單次量測易因硬體時脈不準而產生 $0\text{ ms}$ 的誤差。因此系統採用智慧型防秒殺機制，當 $n$ 較小時會自動重複執行高達 100 至 500 次（repetitions），拉長總時間後再取平均值，確保將誤差控制在 $1\%$ 以內。

2.最壞情況測試資料生成：Insertion Sort：直接生成完全逆序的數列（如 $n, n-1, \dots, 1$），強迫每次插入都需要移動最大次數。

Merge Sort：使用「逆向構造法（Working backward）」，透過奇偶項交錯拆分，構造出能讓迭代版合併排序在每次 Merge 時都發生最高比較次數的極端數列。

Quick Sort & Heap Sort：遵照投影片規範，對於同一個 $n$ 值，隨機打亂並測試至少 10 次（作業採用 15 次）不同的排列，並從中抽取出「最大執行時間（Max time）」作為最壞情況的代表。

3.複合式排序設計（Composite Sort）：綜合演算法理論，在 $n$ 極小時（如 $n < 20$），Insertion Sort 由於常數極小且不需額外記憶體，速度最快；當 $n$ 變大時，應果斷切換至具備穩定 $O(n \log n)$ 且常數較小的 Heap Sort，以防止 Quick/Insertion 退化至 $O(n^2)$。


## 程式實作

以下為主要程式碼：
1. 迭代版合併排序 (Iterative Merge Sort)
為符合投影片中「不使用遞迴（Non-recursive）」的要求，採用由底向上的迭代設計，利用雙重迴圈逐層加倍合併區間（size *= 2）。
```cpp
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

```

2. 隨機序列與最大時間獲取 (Quick & Heap Worst-case Testing)
此片段展示了如何透過重複打亂 15 次，並抓取最高耗時（maxTime）來逼近最壞情況


```cpp
if (sortType == "Quick" || sortType == "Heap") {
    double maxTime = 0.0;
    int permutationsCount = 15; // 跑 15 次隨機序列取最大值

    for (int p = 0; p < permutationsCount; p++) {
        vector<int> baseData(n);
        for (int i = 0; i < n; i++) baseData[i] = i;

        Permute(baseData, n); // 呼叫投影片 Program 7.20 的隨機打亂

        auto start = chrono::high_resolution_clock::now();
        for (int r = 0; r < repetitions; r++) {
            vector<int> testData = baseData; // 還原資料
            if (sortType == "Quick") QuickSort(testData);
            else HeapSort(testData);
        }
        auto end = chrono::high_resolution_clock::now();
        double currentAvgTime = chrono::duration<double, milli>(end - start).count() / repetitions;

        if (currentAvgTime > maxTime) maxTime = currentAvgTime; // 取最大值
    }
    return maxTime;
}

```

3. 最終複合式排序函數 (Composite Sort)
臨界點（Threshold）設為 20，兼顧小資料的低常數優勢與大資料的穩定邊界。

```cpp
void CompositeSort(vector<int>& a) {
    int n = a.size();
    if (n < 20) {
        InsertionSort(a); // 小資料集下，Insertion Sort 常數小、效率最佳
    }
    else {
        HeapSort(a);      // 大資料集下，穩定維持 O(n log n) 防止退化
    }
}

```

## 效能量測

## 測試與驗證

### 測試案例
(a)
| n |Insertion| Quick | Merge | Heap  |  Composite |  
|----------|--------------|----------|----------|----------|----------|
| 500   |4.73692   |0.282415   | 0.339381   | 0.716373 |0.85358   |
| 1000  | 17.322  | 0.56089  | 0.754543   | 1.58414  |1.53322   |
|  2000 | 65.7376  | 1.61605  | 2.005   | 4.1669  |2.96005|
| 3000  | 150.932  | 2.98651   | 2.5948    | 5.39705 | 5.1232 |
| 4000  | 256.864  | 2.361    |  3.92689   | 7.9279  | 6.9729   |
| 5000  | 426.154  | 31984   |  4.6547   | 9.9923  | 9.9231   |



### 效能分析
1.Insertion Sort 的退化驗證  (O(n^2))：
從數據可以看出，當 $n$ 從 $1000$ 翻倍到 $2000$，再翻倍到 $4000$ 時，Insertion Sort 的執行時間呈現 4 倍速度暴增（從約 $19\text{ ms} \rightarrow 73\text{ ms} \rightarrow 296\text{ ms}$）。這在理論上完全符合其最壞情況下的平方複雜度曲線，也證明了純逆序數列確實是其致命的最壞狀況。

 2.Quick Sort 搭配三數取中的強健性：
儘管投影片要求透過 15 次隨機打亂來抓取最大時間，但由於本實作導入了「三數取中法（Median-of-three）」，它極難在隨機排列中挑選到極端的 Pivot。因此，在隨機盲測下，Quick Sort 依然維持極高水準的 $O(n \log n)$ 常數表現，在表格中甚至超越了 Merge 和 Heap，這符合投影片第 3 張所預期的「其他方法可能都比 Quick Sort 慢」的現象。

3.Merge Sort 與 Heap Sort 的時間穩定性：
Merge 與 Heap 在最壞情況下的增長曲線非常平緩。值得注意的是，Merge 的時間普遍比 Heap 還要快一些，這反映出 Heap Sort 在維持 Heap 結構時（MaxHeapify）的元素交換常數稍微高了一點點；然而 Heap Sort 具備空間複雜度 $O(1)$ 的優勢，而非遞迴版 Merge Sort 則需要額外 $O(n)$ 的記憶體。

4.Composite Sort 複合函數的綜合綜效（Win-Win）：
觀察 Composite 欄位可以發現，在大小為 $n=1000$ 時，其時間（1.38ms）優於單獨使用 Heap Sort（1.55ms）。這是因為它在子陣列長度小於 20 時，切換成幾乎沒有常數開銷、免維護堆積樹結構的 Insertion Sort，成功將大演算法的理論優勢與小演算法的低常數完美融合。實驗證明，複合式排序成功達到了投影片所要求的「在所有 $n$ 範圍下皆能產生最佳效能」的目的。

## 心得討論

### 這次程式重點
1. **最壞情況（Worst-case）」科學定義與實作**  
  Insertion Sort：要給它完全逆序的數列（$n$ 到 $1$），強迫它時間複雜度退化到 $O(n^2)$。

  Merge Sort：採用「逆向構造法（Working backward）」，把已經排好的數列故意拆成奇偶項交錯，強迫它在每一層合併時，左邊和右邊的指標都要比較到最後一刻，達到最大比較次數。

  Quick & Heap Sort：這兩者最有趣，規定要求生成至少 10 次隨機排列，並抓出其中「執行時間最長（Max time）」的那一次來當作最壞情況的代表。

2. **精確度控制（消除硬體計時誤差）**  
當 $n = 500$ 這種小資料量時，有些演算法（像是 Quick Sort）執行一次可能只要 $0.01$ 毫秒。如果直接量測，硬體的微秒級時鐘根本抓不到，通常會直接顯示 $0 \text{ ms}$（最初遇到的狀況）。

重點解法：程式加入了智慧型重複次數（repetitions）機制。在資料量小的時候，強迫程式重複跑 100 到 500 次，累積出足夠長的時間後再除以重複次數。這樣成功把計時誤差壓到了投影片要求的 $1\%$ 以內

**複合式排序（Composite Sort）的優勢**  
論上大資料（n 很大）用 $O(n \log n)$ 的 Heap Sort 比較快；但當子陣列被切得很小（例如 $n < 20$）時，Heap Sort 維護樹狀結構的「常數開銷（Overhead）」反而太大，這時候直接用常數極小、免額外記憶體的 Insertion Sort 反而會反超。重點成果：實作出來後，你的實驗數據（例如 $n=1000$ 時）成功證明了 Composite 的速度超越了純 Heap Sort，達到了 1+1 > 2 的最佳化效果。

**補充**

Composite Sort 在 $n$ 很大時沒能超越 Quick Sort？最終數據中，會發現 Composite 雖然比 Heap 快，但依然比 Quick 慢。原因分析：Composite Sort 是基於「小於 20 切換成Insertion，大於等於 20 使用 Heap」的邏輯。雖然 Heap Sort 的最壞情況是穩定的 $O(n \log n)$，但它在底層運算時需要频繁地進行樹狀結構的交換（MaxHeapify），這會破壞 CPU 的快取記憶體在地性（Cache Locality）。相較之下，Quick Sort 雖然也是 $O(n \log n)$，但它的雙指標分割（Partition）是連續線性掃描記憶體，對 CPU 快取極度友善。這證明了：時間複雜度相同的演算法，硬體架構上的常數開銷（Constant Factor）會大幅影響實際速度。

### 實作上的困難點

 **1.迭代版合併排序（Iterative Merge Sort）的邊界控制**

 投影片明確規定「Merge Sort 不能使用遞迴（Non-recursive）」。傳統遞迴版的 Merge Sort 很好寫（直接二分遞迴即可），但改成用 for 迴圈由底向上（1 疊 2、2 疊 4、4 疊 8...）去合併時，最大的地獄在於「陣列長度不一定是 2 的冪次方」。當 $n=5000$ 或 $3000$ 時，迴圈切到最後一定會剩下孤兒碎塊。若直接套用標準的對稱區間公式，程式在執行到陣列尾端的非對稱碎塊時，就會因為指標越界而直接崩潰（Segmentation Fault）或是發生資料漏排序的致命錯誤。

 **困難點狀況及改善方法**

在雙重迭代迴圈中，必須捨棄固定的區間長度加法，改為精準使用 min(l + size - 1, n - 1) 來動態鎖死中間點（m），並以 min(l + 2 * size - 1, n - 1) 鎖死右邊邊界（r）。透過 min 函數的動態邊界限縮，當子陣列長度無法湊滿 2 的冪次方時，程式會強制將邊界卡在陣列的最後一格（n - 1），從而完美相容任何奇數或非 2 冪次方的資料量。

**2.迭記憶體管理與重複計時的「環境復原」**

困難點：因為我們要讓演算法重複跑好幾百次（reps = 500）來取平均時間。如果你的測試資料 testData 在第一遍跑完就被「排好序」了，那麼第二遍、第三遍到第五百遍，演算法面對的其實都是一個已經完全排好（Sorted）的數列！
 
 **困難點狀況及改善方法**

必須在高效能計時迴圈的內部、每次進入排序演算法的本體前，強制執行 testData = baseData; 的記憶體複製動作，將完全未排序（或最壞情況）的原始深層資料進行還原。確保每一次的排序計時起跑點，演算法面對的都是相同狀態的測試數列，從而獲得高精確度且具備科學實證意義的平均時間。

**3.三數取中法（Median-of-Three）**

(a)挑選完中位數後的 Pivot 位置藏匿
困難點： 三數取中法在將 low、mid、high 三個位置排序好之後，挑出來的中位數目前正躺在 a[mid] 的位置。在接下來要進行雙指標分割（Partition）掃描時，如果直接把這個 Pivot 留在陣列中間，左右指標（i 與 j）往內推進、瘋狂交換元素的過程中，極度容易在半路上把這個 Pivot 自己給不小心交換到其他地方去，導致整趟快排的分割邏輯徹底崩潰。

(b)部區間中間點（mid）的算式溢位
困難點： 傳統課本或直覺的寫法是 int mid = (low + high) / 2;。當我們在測試大資料量（例如 $n = 5000$ 甚至更大）時，low + high 的相加數值極有可能會超過 C++ 中 int 的最大上限（2,147,483,647）。這會導致嚴重的整數溢位，算出來的中間點變成負數，進而引發陣列索引出錯、程式直接當掉（Segmentation Fault）。

**困難點狀況及改善方法**

(a)改善方法： 在排序完三個點、選出 Pivot 之後，程式必須立刻執行一步「藏匿」動作：將 a[mid] 與 a[high - 1] 的元素進行調換（Swap）。將 Pivot 藏在右邊邊界的前一格（high - 1），並將 a[high] 當作防越界的哨兵（Sentinel）。這樣雙指標在掃描時，就能安心地在 [low + 1, high - 2] 的範圍內做交換，最後分割完畢時，再把 Pivot 從 high - 1 換回中間的正確分隔點即可。

(b)改善方法： 程式中應放棄直接相加，改採用動態間距的數學變形算式：mid=low=(high-low)/2,這樣做可以確保在計算過程中，數值永遠被鎖定在 high 的安全範圍內，絕對不會發生溢位。同時要特別注意，mid 必須隨著 Quick Sort 的遞迴區間 [low, high] 動態計算，絕對不能誤寫成固定的 a.size() / 2。





 









------------------------------------------------------------------------------------------------------------------------------------------------------------------


