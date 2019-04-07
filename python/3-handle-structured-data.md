# 1. 概述
上一章学习了python的内置数据类型列表。这一章学习剩下的三个内置数据类型：字典、元组、集合。  

字典的键通常是字符串，值部分可以是任意的python对象。当然了键也可以是别的类型。下边通过一个例子来看看怎么使用字典。  
之前我们有个程序是显示找到的元音字母，不过现在我们还想要统计元音字母出现的次序，怎么做呢？列表貌似不太好实现，看看字典就很方便。为了使用字典实现这个目的，我们先学习一些字典的基本知识。  

# 2. 字典
**发现代码中的字典**  
```py
person = {'Name': 'Ford Prefect', 'Gender': 'Male', 'Occupation': 'Researcher', 'Home Planet': 'Betelgeuse Seven'}
>>>person
# 输出
"""
{'Gender': 'Male', 'Name': 'Ford Prefect', 'Occupation': 'Researcher', 'Home Planet': 'Betelgeuse Seven'}
"""
>>>person['Name']
# 输出
"""
'Ford Prefect'
"""
```  
这一行代码中有如下几点：  
 - 开始大括号{表示字典的开始；  
 - 键和值之间用冒号分开，在这个字典中，键和值都是字符串；  
 - 键值对之前用逗号分隔；  
 - 结束大括号}表示字典的结束;  
 - 字典不保证插入的顺序；  
 - 字典中的值通过中括号和键的值来获取。  

也可以在运行时处理字典，例如在person中增加一个键值对：  
```py
person['Age'] = 33
>>>person
# 输出
"""
{'Name': 'Ford Prefect', 'Gender': 'Male', 'Occupation': 'Researcher', 'Home Planet': 'Betelgeuse Seven', 'Age': 33}
"""
```   
如何迭代处理字典呢？  
```py
for k in person:
    print(k)
# 输出
"""
Name
Gender
Occupation
Home Planet
Age
"""
```
注意，以上方式只是输出了key。要想便利键和值应该这么做:  
```py
for k in person:
    print(k , ' = ', person[k])
# 输出
"""
Name  =  Ford Prefect
Gender  =  Male
Occupation  =  Researcher
Home Planet  =  Betelgeuse Seven
Age  =  33
"""

for k,v in person.items():
    print(k, ' = ', v)
# 输出
"""
Name  =  Ford Prefect
Gender  =  Male
Occupation  =  Researcher
Home Planet  =  Betelgeuse Seven
Age  =  33
"""

"""
for k,v in person:
    print(k, ' = ', v)
将会报错。
"""

# 如果需要按照键的顺序排序输出呢？可以这么做：  

for k in sorted(person):
    print(k, ' = ', person[k])
"""
Age  =  33
Gender  =  Male
Home Planet  =  Betelgeuse Seven
Name  =  Ford Prefect
Occupation  =  Researcher
"""
for k,v in sorted(person.items()):
    print(k, ' = ', v)
"""
Age  =  33
Gender  =  Male
Home Planet  =  Betelgeuse Seven
Name  =  Ford Prefect
Occupation  =  Researcher
"""
```
在使用sorted排序输出时，sorted并不会改变原有字典的顺序，而是会构造一个原有字典的副本。  

接下来看看如何统计元音出现的次数吧，程序如下:   
```py
vowels = ['a', 'e', 'i', 'o', 'u']
word = input("Provide a word to search for vowels: ")

found = {} # {}表示found为一个空字典
# 初始化键关联的值。
found['a'] = 0
found['e'] = 0
found['i'] = 0
found['o'] = 0
found['u'] = 0

for letter in word:
    if letter in vowels:
        found[letter] += 1 # python中没有++ 和--操作符
        
for k, v in sorted(found.items()):
    print(k, 'was found', v, 'time(s).')

# 输出
"""
Provide a word to search for vowels: ruikjhgfkh
a was found 0 time(s).
e was found 0 time(s).
i was found 1 time(s).
o was found 0 time(s).
u was found 1 time(s).

>>>====================================================
再次运行
Provide a word to search for vowels: sky
a was found 0 time(s).
e was found 0 time(s).
i was found 0 time(s).
o was found 0 time(s).
u was found 0 time(s).
"""
```
上边的例子中，即使某个元音字母没有被找到也会被打印出来，有时候，会希望只有真正找到结果时才会显示，也就是说，不想看到：found 0 time(s)。该如何解决呢？？   
python的字典不是动态的吗？我们是不是删除了  
```py
found['a'] = 0
found['e'] = 0
found['i'] = 0
found['o'] = 0
found['u'] = 0
```  
**就可以呢？看看可以吗：**  
```py
vowels = ['a', 'e', 'i', 'o', 'u']
word = input("Provide a word to search for vowels: ")

found = {}

for letter in word:
    if letter in vowels:
        found[letter] += 1
        
for k, v in sorted(found.items()):
    print(k, 'was found', v, 'time(s).')
# 输出
"""
Provide a word to search for vowels: gyiuiuiuyiyu
Traceback (most recent call last):
  File "F:/mygithub/xufubo-blog/python/python_study_code/ch03/vowels5.py", line 9, in <module>
    found[letter] += 1
KeyError: 'i'

再次运行：
Provide a word to search for vowels: fffgh
"""
```
可见，是 found[letter] += 1 引发的错误。导致了程序的崩溃。不是python的字典可以动态扩展吗？  
这是因为：**字典的键必须被初始化**，如果试图访问键不存在的关联值就会产生KeyError。那么该怎么办呢？？  

前面我们知道： 如果试图访问键不存在的关联值就会产生KeyError，为了避免这个问题，我们可以使用in操作符来判断字典中是否存在某个键来避免KeyError。  
例如：  
```py
>>>fruits = {}
>>>fruits
{}
>>>fruits['apples']=10
>>>fruits
{'apples': 10}
>>>'apples' in fruits
True
```  
**注意：python中使用True和False表示布尔值。首字母大写哈**  
再看一个使用in检查成员关系的例子:  
```py
if 'bananas' in fruits:  # 避免了KeyError错误
    fruits['bananas'] +=1
else:
    fruits['bananas'] = 1
fruits

# 输出 
'''
{'apples': 10, 'bananas': 1}
'''

# 再次执行，输出： 
"""
{'apples': 10, 'bananas': 2}
"""
```
因此要确保python字典的键在使用前一定要初始化。  
我们知道，not in可以用于列表，其实也可以用于字典。  
```py
if 'pears' not in fruits:
    fruits['pears'] = 0
fruits['pears'] +=1
fruits
# 输出：
"""
{'apples': 10, 'bananas': 2, 'pears': 1}
"""
```
在python中，if/not in的使用实在太常见了，为此，python提供了一个setdefault方法来替代if/not in的两行，以下的代码和上边的等价：  
```py
fruits.setdefault('pears', 0)   #初始化(如果键不在字典中，则初始化，否则什么也不做)
fruits['pears'] +=1
fruits
# 输出：
"""
{'apples': 10, 'bananas': 2, 'pears': 2}
"""
```
**有了以上基础就可以实现我们的元音统计了：**  
```py
vowels = ['a', 'e', 'i', 'o', 'u']
word = input("Provide a word to search for vowels: ")

found = {}

for letter in word:
    if letter in vowels:
        found.setdefault(letter, 0)  # 加入一行代码，使得程序大不相同
        found[letter] += 1
        
for k, v in sorted(found.items()):
    print(k, 'was found', v, 'time(s).')
```
这里使用了setdefault解决了KeyError问题。  

到这里，字典的学习先告一段落，不过字典的学习还未结束。前面我们说过字典的值部分是一个对象，这个对象也可以是列表或者字典。后边我们会进行学习。接下来看看剩下的两个内置数据结构: 元组和集合。先从集合开始学习。  

# 3. 集合
集合与我们在学校学习的数学中的集合很像，关键特性就是不允许有重复值。  
在python中，集合对象之间使用逗号分隔，被大括号{}包围，例如下边就是一个集合：  
```py
vowels = {'a', 'e', 'i', 'o', 'u'}
```
区分集合与字典的关键是是否用冒号:来分割键和值。**与字典一样，集合也不会维持插入的顺序，不过与所有数据结构类似，集合可以使用sorted函数对输出排序。**集合可以像数学中的概念一样，可以计算集合的交集、并集和差集。记得之前在第二章我们就说过，使用集合统计元音字母，现在就来这么做。  
先看看之前的代码：  
```py
vowels = ['a', 'e', 'i', 'o', 'u']
word = input("Provide a word to search for vowels: ")
found = [] # 定义一个空列表
for letter in word:
    if letter in vowels:
        if letter not in found: # 判断对象不在列表中   这里使用集合其实更好，后边我们学习了再来看。
            found.append(letter) # 运行的时候向列表追加对象，扩展列表。
for vowel in found:
    print(vowel)
```
为了使用集合，我们先学习点基本知识。  

先创建一个集合：  
```py
>>>vowels2 = set('aaeiouuu')
>>>vowels2
{'e', 'u', 'a', 'o', 'i'}
```
充分利用集合的方法：  
1. 求并集：union  
2. 求差集：difference   
3. 求交集：intersection  
举例如下：   
```py
vowels = set('aeiou')
word = 'hello'
u = vowels.union(set(word))
u
"""
{'a', 'h', 'l', 'i', 'u', 'e', 'o'}
"""
word
"""
'hello'
"""
vowels
"""
{'a', 'i', 'u', 'e', 'o'}
"""
d = vowels.difference(set(word))
d
"""
{'a', 'u', 'i'}
"""
word
"""
'hello'
"""
vowels
"""
{'a', 'i', 'u', 'e', 'o'}
"""
i
"""
{'e', 'o'}
"""
word
"""
'hello'
"""
vowels
"""
{'a', 'i', 'u', 'e', 'o'}
"""
# 由上可以看出，集合的并集、差集、交集运算会创建一个集合副本。并不会改变参与运算的集合。
```
现在看看我们的代码如何实现：  
```py
vowels = {'a', 'e', 'i', 'o', 'u'}
word = input("Provide a word to search for vowels: ")
found = = vowels.intersection(set(word))
for vowel in found:
    print(vowel)
```
集合就学习完了，接下来学习元组。  

# 4. 元组
在第二章中我们知道了python中元组可以理解成不可变的列表。元组的创建如下：  
```py
vowels = ('a', 'e', 'i', 'o', 'u')  # 元组用小括号()，列表用中括号[]
```
我们可以使用内置函数type来判断对象的类型：  
```py
>>>vowels = ('a', 'e', 'i', 'o', 'u')
>>>type(vowels)
<class 'tuple'>
>>>vowels = ['a', 'e', 'i', 'o', 'u']
>>>type(vowels)
<class 'list'>
```
想要改变元组的元素，会发生错误：  
```py
vowels = ['a', 'e', 'i', 'o', 'u']
vowels[2]='d'
vowels
"""
['a', 'e', 'd', 'o', 'u']
"""
vowels = ('a', 'e', 'i', 'o', 'u')
vowels[2]='d'
# 输出
"""
Traceback (most recent call last):
  File "<input>", line 1, in <module>
TypeError: 'tuple' object does not support item assignment
"""
```
在使用元组时一定要注意只有一个对象的元组。  
```py
t = ('python')
type(t)
<class 'str'>
t2=('python',)
type(t2)
<class 'tuple'>
```
**即：当为单个对象的元组时，末尾的逗号不可少，这点要注意哈，记住就好了，就是python的语法规则**

# 5 组合内置的数据结构
python中一切都是对象，因此内置数据结构可以任意嵌套。下边我们看一个复杂的例子:   
先看一个复杂数据集合:  
```
Name             Gender         Occupation          Home Planet
----------------------------------------------------------------------
Ford Prefect     Male           Researcher          Betelgeuse Seven
Arthur Dent      Male           Sandwich-Maker      Earth
Tricia McMillan  Female         Mathematician       Earth
Marvin           Unknown        Paranoid Android    Unknown
```
接下里用**包含字典的字典**来存储以上数据
```py
>>> people = {}
>>> people['Ford'] = { 'Name': 'Ford Prefect','Gender': 'Male','Occupation': 'Researcher','Home Planet': 'Betelgeuse Seven' }
>>> people['Arthur'] = { 'Name': 'Arthur Dent','Gender': 'Male','Occupation': 'Sandwich-Maker','Home Planet': 'Earth' }
>>> people['Trillian'] = { 'Name': 'Tricia McMillan','Gender': 'Female','Occupation': 'Mathematician','Home Planet': 'Earth' }
>>> people['Robot'] = { 'Name': 'Marvin','Gender': 'Unknown','Occupation': 'Paranoid Android','Home Planet': 'Unknown' }
>>> people
'''
{'Ford': {'Name': 'Ford Prefect', 'Gender': 'Male', 'Occupation': 'Researcher', 'Home Planet': 'Betelgeuse Seven'}, 'Arthur': {'Name': 'Arthur Dent', 'Gender': 'Male', 'Occupation': 'Sandwich-Maker', 'Home Planet': 'Earth'}, 'Trillian': {'Name': 'Tricia McMillan', 'Gender': 'Female', 'Occupation': 'Mathematician', 'Home Planet': 'Earth'}, 'Robot': {'Name': 'Marvin', 'Gender': 'Unknown', 'Occupation': 'Paranoid Android', 'Home Planet': 'Unknown'}}
'''

# 可以更加优美的打印
>>> import pprint
>>> pprint.pprint(people)
#输出
"""
{'Arthur': {'Gender': 'Male',
            'Home Planet': 'Earth',
            'Name': 'Arthur Dent',
            'Occupation': 'Sandwich-Maker'},
 'Ford': {'Gender': 'Male',
          'Home Planet': 'Betelgeuse Seven',
          'Name': 'Ford Prefect',
          'Occupation': 'Researcher'},
 'Robot': {'Gender': 'Unknown',
           'Home Planet': 'Unknown',
           'Name': 'Marvin',
           'Occupation': 'Paranoid Android'},
 'Trillian': {'Gender': 'Female',
              'Home Planet': 'Earth',
              'Name': 'Tricia McMillan',
              'Occupation': 'Mathematician'}}

"""
```
访问嵌套数据：  
```py
>>> people['Arthur']
{'Name': 'Arthur Dent', 'Gender': 'Male', 'Occupation': 'Sandwich-Maker', 'Home Planet': 'Earth'}
>>> people['Arthur']['Occupation']
'Sandwich-Maker'
```
数据结构的嵌套是个非常强大的功能，python会帮我们考虑内存的分配等等问题。  

下一章我们开始学习python最基本的代码重用技术：函数。  

