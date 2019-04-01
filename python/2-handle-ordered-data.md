# 1. 概述
任何程序都离不开处理数据，python也不例外，为了处理数据，通常需要合理的组织数据，python在这方面很出色，这得益于它提供了一组广泛应用的数据结构：列表，字典，元组，集合。本章先简要介绍这几种数据结构。并详细研究列表。下一章详细研究其他三种数据结构。  

# 2. 基本认识
python中一切都是对象，所有对象都可以赋给变量。你可以直接给变量赋值，python会取所赋的那个值的类型。这么说python是面向对象的了？算是吧，可以采用面向对象的方式使用类、对象、实例等编写python程序(后边会介绍)，不过不是非得这么做。python更像是基于对象的。面向对象是可选的。  

python中，任何东西都可以赋给任何变量。变量可以赋为任何东西（数字，字符串，函数，小部件......）。  

下边看看python为存储值集合提供的数据结构。  

# 3. python的4个内置数据结构
python的四个内置的数据结构有：列表、元组、字典、集合。之所以说它们是内置的是因为在使用之前无需导入，是python语言的一部分。  
1. 列表：有序的可变对象集合。类似于其他语言的数组，下标从0开始。不过它可以自动扩张或者搜索。而且在**一个列表中可以包含不同类型的对象**，这一点和其他语言的数组/列表不同。   
2. 元组：有序的不可变对象集合。元组是一个不可变的列表，一旦向一个元组赋了对象，任何情况下，这个元组都不能再改变。可以把元组看成是一个常量列表。下标也从0开始。  
3. 字典：无序的键值对集合。也可以自动扩展和收缩。你也许知道关联数组、映射、散列这些名字，字典和它们是类似的。与键关联的值可以是任何对象。在字典中增加键值对时会有一个顺序，但是字典不会保持这个顺序。不过放心了，如果需要，完全可以用某个特定的顺序显示你的字典数据。  
4. 集合：无序的唯一(不重复)对象集合。集合可以确保其中的对象不重复。你还可以计算集合的交，并，差。也可以自动扩展和收缩。  

本章主要学习列表。  

# 4. 列表
## 4.1 创建列表
```py
prices = []
temps = [32.0, 212.0, 0.0, 81.6, 100.0, 45.3]
words = ['hello', 'world']
car_details = ['Toyota', 'RAV4', 2.2, 60807]
everything = [prices, temps, words, car_details]
odds_and_ends = [ [1, 2, 3], ['a', 'b', 'c'], ['one', 'two', 'three'] ]
```
## 4.2 让列表开始工作
确定一个给定的单词中是否包含某个元音字母。  
```py
vowels = ['a', 'e', 'i', 'o', 'u']
word = "Milliways"
for letter in word:
    if letter in vowels:
        print(letter)
```
这里用到了in操作符来检查一个列表中是否包含某个对象。也可以检查某个对象不在列表里,使用：not in。  
下边改造我们的代码，如上会输出两个i一个a。如果我们只想输出i和a呢？  
这时需要我们先把word中的原因字母存储起来，并且不能重复。代码如下：  
```py
vowels = ['a', 'e', 'i', 'o', 'u']
word = "Milliways"
found = [] # 定义一个空列表
for letter in word:
    if letter in vowels:
        if letter not in found: # 判断对象不在列表中   这里使用集合其实更好，后边我们学习了再来看。
            found.append(letter) # 运行的时候向列表追加对象，扩展列表。
for vowel in found:
    print(vowel)
print(len(found)) # len函数输出列表长度。

"""
输出：
i
a
2
"""
```
上边我们的单词是提前写好的，不过灵活，我们需要输入单词，这时需要将：  
```py
word = "Milliways"
```
改为：  
```py
word = input("Provide a word to search for vowels: ")
```
即可。  
上边我们知道了如何向列表中追加对象，接下来学习操作列表的四个非常重要的函数：remove、pop、extend、insert。    

1. remove：取一个对象值作为唯一参数。将这个值在列表中第一次出现的对象删除，同时列表的大小减一。如果没有这个值，解释器会报错（后边详细说）。  
2. pop：根据给出的索引值从列表中删除一个对象。如果没有指定索引值，将删除并返回列表中的最后一个对象。如果指定了索引值，就删除并返回指定位置上的对象。如果列表为空，或者索引值不存在，解释器会报错。  
3. extend：一个对象列表作为唯一参数，可以将两个列表合并为一个列表。这个与append还是有区别的。nums.append([])会在列表nums中增加一个空列表对象，而nums。nums.extend([])什么也不做。  
4. insert：append和extend用于在列表尾部增加对象。用于将对象插入现有列表的任何位置。参数为一个索引值和一个对象。这个对象将插入到指定索引值的前面。  

**看个例子：把"Don't panic!"抓换为"on tap" panic.py**  
```py
phrase = "Don't panic!"
plist = list(phrase)
print(phrase) 
print(plist)

for i in range(4):
    plist.pop()
plist.pop(0)
plist.remove("'")

plist.extend([plist.pop(), plist.pop()]) # 先pop后extend
plist.insert(2, plist.pop(3)) # 先pop后insert

new_phrase = ''.join(plist) # 列表转换为一个字符串
print(plist)
print(new_phrase)

# 输出
"""
Don't panic!
['D', 'o', 'n', "'", 't', ' ', 'p', 'a', 'n', 'i', 'c', '!']
['o', 'n', ' ', 't', 'a', 'p']
on tap
"""
```
以上是列表最基本的用法，接下来看看别的用法以及需要注意的东西。特别是将一个列表复制到另一个列表时。   
看起来像复制，其实不是：  
```py
first=[1,2,3,4,5]
first
[1, 2, 3, 4, 5]
second=first
second
[1, 2, 3, 4, 5]
second.append(6)
second
[1, 2, 3, 4, 5, 6]
first
[1, 2, 3, 4, 5, 6]
```  
可见，赋值其实拷贝的是引用，而非列表中的对象。要复制列表中的对象，应该这么做：  
```py
third=second.copy()
third
[1, 2, 3, 4, 5, 6]
third.append(7)
third
[1, 2, 3, 4, 5, 6, 7]
first
[1, 2, 3, 4, 5, 6]
second
[1, 2, 3, 4, 5, 6]
```

我们一直在说prthon中的列表就像其他语言中的数组。这是因为确实prthon中访问列表中的值时使用[]。不过python也有很多与数组不同的地方：  
 - python的列表认识正索引值，从0开始。也认识负索引值。负索引值从相反的方向开始。这时，总是可以通过list[0]和list[-1]来获取开始和结束的两个对象。  
 - prthon对中括号的扩展不仅仅支持负数，还包括start，stop，step的概念。不过不包括stop这个索引。如果没有指定start，则start==0，如果没有指定stop，则默认值为列表允许的最大值，如果没有指定步长，默认值为1 。   
使用这些特性的例子如下：  
```py
saying="Don't panic!"
letters=list(saying) # 字符串转换为一个列表
letters
['D', 'o', 'n', "'", 't', ' ', 'p', 'a', 'n', 'i', 'c', '!']
letters[0]
'D'
letters[3]
"'"
letters[-1]
'!'
letters[-3]
'i'
letters[-6]
'p'
letters[0:10:3]
['D', "'", 'p', 'i']
letters[3:]
["'", 't', ' ', 'p', 'a', 'n', 'i', 'c', '!']
letters[3::]
["'", 't', ' ', 'p', 'a', 'n', 'i', 'c', '!']
letters[:10]
['D', 'o', 'n', "'", 't', ' ', 'p', 'a', 'n', 'i']
letters[::10]
['D', 'c']
letters[::2]
['D', 'n', 't', 'p', 'n', 'c']
```

再看一些例子：  
```py
book="The Hitchhiker's Guide to the Galaxy"
booklist=list(book)
booklist
['T', 'h', 'e', ' ', 'H', 'i', 't', 'c', 'h', 'h', 'i', 'k', 'e', 'r', "'", 's', ' ', 'G', 'u', 'i', 'd', 'e', ' ', 't', 'o', ' ', 't', 'h', 'e', ' ', 'G', 'a', 'l', 'a', 'x', 'y']
''.join(booklist[-6::])
'Galaxy'
backwards=booklist[::-1]
''.join(backwards)
"yxalaG eht ot ediuG s'rekihhctiH ehT"
backwardsnew=booklist[-1::-1]
''.join(backwardsnew)
"yxalaG eht ot ediuG s'rekihhctiH ehT"
''.join(booklist[13:3:-1])
'rekihhctiH'
```
**实际上，切片无处不在，你会看到很多使用[start:stop:step]截取python中的任意序列**  

**接下来使用列表切片([start:stop:step])的方式来将上边把"Don't panic!"抓换为"on tap"的例子实现panic2.py：**  
```py
phrase = "Don't panic!"
plist = list(phrase)
print(phrase) 
print(plist)

new_phrase = ''.join(plist[1:3])
new_phrase = new_phrase + ''.join([plist[5], plist[4], plist[7], plist[6]])

print(plist)
print(new_phrase)

# 输出
"""
Don't panic!
['D', 'o', 'n', "'", 't', ' ', 'p', 'a', 'n', 'i', 'c', '!']
['D', 'o', 'n', "'", 't', ' ', 'p', 'a', 'n', 'i', 'c', '!']
on tap
"""
```
可以看到，这个输出与上一个不同，上一个是：   
```py
"""
Don't panic!
['D', 'o', 'n', "'", 't', ' ', 'p', 'a', 'n', 'i', 'c', '!']
['o', 'n', ' ', 't', 'a', 'p']
on tap
"""
```  
哪一个好呢？  
其实这里的核心在于列表的方法：append，exend，pop，remove，insert改变了列表的内部结构和状态，而[start, stop, step]的方式不会改变列表的状态。哪个好取决于你想要什么效果。

for循环对切片也很了解，例如：  
```py
paranoid_android = "Marvin, the Paranoid Android"
letters = list(paranoid_android)
for char in letters[:6]:
    print('\t', char)
print()
for char in letters[-7:]:
    print('\t'*2, char)
print()
for char in letters[12:20]:
    print('\t'*3, char)
# 输出：
"""
	 M
	 a
	 r
	 v
	 i
	 n

		 A
		 n
		 d
		 r
		 o
		 i
		 d

			 P
			 a
			 r
			 a
			 n
			 o
			 i
			 d
"""
```
前面学习了列表的基本用法，后边还会学习列表，比如：列表排序等等。  
列表在程序中会被大量使用，不过也有不适合列表的地方。**如果你想存储的数据有一种明确的结构，就要考虑其他的数据结构了。**  
比如，下边的数据就更加适合使用**字典**来存储。下一章我们会进行学习。  
```py
"""
Name:Ford Prefect
Gender:Male
Occupation:Researcher
Home Planet:Betelgeuse Seven
"""
```

本章的学习到此结束，接下来学习第三章，元组、字典、集合，另外三种重要的内置数据结构。  
