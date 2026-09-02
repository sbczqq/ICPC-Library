## 数论 - 莫比乌斯反演计算 gcd(x,y)=k

### 算法说明

* **用途**：快速计算 $\sum_{x=1}^n \sum_{y=1}^m [\gcd(x, y) = k]$ 的对数（即在指定范围内，最大公约数为 $k$ 的数对数量）。
* **时间复杂度**：
  * **预处理**：$O(N)$（线性筛求 $\mu$ 函数及前缀和）。
  * **单次查询**：$O(\sqrt{N})$（数论整除分块）。非常适合处理高达 $10^5$ 级别的多组询问 (Multi-testcases)。
* **核心推导**：
  将求 $[\gcd(x, y) = k]$ 转化为 $[\gcd(x/k, y/k) = 1]$，套用莫比乌斯核心定理 $[n=1] = \sum_{d|n} \mu(d)$，交换求和次序后得到最终化简公式：
  $$ \text{Ans} = \sum_{d=1}^{\min(\lfloor n/k \rfloor, \lfloor m/k \rfloor)} \mu(d) \lfloor \frac{n}{kd} \rfloor \lfloor \frac{m}{kd} \rfloor $$
* **实现细节**：公式中 $\lfloor \frac{n}{kd} \rfloor \lfloor \frac{m}{kd} \rfloor$ 呈块状分布，使用整除分块跳跃式累加，配合 `sum_mu` 前缀和实现 $O(1)$ 块内求值。