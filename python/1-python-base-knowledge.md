# 1 开门见山
```py
# 第一句从python标准库的datetime模块中导入了子模块，该子模块也叫datetime
"""
第一句从python标准库的datetime模块中导入了子模块
该子模块也叫datetime
"""
from datetime import datetime

odds = [1,  3,  5,  7,  9, 11, 13, 15, 17, 19,
       21, 23, 25, 27, 29, 31, 33, 35, 37, 39,
       41, 43, 45, 47, 49, 51, 53, 55, 57, 59]

right_this_minute = datetime.today().minute
if right_this_minute in odds:
    print("This minute seems a little odd.")
else:
    print("Not an odd minute.")
```
python是解释执行的。解释器会一次一条语句的解释执行你的代码。python的单行注释是使用 #，多行注释是三个单引号 """ 注释内容 """。  
prthon中，函数是模块的一部分，模块是标准库的一部分。又如：  
```py
# 从模块os中导入一个函数
from os import getcwd

# 依据需要调用
print(getcwd())
```
python的标准库提供了大量可重用的代码模块，可以帮助你完成一系列的工作。但是我怎么知道标准库里的某个模块能干什么呢?  
python文档给出了所有答案。地址是： https://docs.python.org/3/library/index.html  

除了标准库，python社区还提供了超级棒的第三方模块集合。见： http://pypi.python.org  

```py
odds = [1,  3,  5,  7,  9, 11, 13, 15, 17, 19,
       21, 23, 25, 27, 29, 31, 33, 35, 37, 39,
       41, 43, 45, 47, 49, 51, 53, 55, 57, 59]
```
python中很多数据结构是内建的，入上就是一个列表，**通常而言，python中，一行结束就标志着一条语句结束**，不过这个一般规律也有例外，比如我们上边看到的多行列表就是其中例外之一。  

python与其他静态语言如C/C++这些不同，变量不需要显式声明类型。如上，odds就是一个列表。right_this_minute就是一个变量，其值为函数调用的结果。

datetime.today().minute 中，我们调用了子模块的方法today, 因为后边有()，所以知道是个函数调用，该函数返回一个时间对象，我们通过点记法来获取对象中成员的值。  

其实right_this_minute = datetime.today().minute和下列语句等价：  
```py
time_now = datetime.today()
right_this_minute = time_now.minute
```
在python中，除了常见的操作符(<, >, =, <=, >=等)，还提供了自己的一些超级操作符，例如：in操作符非常强大，它可以确定一个对象是不是在另一个对象里。in操作符的结果为true/false。  

python中的代码块很容易发现，他们总是缩进的。这也是python和其他语言不同的地方，python中的代码块不通过大括号{}来区分。python使用缩进来区分代码块（缩进是python提供的唯一代码分组机制）。在python中通常称为代码组。python中的大括号也有用，通常用来分隔数据（后边会看到）。另外就是:, 冒号这个字符用来引入与某个python控制语句(if, else, for等)关联的必须向右缩进的代码组。我们会看到大量的例子。如果忘记在冒号后缩进代码，解释器会报错。如：  
```py
if right_this_minute in odds:
print("This minute seems a little odd.")
else:
    print("Not an odd minute.")
```
报错  
```
print("This minute seems a little odd.")
        ^
IndentationError: expected an indented block
```  
**注意：if和else后都有冒号来引入代码块，这个别忘了**  
有else，是不是也有else if呢？是的，不过在python中是 elif。如：  
```py
if today == "Saturday":
    print("Party!!")
elif today == "Saturday":
    print("Recover!!")
else:
    print("Work, Work, Work!!")
```

代码组可以包含嵌套代码组。例如：  
```py
if today == 'Saturday':
    print('Party!')
elif today == 'Sunday':
    if condition == 'Headache':
        print('Recover, then rest.')
    else:
        print('Rest.')
else:
    print('Work, work, work.')
```  

# 2 扩展程序
```py
from datetime import datetime

odds = [1,  3,  5,  7,  9, 11, 13, 15, 17, 19,
       21, 23, 25, 27, 29, 31, 33, 35, 37, 39,
       41, 43, 45, 47, 49, 51, 53, 55, 57, 59]

right_this_minute = datetime.today().minute
if right_this_minute in odds:
    print("This minute seems a little odd.")
else:
    print("Not an odd minute.")
```
接下来要做三件事：
 - 1、让判断分钟的if/else循环五次；
 - 2、暂停执行指定的秒数；
 - 3、暂停执行随机的秒数；

第一件事有多种方法，你可以从python的shell中试验一下，看看怎么做到。  
这里我们使用for循环。循环次数已知时使用for，循环次数未知时推荐while。for循环的三种典型方式如下：  
```py
for i in [1, 2, 3]:
    print(i)

1
2
3

for ch in "hi!":
    print(ch)
    
h
i
!

for num in range(5):
    print("head first rocks!")
    
head first rocks!
head first rocks!
head first rocks!
head first rocks!
head first rocks!

for num in range(5):
    print(num)
    
0
1
2
3
4
```
接下来看看使我们的代码循环起来:  
```py
from datetime import datetime

odds = [1,  3,  5,  7,  9, 11, 13, 15, 17, 19,
       21, 23, 25, 27, 29, 31, 33, 35, 37, 39,
       41, 43, 45, 47, 49, 51, 53, 55, 57, 59]
for num in range(5):
    right_this_minute = datetime.today().minute
    if right_this_minute in odds:
        print("This minute seems a little odd.")
    else:
        print("Not an odd minute.")
```
如上，要注意原来代码的缩进。如下代码其实也是等价的： 
```py 
for num in [3,4,5,6,7]:
for ch in "asdjg":
```
接下来看看如何暂停执行指定的秒数。这可以使用sleep函数，来自于模块time。为了使用函数sleep需要先导入对应的模块：  
```py
import time
time.sleep(5)

或者
from time import sleep
sleep(6)
```
接下来完成第三个目标，这个需要使用python的随机数模块random。每次我们都要查python文档？其实不是，可以通过python shell中输入命令dir来查看与某个东西相关的属性：  
```py
import random
dir(random)
['BPF', 'LOG4', 'NV_MAGICCONST', 'RECIP_BPF', 'Random', 'SG_MAGICCONST', 'SystemRandom', 'TWOPI', '_BuiltinMethodType', '_MethodType', '_Sequence', '_Set', '__all__', '__builtins__', '__cached__', '__doc__', '__file__', '__loader__', '__name__', '__package__', '__spec__', '_acos', '_bisect', '_ceil', '_cos', '_e', '_exp', '_inst', '_itertools', '_log', '_os', '_pi', '_random', '_sha512', '_sin', '_sqrt', '_test', '_test_generator', '_urandom', '_warn', 'betavariate', 'choice', 'choices', 'expovariate', 'gammavariate', 'gauss', 'getrandbits', 'getstate', 'lognormvariate', 'normalvariate', 'paretovariate', 'randint', 'random', 'randrange', 'sample', 'seed', 'setstate', 'shuffle', 'triangular', 'uniform', 'vonmisesvariate', 'weibullvariate']

dir(random.randint)
['__call__', '__class__', '__delattr__', '__dir__', '__doc__', '__eq__', '__format__', '__func__', '__ge__', '__get__', '__getattribute__', '__gt__', '__hash__', '__init__', '__init_subclass__', '__le__', '__lt__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__self__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__']

```

dir列出了大量的函数，想要看看**该函数的帮助文档**，可以使用help：  
```py
help(random.randint)
Help on method randint in module random:
randint(a, b) method of random.Random instance
    Return random integer in range [a, b], including both end points.
```
帮助文档很明确，该函数产生一个位于[a, b]之间的随机数，包括a和b。  

有了这些知识，可以修改我们的程序了。  
```py
from datetime import datetime
import random
import time

odds = [1,  3,  5,  7,  9, 11, 13, 15, 17, 19,
       21, 23, 25, 27, 29, 31, 33, 35, 37, 39,
       41, 43, 45, 47, 49, 51, 53, 55, 57, 59]

for num in range(5):
    right_this_minute = datetime.today().minute
    if right_this_minute in odds:
        print("This minute seems a little odd.")
    else:
        print("Not an odd minute.")
    wait_time = random.randint(1, 60)
    time.sleep(wait_time)
```

# 3. 编写一个商业应用
```py
word = "bottles"
for beer_num in range(99, 0, -1):
    print(beer_num, word, "of beer on the wall.")
    print(beer_num, word, "of beer.")
    print("Take one down.")
    print("Pass it around.")
    if beer_num == 1:
        print("No more bottles of beer on the wall.")
    else:
        if (beer_num - 1) == 1:
            word = "bottle"
        print(beer_num - 1, word, "of beer on the wall.")
    print()

......
3 bottles of beer on the wall.
3 bottles of beer.
Take one down.
Pass it around.
2 bottles of beer on the wall.

2 bottles of beer on the wall.
2 bottles of beer.
Take one down.
Pass it around.
1 bottle of beer on the wall.

1 bottle of beer on the wall.
1 bottle of beer.
Take one down.
Pass it around.
No more bottles of beer on the wall.
```
这次我们注意到了range的用法和之前不同了，查文档？？不不，我们通过python shell看看range的帮助文档。  
```py
help(range)
Help on class range in module builtins:
class range(object)
 |  range(stop) -> range object
 |  range(start, stop[, step]) -> range object
 |  
 |  Return an object that produces a sequence of integers from start (inclusive)
 |  to stop (exclusive) by step.  range(i, j) produces i, i+1, i+2, ..., j-1.
 |  start defaults to 0, and stop is omitted!  range(4) produces 0, 1, 2, 3.
 |  These are exactly the valid indices for a list of 4 elements.
```
看来找到了我们需要的知识了。  
range可以使用一个参数调用，也可以使用三个参数调用。当为三个参数时，表示为 start，stop，step。含义如下：  
- 1、start：如果使用一个参数，range的默认起始从0开始。当然你也可以提供一个你希望的值。如果设置了起始值就必须设置结束值。  
- 2、stop：结束值，生成的范围并不包含结束值。例如：range(5)，就不包含5.  
- 3、step: 指定开始和结束值时还可以指定一个步长参数。默认步长为1。步长可以为任意值，负数也可以。  

下边做些range的试验：  
```py
>>> range(5)
range(0, 5)
>>> list(range(5))
[0, 1, 2, 3, 4]
>>> list(range(0))
[]
>>> list(range(-1))
[]
>>> list(range(0, 12, 2))
[0, 2, 4, 6, 8, 10]
>>> list(range(0, 11, 2))
[0, 2, 4, 6, 8, 10]
>>> list(range(0, 13, 2))
[0, 2, 4, 6, 8, 10, 12]
>>> list(range(0, 13, -2))
[]
```

# 4. 小结
本节总结几个核心的点：  
 - prthon使用缩进表示代码组；  
 - 缩进时，不要混用制表符和空格；  
 - range可以设施开始，结束和步长；  
 - 准备引出代码组的时候别忘记冒号(:)[for，if，else，elif等]；  
 - 学会使用dir查看某个模块的相关属性，使用help查看函数的帮助文档；  
 - 学会使用python shell来试验自己想要的功能；  
 - 知道次数的循环推荐用for，未知次数的循环用while；  
 - python中没有main函数的概念，它一行一行的由python解释器解释执行；  


