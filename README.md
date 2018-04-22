## 根据表达式生成PLC的逻辑代码

Programmable Logic Controller

PLC expression generator.

在PLC中, 如果想让输入`0`和输入`1`异或后输出到`1002`中, 

需要写这样的代码

```haskell
LD 0
ANDNOT 1
LDNOT 0
ANDNOT 1
ORLD
OUT 1002
```

但是在使用类`PLCEXP`后, 可以这样写

```cpp
PLCEXP a = 0, b = 1 ,c = 1002;
a * !b + !a * b >> c;
```

就可以自动生成相同逻辑的PLC代码了.

### 具体使用方法

需要声明PLC表达式, 然后用PLC表达式进行运算.

* `*`代表`and`
* `+`代表`or`
* `!`代表取反
* `>>`代表将左边表达式值输出到右边

具体代码如下

```c++
#include<iostream>
#include "PLC.h" // 包含PLC.h头文件, 放在程序相同目录下
using namespace std;

// 创建PLC表达式
PLCEXP a = 0; 
PLCEXP b = 1;
PLCEXP c = 1002;

int main()
{
    // 写逻辑运算的表达式
    a * !b + !a * b >> c; // 将 a * !b + !a * b 的结果输出到 c中
    return 0;
}
```

#### 效果

##### 生成的代码

```haskell
LD 0
LDNOT 1
ANDLD
LDNOT 0
LD 1
ANDLD
ORLD
OUT 1002
```

##### 梯形图

![](xor(a,b).PNG)

### 更复杂的例子

#### 不带TIM

实现`!(A+B * !D+C * !D)`输出到`E`.

我们知道`!(A+B * !D+C * !D) = !A * !(B * !D) * !(C * !D) = !A * (!B + D) * (!C + D) `

```c++
#include<iostream>
#include "PLC.h" // 包含PLC.h头文件, 放在程序相同目录下
using namespace std;

// 创建PLC表达式
PLCEXP A = 0;
PLCEXP B = 1;
PLCEXP C = 2;
PLCEXP D = 3;
PLCEXP E = 1004;
int main()
{
    // 写逻辑运算的表达式
    !(A+B * !D+C * !D) >> E;
    return 0;
}
```

##### 生成的代码

```haskell
LDNOT 0
LDNOT 1
LD 3
ORLD
ANDLD
LDNOT 2
LD 3
ORLD
ANDLD
OUT 1004
```

##### 梯形图

![](2.PNG)

