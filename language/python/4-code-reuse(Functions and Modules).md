# 1. 概述
本章学习Python中代码的重用：函数。  
在第一章我们已经知道：**函数是模块的一部分，模块是标准库的一部分**。为了重用代码，我们接下来对函数进行学习。  

# 2. 函数的定义和使用
为了使用函数，必须先定义函数。Python中定义一个函数需要如下几步：  
1. 引入了两个新关键字：def和return。def用于指定函数名。return关键字是可选的，用于向调用这个函数的代码返回一个值。  
2. 函数可以接受参数。   
3. 函数除包含代码外，通常还包含文档。代码在def行下缩进一层，会包含适当的注释。注释分为两种：单行注释和多行注释。单行注释以#开头，多行注释用""" 多行注释 """ 或者''' 多行注释 ''' 包围。  
如下是一个函数的模板：  
```py
def a_descriptive_name(optional_arguments):
    """A documentation string."""        # docstring 描述这个函数的用途
    # Your function's code goes here. 
    # Your function's code goes here. 
    # Your function's code goes here. 
    return optional_value
```
对于python函数，我们要知道:  
 - python解释器不要求指定函数参数和返回值的类型，可以将任何对象作为参数发送给函数，也可以将任何对象作为返回值传回。在python3中，可以指示返回值和参数类型。不过这只是为了方便函数调用方方便使用，python3不会做任何类型检查。  
 - 函数是用def命名的代码块，格式为：def后面是函数名，然后是一个可选的参数表，一个冒号，然后是一行或者多行代码。  
还记得上一章最后的程序吗：  
```py
vowels = set('aeiou')
word = input("Provide a word to search for vowels: ")
found = = vowels.intersection(set(word))
for vowel in found:
    print(vowel)
```
现在把它封装成一个函数并调用它：  
```py
def search4vowels():  #不要忘记冒号
    """Display any vowels found in a asked-for word."""
    vowels = set('aeiou')
    word = input("Provide a word to search for vowels: ")
    found = vowels.intersection(set(word))
    for vowel in found:
        print(vowel)

search4vowels()  # 调用
```
在python中，字符串可以用单引号，双引号，或者三重引号('''或者""")包围。  
三重引号的字符串称为docstring。主要作为文档来描述函数的用途。三重引号包围的字符串可以跨多行，单引号或者双引号包围的字符串不能跨多行。究竟使用哪种呢？这取决于你，不过大多数python程序员习惯用单引号。实际上最重要的是保持一致。  

## 2.1 遵循PEP的最佳实践：
**PEP是python社区建立起来的python最佳实践的信息。PEP文档的技术性强，通常不易理解。不过PEP 8除外，PEP 8是python代码风格的指南。另外还有PEP 257描述了格式化docstring的有关约定，也很必要读一读。**  
PEP地址： https://www.python.org/dev/peps/  
下边给出符合PEP 8和PEP 257的search4vowels函数：  
```py
def search4vowels():  #不要忘记冒号
    """Display any vowels found in a asked-for word."""
    vowels = set('aeiou')
    word = input('Provide a word to search for vowels: ')
    found = vowels.intersection(set(word))
    for vowel in found:
        print(vowel)
```
其实以上代码并不完全遵循PEP8，PEP8中函数名中的单词之前用下划线隔开。例如：search_for_vowels才是符合PEP 8的代码。不过字符串使用单引号是PEP 8的标准。docstring使用"""是PEP 257的规定。  

## 2.2 函数参数和返回值
函数参数：  
```py
def search4vowels(word):  #不要忘记冒号
    """Display any vowels found in a asked-for word."""
    vowels = set('aeiou')
    found = vowels.intersection(set(word))
    for vowel in found:
        print(vowel)
search4vowels('dfhiude')
```
函数返回值：  
search4vowels返回一个bool值，表示输入字符串中是否有元音字母。python中，每一个对象都有一个关联的真值，表示这个对象计算为True或者False。如果计算为0、值None、空串或一个空的内置数据结构则为False，其他对象都计算为True。通过函数bool来计算结果。例如：  
```py
>>> bool(0)
False
>>> bool(0.0)
False
>>> bool('')
False
>>> bool([])
False
>>> bool({})
False
>>> bool(None)
False
# 以下为true
>>> bool(1)
True
>>> bool(-1)
True
>>> bool(42)
True
>>> bool(0.0000000000000000000000000000001)
True
>>> bool('Panic')
True
>>> bool([42, 43, 44])
True
>>> bool({'a': 42, 'b':42})
True
```
有了如上基础接下来看看search4vowels函数:  
```py
def search4vowels(word):  #不要忘记冒号
    """Display any vowels found in a asked-for word."""
    vowels = set('aeiou')
    found = vowels.intersection(set(word))
    return bool(found)
```
函数被设计成返回一个值。如果需要返回多个值，唯一的办法就是把多个值打包到一个数据结构中，然后返回这个数据结构。
**返回一个集合**
```py
def search4vowels(word):  #不要忘记冒号
    """Display any vowels found in a asked-for word."""
    vowels = set('aeiou')
    found = vowels.intersection(set(word))
    return found
print(search4vowels('fgnn'))
#输出：set()
```
 - set()代表空集合，而不是{};  
 - {}、dict()代表空字典;  
 - list()、[]代表空列表;  
 - tuple()、()代表空元组。  

## 2.3 使用注解改进文档
函数注解的目的是帮助使用者更加清晰的使用函数。因为docstring的风格不一样。函数不易使用。为此**python3引入了函数注解。**注解描述了参数的类型和返回值类型，不过要注意:  
 - 注解是可选的：很多python代码都没有注解，因为python3才开始支持注解。   
 - 注解可以提供信息，也仅可以提供信息。注解不会有任何其他行为(比如类型检查)。  

```py
# 为参数和返回值添加注解：
def search4vowels(word: str) -> set:
    """Return any vowels found in a supplied word."""
    vowels = set('aeiou')
    return vowels.intersection(set(word))
# 为了可读性，我们把word改为phrase
def search4vowels(phrase: str) -> set:
    """Return any vowels found in a supplied phrase."""
    vowels = set('aeiou')
    return vowels.intersection(set(phrase))
```
**注解的目的仅仅是一个标准的文档。并不会强制类型检查。可以通过内建函数help来输出函数的注解和docstring**。例如，对于以上函数，help(search4vowels)将输出：  
```py
Help on function search4vowels in module __main__:

search4vowels(word: str) -> set
    Return any vowels found in a supplied word.
```
关于注解的更多信息见PPEP 3107: https://www.python.org/dev/peps/pep-3107  

接下来我们改进一下search4vowels，使其变成一个更加通用的函数: 函数接收两个参数，第二个参数指定要搜素什么（而不仅限于元音字母）。我们需要如下几步：  
1. 取一个更加通用的函数名。我们指定为search4letters  
2. 增加第二个参数和对应的注解。  
3. 删除vowels变量。  
4. 更新docstring。  

这时我们得到两个函数。
```py
def search4vowels(phrase: str) -> set:
    """Return any vowels found in a supplied phrase."""
    vowels = set('aeiou')
    return vowels.intersection(set(phrase))

def search4letters(phrase: str, letters: str) -> set:
    """Return a set of the 'letters' found in 'phrase'."""
    return set(letters).intersection(set(phrase))
```

可以为函数参数指定默认值：  
```py
def search4letters(phrase: str, letters: str='aeiou') -> set:
```
调用函数时，有两种赋值方式：位置赋值和关键字赋值。  
1. 位置赋值  
search4letters('galaxy', 'xyz')  
phrase == 'galaxy', letters == 'xyz'  
2. 关键字赋值  
search4letters(letters = 'xyz', phrase = 'galaxy')   

两种效果一样，不过对于参数很多的函数，关键字赋值其实是非常好的。  
函数的学习快要告一段落了，不过还有个小问题：函数参数的语义--按值传递还是按引用传递？  

## 2.4 函数参数的语义
先看两个例子:  
```py
def double(arg):
    print('Before: ', arg)
    arg = arg * 2
    print('After:  ', arg)

def change(arg):
    print('Before: ', arg)
    arg.append('More data')
    print('After:  ', arg)
```
**按值传递：**  
```py
>>> num = 10
>>> double(num)
Before:  10
After:   20
>>> num
10
>>> saying = 'hello '
>>> double(saying)
Before:  hello 
After:   hello hello 
>>> saying
'hello '
>>> numbers = [1, 2, 3]
>>> double(numbers)
Before:  [1, 2, 3]
After:   [1, 2, 3, 1, 2, 3]
>>> numbers
[1, 2, 3]
```

**按引用传递：**  
```py  
>>> num2 = 10
>>> change(10)
Before:  10
Traceback (most recent call last):
  File "<input>", line 1, in <module>
  File "<input>", line 3, in change
AttributeError: 'int' object has no attribute 'append'
# 以上由于整数没有append函数因此出错，必须传递具有append函数的类对象。  
>>> nums2 = [4, 5, 6]
>>> change(nums2)
Before:  [4, 5, 6]
After:   [4, 5, 6, 'More data']
>>> nums2
[4, 5, 6, 'More data']
```
**python中，根据具体情况，函数参数的语义既支持按值调用也支持按引用调用。**  
在python中，我们看到的变量是对象引用。可以把变量中存的值认为是值的内存地址。会把这个地址传入函数参数。python中更加准确的描述是：**按对象引用调用语义**。依据对象指示的类型，不同情况下语义不同。解释器会查看对象引用指示的那个值的类型，如果指示一个可变值，就会按引用语义调用，如果指示不可变值，则按值调用。  

字符串、整数、元组是不可变的，因此为按值调用。  
**但是对于double函数，为什么传递列表，也没有改变列表的值呢?矛盾了？？？**要理解这里发生了什么，需要理解：  
```py
arg = arg * 2
```
是一个赋值语句。赋值时会发生如下动作：  
 - =右边的代码先执行  
 - 将创建的值的对象引用赋值给左边的变量。  
因此，arg * 2 会产生一个新的对象引用。然后赋值给arg变量。**其实是又创建了一个变量arg，这个与调用方的arg不同**。而
```py
arg.append('more data')
```
则不会创建新变量。就会改变原来arg引用的值。  

可以使用id函数来打印对象的地址信息：  
```py
def double(arg):
    print('Before: ', arg)
    print(id(arg))
    arg = arg * 2
    print('After:  ', arg)
    print(id(arg))

def change(arg):
    print('Before: ', arg)
    print(id(arg))
    arg.append('More data')
    print('After:  ', arg)
    print(id(arg))
```
输出:  
```py
>>> num=[11,2,3]
>>> double(num)
Before:  [11, 2, 3]
2147600249544
After:   [11, 2, 3, 11, 2, 3]
2147600229832
>>> num
[11, 2, 3]
    
>>> change(num)
Before:  [11, 2, 3]
2147600249544
After:   [11, 2, 3, 'More data']
2147600249544
>>> num
[11, 2, 3, 'More data']
>>> double(num)
Before:  [11, 2, 3, 'More data']
2147600249544
After:   [11, 2, 3, 'More data', 11, 2, 3, 'More data']
2147599871176
>>> num
[11, 2, 3, 'More data']
```

**这块具体怎么传递，可能与C/C++，Java这些不同，实在不确定的，使用之前可以在python shell下试验一下，以免出错。**  

# 3. 模块知识
如前，假设我们得到的两个函数保存在了vsearch.py中，这时就得到了一个模块，在python中，**模块就是包含函数的文件**

创建模块很容易：只需要为你想要共享的函数创建一个文件。然后import这个模块即可。不过，解释器会做一个假设：认为当前模块在**搜索路径**中，接下来研究这一点。  
当我们使用import时，python会在3个位置搜索模块：  
1. 你的当前工作目录。  
2. 你的解释器的site-packages位置：这些目录包含你可能已经安装的第三方python模块(包含你自己写的)  
3. 标准库位置

我们先创建一个文件夹，mymodules，将vsearch.py拷贝至该文件夹，然后，在命令行切换到mymodules之后启动python：  
```py
C:\Users\13328>cd f:
F:\
C:\Users\13328>f:
F:\>cd ./mygithub/xufubo-blog/python/python_study_code/mymodules
F:\mygithub\xufubo-blog\python\python_study_code\mymodules>py -3
Python 3.7.2 (tags/v3.7.2:9a3ffc0492, Dec 23 2018, 23:09:28) [MSC v.1916 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
>>>
>>> import vsearch
>>> vsearch.search4vowels('fdsferfrgi')
{'i', 'e'}
>>> vsearch.search4letters('fdsferfrgi', 'fgrtuk')
{'r', 'f', 'g'}
>>> exit()
F:\mygithub\xufubo-blog\python\python_study_code\mymodules>
F:\mygithub\xufubo-blog\python\python_study_code\mymodules>cd ../
F:\mygithub\xufubo-blog\python\python_study_code>py -3
Python 3.7.2 (tags/v3.7.2:9a3ffc0492, Dec 23 2018, 23:09:28) [MSC v.1916 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
>>> import vsearch
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ModuleNotFoundError: No module named 'vsearch'
>>>
```
前面之所以import模块vsearch是因为该模块位于当前工作目录(切换到mymodules后启动的python3)，后来由于工作目录不在mymodules，因此会出现ImportError。这个与操作系统平台无关。为此，必须将模块安装到site-packages位置。python解释器不支持绝对路径的导入，即：  
```py
import F:\mygithub\xufubo-blog\python\python_study_code\mymodules\vsearch
```
是不行的。因此需要将自己的模块安装在site-packages位置。  

## 3.1 模块安装到site-packages
接下来学习使用setuptools(python3.4即以上版本的标准库包含的模块)将模块安装到site-packages。  
将模块安装到site-packages需要三步：  
1. 创建一个发布描述：明确setuptools要安装的模块   
2. 生成一个发布文件：使用python创建一个文件  
3. 安装发布文件：使用python将发布文件安装到site-packages   

### 3.1.1. 创建描述文件   
至少创建：setup.py和README.txt两个文件，这两个文件应该与vsearch.py文件放在同一个文件夹中，不论在哪个系统平台上都要这么做。第一个文件必须为setup.py，用来详细描述要安装的模块：  
```py
# setup.py的内容如下
from setuptools import setup
setup(
    name='vsearch',    # name参数指定发布包名
    version='1.0',
    description='The Head First Python Search Tools',
    author='HF Python 2e',
    author_email='hfpy2e@gmail.com',
    url='headfirstlabs.com',
    py_modules=['vsearch'],     # 模块名字
)
```
该文件包含两条语句：导入setup函数，调用setup函数。  
除了setup.py，setuptools还需要一个readme文件，这个文件中可以放入包的文件描述。尽管必须有这个文件，但是它的内容是可选的。现在我们在setup.py，vsearch.py的目录下创建一个空文件：README.txt。  

接下来就创建发布文件。  

### 3.1.2. 创建发布文件  
到了这里，我们在mymodules下包含三个文件：setup.py、vsearch.py、README.txt。接下来创建发布文件：  
```
F:\mygithub\xufubo-blog\python\python_study_code\mymodules>py -3 setup.py sdist
running sdist
........
creating dist
Creating tar archive
removing 'vsearch-1.0' (and everything under it)
```
这时，在**mymodules/dist**目录下有个vsearch-1.0.tar.gz文件，就是我们需要的发布文件。  
Linux/unix下为：python3 setup.py sdist  

### 3.1.3. 安装发布文件  
```
F:\mygithub\xufubo-blog\python\python_study_code\mymodules>cd dist
F:\mygithub\xufubo-blog\python\python_study_code\mymodules\dist>py -3 -m pip install vsearch-1.0.tar.gz
Processing f:\mygithub\xufubo-blog\python\python_study_code\mymodules\dist\vsearch-1.0.tar.gz
Installing collected packages: vsearch
  Running setup.py install for vsearch ... done
Successfully installed vsearch-1.0

# linux/unix下为： sudo python3 -m pip install vsearch-1.0.tar.gz
```
这时就将自己的vsearch模块安装到了site-packages中了。如下，可以在别的工作目录使用了：  
```py
F:\mygithub\xufubo-blog\python\python_study_code>py -3   # 之前在这个目录下会出错
Python 3.7.2 (tags/v3.7.2:9a3ffc0492, Dec 23 2018, 23:09:28) [MSC v.1916 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
>>>
>>> import vsearch
>>> vsearch.search4vowels('dsgfhji')
{'i'}
>>>
```
### 3.1.4. 共享你的代码
创建好发布文件，你可以共享你的代码，可以通过email、U盘等传播给他人，也可以**正式的共享你的代码**。这时需要将发布文件上传到python集中管理的软件存储库PyPI(pie-pee-eye)，详细信息可以谷歌，或者见:https://pypi.python.org/pypi 和https://www.pypa.io。  


# 4. 代码风格
良好一致的代码风格有助于分享代码，前面说过，最好遵循PEP 8，那么可以检查我的代码是否符合PEP 8吗？答案是可以的。不过需要安装一个工具：**pytest**，pytest是一个测试框架，你可以为pytest增加插件来扩展它的功能。pep8就是它的一个插件。pytest的更多信息，见:https://doc.pytest.org/en/latest/ 。 
接下里安装测试开发工具检查我们的代码。  
```
py -3 -m pip install pytest
py -3 -m pip install pytest-pep8
```
pip(package Install for Python):python的包安装工具。  

然后检查代码是否符合PEP8：  
```
F:\mygithub\xufubo-blog\python\python_study_code\ch04>py.test --pep8 vsearch.py
d:\program files\python37\lib\site-packages\pep8.py:110: FutureWarning: Possible nested set at position 1
  EXTRANEOUS_WHITESPACE_REGEX = re.compile(r'[[({] | []}),;:]')
==================================================== test session starts =================================================
platform win32 -- Python 3.7.2, pytest-4.4.0, py-1.8.0, pluggy-0.9.0
rootdir: F:\mygithub\xufubo-blog\python\python_study_code\ch04
plugins: pep8-1.0.6
collected 1 item

vsearch.py F                                                                                                        [100%]

===================================================== FAILURES ===========================================================
_____________________________________________________ PEP8-check _________________________________________________________
F:\mygithub\xufubo-blog\python\python_study_code\ch04\vsearch.py:14:1: W391 blank line at end of file
help(search4letters)
^

===================================================== 1 failed in 0.06 seconds ===========================================
F:\mygithub\xufubo-blog\python\python_study_code\ch04>
```
提示我们在第14行的问题：文件应该以空行结尾，我们改一下，在文件末尾加入空行即可通过：  
```
F:\mygithub\xufubo-blog\python\python_study_code\ch04>py.test --pep8 vsearch.py
d:\program files\python37\lib\site-packages\pep8.py:110: FutureWarning: Possible nested set at position 1
  EXTRANEOUS_WHITESPACE_REGEX = re.compile(r'[[({] | []}),;:]')
===================================================== test session starts ================================================
platform win32 -- Python 3.7.2, pytest-4.4.0, py-1.8.0, pluggy-0.9.0
rootdir: F:\mygithub\xufubo-blog\python\python_study_code\ch04
plugins: pep8-1.0.6
collected 1 item

vsearch.py .                                                                                                       [100%]

===================================================== 1 passed in 0.06 seconds ===========================================

F:\mygithub\xufubo-blog\python\python_study_code\ch04>
```
感兴趣的可以看看:https://pep8.org/  是一个python风格指南的很美观的版本。  

# 5. 小结一下
到此，我们已经学习了四章python知识了，主要有：  
1. 学习了IDLE这个python内置的IDE，通过python3或者py -3来启动python解释器。  
2. 已经了解了python对单值数据项的支持：整数、字符串、bool。  
3. 学习了四个内置数据结构:列表、元组、字典、集合。  
4. 使用了很多python语句:if elif else for return def import。  
5. 了解了python丰富的标准库：datetime、random、pprint、sys、os、time、html、setuptools、pip。  
6. 了解了内置函数: print dir help list len range input sorted dict set tuple type。  
7. python的常用操作符: in, not in, +, -, =, ==, +=和*。  
8. 除了支持[]来处理列表数据外，python扩展了它的用法来支持切片，允许你指定start、end、step。  
9. 学习了def来定义函数并返回值。  
10. 学习了字符串的表示，docstring、注解、PEP8、PEP 257等。  
11. 学习了模块知识：编写模块、安装模块、使用模块、共享自己的模块。  
12. 你已经认识到python中一切都是对象。开始用类定义你自己的定制对象时(后边学些)，这个概念非常重要。  

接下来学习第五章。  
