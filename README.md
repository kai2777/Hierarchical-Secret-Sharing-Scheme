# Hierarchical-Secret-Sharing-Scheme

這是一個使用 C++ 實作的Hierarchical Secret Sharing Scheme專案。

此方案的目的是將一個秘密 $K$ 分享給多個參與者，但與傳統的秘密分享不同，它定義了不同的「存取層級」或「群組」。只有當某個被授權的群組**全員到齊**時，該群組才能獨立還原出秘密。

這個專案的核心是基於「**Shamir 秘密分享方案 (Shamir's Secret Sharing, SSS)**」，並利用「**拉格朗日插值法 (Lagrange Interpolation)**」在有限體 (Finite Field) $\mathbb{Z}_Q$ 上進行運算。

## 核心概念

本專案實作了一個**兩層的階層架構**：

1.  **第一層 (群組層)**：
    * 秘密 $K$ 被拆分為兩個部分：一個「次級秘密」 $S_i$ 和一個「群組總份額」 $V_i$。
    * 它們的關係是 $K = (S_i + V_i) \pmod Q$。
    * $V_i$ 會被安全地儲存，或交給一個群組管理員。

2.  **第二層 (成員層)**：
    * 「次級秘密」 $S_i$（在程式碼中即為 $f_i(0)$）接著使用 Shamir 秘密分享方案，被進一步拆分給群組 `A[i]` 中的 $n$ 個成員。
    * 這裡使用的是 $(n, n)$ 門檻，意味著該群組**必須 $n$ 個成員全部到齊**，才能還原出「次級秘密」 $S_i$。

**還原流程**：
群組 `A[i]` 全員到齊 $\rightarrow$ 結合個人份額 `R` $\rightarrow$ 使用拉格朗日插值法還原 $S_i$ $\rightarrow$ 結合群組總份額 $V_i$ $\rightarrow$ 還原最終秘密 $K$。

## 專案功能與使用技術

本專案包含一整套在密碼學中至關重要的數論工具函數：

* **`LagrangeInterpolation` (拉格朗日插值法)**
    * **用途**：Shamir 秘密分享的核心。給定 $n$ 個點 $(x_i, y_i)$，此函數能還原出唯一穿過它們的 $n-1$ 次多項式 $f(x)$，並計算出 $f(0)$ 的值（即 $y$ 軸截距）。

* **`ExtentionEuclid` (擴展歐幾里得算法)**
    * **用途**：計算「模乘法反元素 (Modular Multiplicative Inverse)」。
    * **原因**：在模 $Q$ 的世界中，除法 $a / b$ 必須轉換為 $a \times b^{-1}$。此函數就是用來找出 $b^{-1}$。

* **`MultiplicationModular` (安全模乘法)**
    * **用途**：計算 $(a \times b) \pmod n$。
    * **原因**：當 $a$ 和 $b$ 都是非常大的 `long long` 時，`a * b` 會在取模 `n` **之前**就發生溢位 (overflow)。此函數使用 "double-and-add" 技巧來避免溢位。

* **`ExponentiationModular` (安全模冪運算)**
    * **用途**：計算 $(a^d) \pmod n$。
    * **原因**：同樣為了防止溢位。此函數使用 "square-and-multiply" 技巧來高效且安全地完成計算。

* **`PrimalityTest` (Miller-Rabin 素性測試)**
    * **用途**：高效判斷一個大數是否為質數。這是建立密碼學系統（選擇 $Q$）的基礎。

* **`GeneratePrime` / `GenerateBigOddNumber`**
    * **用途**：用於生成大質數的輔助函數。
## 檔案結構
├── MathFunction.h # 宣告所有數學工具函數的標頭檔

├── MathFunction.cpp # 實作所有數學工具函數的程式碼

└── main.cpp # 主程式，演示 HSSS 的「分發」與「重建」流程
 
## `main.cpp` 流程解析

`main.cpp` 清楚地展示了此方案的兩個階段：

### 1. Distribution (分發階段)

1.  **初始化**：定義秘密 `K`、質數模 `Q`、兩個群組 `A[0]` 和 `A[1]`，以及所有成員的個人份額 `R`。
2.  **計算群組總份額 `V`**：
    * 遍歷每個群組 `A[i]`。
    * 收集該群組所有成員的 `(ID, Share)` 組合。
    * 使用 `LagrangeInterpolation` 還原出該群組的「次級秘密」 $f_i(0)$。
    * 計算「群組總份額」 $V_i = (K - f_i(0)) \pmod Q$。

### 2. Reconstruction (重建階段)

1.  **模擬還原**：再次遍歷每個群組 `A[i]`。
2.  **還原次級秘密**：
    * 模擬該群組成員到齊，收集他們的 `(ID, Share)`。
    * 使用 `LagrangeInterpolation` 再次計算出 $f_i(0)$。
3.  **還原最終秘密**：
    * 將「次級秘密」 $f_i(0)$ 與先前儲存的「群組總份額」 $V_i$ 相加。
    * $answer = (f_i(0) + V_i) \pmod Q$
    * 由於 $V_i = K - f_i(0)$，所以 $answer$ 總是等於 $K$。
4.  **驗證**：程式會輸出兩次 `K` 的值，證明 `A[0]` 和 `A[1]` 都能獨立還原秘密。
