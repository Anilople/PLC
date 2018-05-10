## PLC逻辑生成 - 内嵌领域专用语言(half)

### 想法来源

使用`CX-Programmer`来编写PLC(Programmable Logic Controller)的代码时, 有2种编写模式

* **梯形图** -- 图形化可视效果好但是编写效率低
* **助记符** -- 编写效率高但是可读性差

个人的编程习惯是使用**助记符**来编写PLC的代码, 但是在写代码的过程中遇到了很多困难, 比如

1. 很难知道自己之前写的代码是做什么的, 即**逻辑表达不清晰** -- 这一点可以通过使用梯形图查看进行缓解
2. **无法记住相应的寄存器**是做什么的, 比如用输入`0`来代表停止运行, 在后边的逻辑编写中可以想起我应该有一个寄存器是用来代表停止运行这件事情, 但是想不起对应的应该是哪个数字, 特别是有很多寄存器都有其相对应的数字的情况下 -- 如果能用单词, 比如`run`来代表这个数字, 即**使用变量**这个概念, 那将带来极大的方便, 当然`CX-Programmer`中也能使用字母来代表数字, 但是每次使用都比较麻烦
3. **修改复杂, 代码量大**, 写程序时需要管理的细节多, 编程复杂度高, 比如想表达`C`等于`A`异或`B`, 用基础的`LD, OR, AND`写出来很复杂, 即使需要的信息在形式上只有`C = A ^ B`
4. **加注释困难**, 在梯形图上的注释看起来很直观, 但是在用`CX-Programmer`的助记符时, 几乎无法加入注释, 这无疑对可读性有比较大的影响

基于以上几点, 以及在当前需要完成的实验中, 逻辑表达式大量存在的前提下, 我尝试着写一个小工具来辅助自己编写PLC代码, 主要是为了减少代码编写量, 以及增加代码的可读性, 以及提供良好的抽象. 联想到之前有了解和听过的知识: **C++运算符重载**和**表达式树**, 感觉貌似可行, 说不定可以实现, 于是开始构思和code.

学校PLC实验的环境是:

- 硬件 -- `CPM2*`
- 软件 -- `CX-Programmer`

### 实现 - 原理简述

这里需要读者

* PLC入门(知道至少10个指令)
* 对C++的运算符重载有了解
* 了解二叉树遍历. 表达式树(expression tree), 可以在阅读过程中搜索资料自学

#### 对助记符的抽象

对于第一步的抽象针对下边这些指令, 由于是对于**逻辑表达式**的抽象, 所以只针对单个bit的运算, 比如输入端口`0,1,2`, 输出端口`1001,1002,1003`等单个bit的寄存器, 这里的抽象不适用于`DM`等由多个bit构成的寄存器.

| LD        | OUT        | AND        | OR        | ORLD      |
| --------- | ---------- | ---------- | --------- | --------- |
| **LDNOT** | **OUTNOT** | **ANDNOT** | **ORNOT** | **ANDLD** |

在PLC中, 如果想让输入`0`和输入`1`异或后输出到`1002`中, 

需要写这样的代码

```haskell
-- 示例PLC代码
LD 0
ANDNOT 1
LDNOT 0
ANDNOT 1
ORLD
OUT 1002
```

这5行代码中, 描述的信息是`1002 = !0 * 1 + 0 * !1`, 注意, 这里使用`*, +, !`来分别代表与, 或, 非. 如果在写代码的时候, 可以直接写`1002 = !0 * 1 + 0 * !1`而不是上边的5行代码, 在写逻辑表达式的时候效率会提高不少.

要用C++来编写这类代码, 第一步可以做什么呢? 我的想法是先实现上述表格中10条助记符的功能, 想把C++写成PLC是不可能的, 这辈子都不可能的, 也就只能`cout`呀这样子来产生PLC代码. 具体的一个例子是

```c++
void ld(int number)
{
    cout << "LD " << number << endl;
}
```

当想在PLC助记符中输入`LD 7`时, 我就可以在C++中调用`ld`这个函数, 具体是语句`ld(7)`, 然后当C++的程序运行后, 就会产生**输出**`LD 7`, 这样, 一个C++函数就和一行PLC助记符对应上了.

如法炮制, 对于PLC助记符`ANDNOT`和`ORLD`, 可以写为一个C++函数

```c++
void andnot(int number)
{
    cout << "ANDNOT " << number << endl;
}

// 注意, orld这个函数不接受参数
// 因为在PLC助记符中的ORLD也不接受参数
void orld() 
{
    cout << "ORLD" <<endl;
}
```

当在C++中调用函数`andnot(3000)`时, 会生成PLC助记符`ANDNOT 3000`.

于是对于表格中的那10条助记符, 都可以编写出一个C++的函数与其对应, 当C++程序运行完毕后, 产生相应的PLC助记符.

到这里, 前边的PLC示例代码, 可以用C++写成

```c++
ld(0);
andnot(1);
ldnot(0);
andnot(1);
orld();
out(1002);
```

这样弄有什么好处呢? 代码行数还是一样呀? 嗯, 确实目前没多大好处, 不过依赖于C++, 我们目前已经可以用变量来代表具体的数字, 以及对代码添加注释了, 就像这样

```C++
int up = 0;
int down = 1;
int wait = 1002;
// 以下实现 wait = up 异或 down
// 即 wait = !up * down + up * !down
ld(up);
andnot(down);
ldnot(up);
andnot(down);
orld();
out(wait);
```

有了注释和变量, 当需求改变, 比如想让`up`对应输入`5`, 只需要将`int up = 0;`改为`int up = 5`就以了.

注意第二行注释, 我们的目的是使得这段代码能像`wait = !up * down + up * !down`这样编写, 这也是接下来的工作.

#### 用* + ! 来写表达式

##### 表达式树 -> PLC代码

到这里就需要一些表达式树的知识了

表达式树相关的内容读者可以参考[Binary_expression_tree](https://www.wikiwand.com/en/Binary_expression_tree)

`wait = !up * down + up * !down`的表达式树为

<img src="PLC/picture/tree_wait.png" width="400">

注意到非叶子节点都是运算符, 叶子节点要么是具体的变量, 要么是这个变量的非. 如果在程序中有这样一颗表达式树, 怎样让它转为具体的PLC助记符代码呢? 

这里直接给出答案, 即**对表达式树进行后序遍历, 会碰到2类节点, 叶子和非叶子节点; 如果遍历的当前节点为非叶子节点, 就输出`ORLD或者ANDLD`, 具体是碰到`*`就输出`ANDLD`, 碰到`+`就输出`ORLD`; 如果遍历的当前节点为叶子节点, 就`LD或LDNOT`这个叶子节点, 如果比如说, 如果碰到`up`就执行`ld(up)`, 碰到`!up`就执行`ldnot(up)`**

所以, 这颗表达式树在后序遍历的执行顺序上, 会执行

```haskell
ldnot(up);
ld(down);
andld();
ld(up);
ldnot(down);
andld();
orld();
```

生成的PLC代码为

```haskell
LD 0
LDNOT 1
ANDLD
LDNOT 0
LD 1
ANDLD
ORLD
```

看着代码有些不对劲, 因为少了`OUT`, 但是没关系, 在C++里再加入一句`out(wait)`就行了.

##### C++表达式 -> 表达式树

现在知道了一颗表达式树如何转为相同逻辑的PLC代码, 那么主要的问题就是怎么生成这颗表达式树了.

这时候需要引入数据结构的知识, 以及C++ (换为任意一门其它的编程语言也可以, 只要有重载这类操作) 的运算符重载了,**引入一个数据类型`PLCEXP`**来代表PLC的表达式, `PLCEXP`是PLC expression的英文缩写.

从表达式树来看, 需要重载的运算符仅有3个, 即`!, *`和`+`, 这里为了简洁性**先不考虑`!`.** 我们知道一颗表达式树可以仅由单个叶子构成, 那就从这单个叶子开始吧, 假设我们有两颗表达式树`A`和`B`, 现在知道`C = A * B`, 怎么生成`C`对应的表达式呢? 示例图如下.

<img src="PLC/picture/andLeaf.png">

对应的C++重载运算符`*`为

```C++
friend PLCEXP operator * (const PLCEXP &a, const PLCEXP &b) // and
{
	PLCEXP head('*', a, b); // 新树
	return head;
}
```

这里并没有显示底层的指针操作, 只是调用了封装好的构造函数, 这个构造函数的声明为`PLCEXP(char ope,PLCEXP left,PLCEXP right) `, 作用是创建一个父节点, 这个父节点的左孩子设置为参数里的`left`, 右孩子设置为参数里的`right`, 运算符设置为传入的字符, 然后返回这个父节点. 下边的重载也使用了这个构造函数.

如果要`C = A + B也是类似的原理`

<img src="PLC/picture/orleaf.png">

对应的C++函数重载为

```C++
friend PLCEXP operator + (const PLCEXP &a, const PLCEXP &b) // or
{
	PLCEXP head('+',a,b); // 新树
	return head;
}
```

但是如果要`C = (A * B) + (A + B)`呢? 只需要再进行组合就行了.如下图

<img src="PLC/picture/ortree.png">

以上的这些操作可以理解为二叉树的合并, 即将**运算符的左边和右边分别当成左孩子和右孩子, 运算符当成树根节点, 然后组成一颗新的二叉树**.

还有`!`这个符号的重载, 当`!`作用到这颗表达式树的时候, 由布尔运算中的德摩根定律, 可以对二叉树进行一次遍历, 将所有非叶子节点里的运算符取成相反的运算符, 即`* -> +`和`+ -> *`, 碰到非叶子节点就对节点取反, 细节上需要对二叉树进行递归. 值得注意的是, 可以用`pair<bool,int>`来表示一个叶子节点, 然后统一对叶子节点的`LD, OUT, AND`等操作.比如对于`LD`

```c++
template<typename T>
pair<bool,T> ld(pair<bool,T> it)
{
	if(it.first) // 如果为true, 说明不用取反
		ld(it.second);
	else // 如果为false, 调用ldnot
		ldnot(it.second);
	return it;
}
```

在碰到`false`的`pair<bool, T> `时, 可以调用响应的`ldnot()`来达到统一行为, 减少编程复杂度的目的.

整套具体的代码在编写好的头文件`PLC.h`中, 可以采用很多种方法来实现. 我在实现上有很多缺陷, 有很多可以改进的地方.

### 具体使用方法

上述为原理的讲述. 

严格来说这并不能算是一个内嵌领域专用语言(DSL), 因为缺少了很多东西, 但是实在是不知道把这个工具归类到哪里, 于是选了和它看起来最接近的领域, 即**领域专用语言(domain specific language / DSL)**, 由于是在C++内部实现的, 所以加上了**内嵌**这2个字.

#### 简单的例子

这里开始, 是如何调用API来使用.

需要声明PLC表达式, 然后用PLC表达式进行运算.

* `*`代表`and`
* `+`代表`or`
* `!`代表取反
* `>>`代表将左边表达式值输出到右边

对于`>>`这个运算符, 它取代了函数`out`, 将`out`函数的功能用`>>`来替代, 作用是将`>>` 右边的表达式`out`(假设这个表达式只有一个叶子节点), 使代码可以更简洁.

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

##### 生成的PLC代码

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

##### 相应的PLC梯形图

![](PLC/picture/xor(a,b).PNG)

#### 更复杂的逻辑表达式

实现`!(A+B * !D+C * !D)`输出到`E`.

我们知道`!(A+B * !D+C * !D) = !A * !(B * !D) * !(C * !D) = !A * (!B + D) * (!C + D) `


$$
! \ (\overline{ A+B * \overline D+C * \overline D }) = \overline A \ * \  \overline {(B * \overline D)} \ * \  \overline {(C * \overline D)} = \overline A * ( \overline  B + D) * ( \overline C + D)
$$


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

##### 生成的PLC代码

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

##### 相应的PLC梯形图

![](PLC/picture/2.PNG)

#### 仅仅是数字吗?

上述的介绍与示例中, 只用到了对应的IO口, 也就是数字, 但实际上在对PLC的编程中往往还有计时器`TIM`, 计数器`CNT`, 触发器`keep`这些指令. 得益于C++的**函数重载以及模板**, 可以将PLCEXP的叶子节点里的数据类型换成`pair<bool,string>`

这样针对与`TIM`这些指令, 在PLC助记符中`LD TIM1`对应的C++函数可以在原来的`void ld(int nubmer)`上增加一个新的函数`void ld(string it)`, 然后传入字符串`"TIM1"`即可, 也就是需要在定义数据的时候注意一下, 

比如可以定义一个变量`string tim1 = "TIM1"`, 然后`ld(tim1)`就可以在程序运行的时候输出`LD TIM1`这个助记符了.

继承于这个特性, 

```haskell
LD 3
TIM 3 #10
LD 5
AND TIM3
OUT 3000
```

可以写成

```C++
#include<iostream>
#include "PLC.h" // 包含PLC.h头文件, 放在程序相同目录下
using namespace std;

// 创建PLC表达式
PLCEXP three = 3;
PLCEXP five = 5;
PCCEXP tim3 = "TIM3"; // 时钟
PLCEXP temp = 3000;
int main()
{
    // 写逻辑运算的表达式
    tim(three,tim3,10); // 时钟函数
    five * tim3 >> temp; // 将带有时钟的表达式输出到3000
    return 0;
}
```

通过这种方式, 原先的函数就可以继承原来的功能, 处理不同的数据. 如果更复杂一点, 还可以使用C++的继承, 从PLCEXP继承一个名字为TIM的新类, 这个新的类可以继承原来的方法, 进而达到相同的行为.

### 实用性?

我个人在PLC编程过程中对于逻辑的处理倾向是使用**状态机**, 一旦状态机写好后, 将其翻译成对应的表达式, 从而满足相应的设计需求, 这个工具在辅助上是缩短**"翻译状态机"**的时间, 以及方便程序改动. 

但是类似的工作也可以通过`CX-Programmer`的GUI编程, 即梯形图, 说不定在GUI转为助记符的过程中, 背后的代码也是针对于二叉树的操作. 

在学习成本上, 这个工具花费的时间比梯形图要高出不少, 梯形图可能对于PLC新手来说一下子就可以入门, 并且可以开始写程序了, 但是这个小工具感觉没那么简单, 相对与梯形图的直观, 鼠标操作, 这个工具晦涩很多, 而且在使用上还需要会一些C++的知识, 可能在效率上还提高不了多少

在编程效率上, 由于我个人没有使用过梯形图进行编程, 所以在效率上无法进行对比.