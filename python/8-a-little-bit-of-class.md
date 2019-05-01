# 1. 概述
类允许把**代码行为和状态打包到一起**。本章学习python类，目的是学会使用python类来创建上下文管理器。学习之后来完善我们之前的web程序。  

python并不要求使用面向对象范式编程，挂接到with语句也不一定使用类（标准库就不是，不过标准库的方法并不广泛），因此推荐的方法是使用类来挂接到with语句。学会了类你就可以创建一个实现并遵循上下文管理协议的类。这个协议是挂接with语句的（python内置）机制。  
和其他语言类似，要使用一个类，需要先通过类来创建对象，这称为对象实例化，多个对象共享行为，但是不共享状态。

# 2. 定义类
python通过关键字class来创建类。一个空类如下：  
```py
class CountFromBy:  # 不要忘记冒号
    pass # 这是一个关键字，代表python的空语句，它什么也不做。在解释器希望找到具体代码的地方都可以使用pass
``` 
在这里我们必须使用pass，因为如果试图创建一个类，但是又没有任何语句，解释器会报错。  

创建python对象：  
```py
a = CountFromBy()
```
以上创建对象看起来像函数调用，那么我们怎么判断是创建对象还是函数调用呢？  
这是个好问题，从表面看，是看不出来的。不过，python社区有一个普遍认可的约定：函数用小写字母和下划线组成（例如之前的log_request）,类名类名采用CamelCase形式，你要遵循这个约定。  

我们希望CountFromBy做什么呢？我们希望这么使用它：  
我们要让它称为一个递增计数器。默认情况下，计数器从0开始，然后增1，我们还可以指定一个起始值和/或增量。例如：你可以创建一个从100开始每次递增10的CountFromBy对象。
```py
>>> c = CountFromBy()
>>> c
0 # 默认从0开始
>>> c.increase()
>>> c.increase()
>>> c.increase()
>>> c
3 # 增加三次，值为3

d = CountFromBy(100) # 初始值为100
>>> d
100
>>> d.increase()
>>> d.increase()
>>> d.increase()
>>> d
103

>>> e = CountFromBy(100, 10) # 指定初始值和增量
>>> e
100
>>> for i in range(3):
e.increase()
>>> e
130

>>> f = CountFromBy(increment=15) # 仅指定增量
>>> f
0
>>> for j in range(3):
f.increase()
>>> f
45
```
在进一步创建类之前，先了解下方法调用的细节。  
c.increase()，即：对象名，点好，方法名。解释器实际上在后台这么执行：CountFromBy.increase(c)。其实你在程序中也可以这么做，不过没人会这么写。  

因此，类的方法至少有一个参数，而且第一个参数用来接收这个类的对象。python社区约定俗成的为将第一个参数指定为一个特殊的名字：self。这其实类似于C++里边的this。  
如果你在代码里不是使用self而是使用其他名字，被认为是非常不好的形式。接下来我们学习如何为类增加方法。  
```py
class CountFromBy:
    def increase(self) -> None:
        self.val += self.incr
```
如上：self是非常重要的。self代表一个具体的对象以下代码就不会做你认为它做的事：  
```py
class CountFromBy:
    def increase(self) -> None:
        val += incr
```
表面上看以上代码没有问题，不过想想看这个increase结束时会发生什么：var和incr只存在于increase方法中，作用域仅限于increase方法，当方法结束时，这两个变量将被销毁。因此是不对的。  

即：变量在函数的代码组中定义时，它们只是在这个函数运行时才存在。因此：如果只是处理一个调用之后完成一些工作然后返回一个值的函数，以上描述的函数没有问题。因为你不关心函数中的变量，只关心其返回值。因此，为了与变量与对象关联，**属性名之前必须加self**。因此，如果需要引用类中的一个属性，必须在属性名前加上self，self中的值是一个别名，指示调用这个方法的对象。  

以上我们使用了对象的属性，那么怎么给属性赋一个初始值呢？以上代码是正确的，但是会失败，因为在python中，不论在哪里使用变量，都必须先用一个初始值初始化。那么，对于python类的新对象如何初始化呢？其他语言通常有一个构造函数，python其实也类似，有一个名为__init__的方法允许你根据需要初始化属性。下面看看他能做什么。  
还记得在第六章里我们dir(flask_req)的输出吗？  
```
['__class__', '__delattr__', '__dict__', '__dir__', '__doc__', '__enter__', '__eq__', '__exit__', '__format__', '__ge__', '__getattribute__', '__gt__', '__hash__', '__init__', '__init_subclass__', '__le__', '__lt__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_cached_json', '_get_data_for_json', '_get_file_stream', '_get_stream_for_parsing', '_load_form_data', '_parse_content_type', '_parsed_content_type', 'accept_charsets', 'accept_encodings', 'accept_languages', 'accept_mimetypes', 'access_route', 'application', 'args', 'authorization', 'base_url', 'blueprint', 'cache_control', 'charset', 'close', 'content_encoding', 'content_length', 'content_md5', 'content_type', 'cookies', 'data', 'date', 'dict_storage_class', 'disable_data_descriptor', 'encoding_errors', 'endpoint', 'environ', 'files', 'form', 'form_data_parser_class', 'from_values', 'full_path', 'get_data', 'get_json', 'headers', 'host', 'host_url', 'if_match', 'if_modified_since', 'if_none_match', 'if_range', 'if_unmodified_since', 'input_stream', 'is_json', 'is_multiprocess', 'is_multithread', 'is_run_once', 'is_secure', 'is_xhr', 'json', 'list_storage_class', 'make_form_data_parser', 'max_content_length', 'max_form_memory_size', 'max_forwards', 'method', 'mimetype', 'mimetype_params', 'on_json_loading_failed', 'parameter_storage_class', 'path', 'pragma', 'query_string', 'range', 'referrer', 'remote_addr', 'remote_user', 'routing_exception', 'scheme', 'script_root', 'shallow', 'stream', 'trusted_hosts', 'url', 'url_charset', 'url_root', 'url_rule', 'user_agent', 'values', 'view_args', 'want_form_data_parsed'] {'o', 'i'}
```
看到其中的很多dunder（双下划线）了吗？它们提供了每个类标准的行为。这些dunder方法由名为object的类实现。python的类都继承自object类，你可以覆盖这些方法。例如：  
 - 如果你想指定对象使用==时发生什么，就要覆盖__eq__方法；  
 - 如果你想指定对象使用>时发生什么，就要覆盖__ge__方法；  
 - 如果想要初始化与对象相关的属性，就要覆盖__init__方法；   
 - 如果想要定义对象如何表示，就要覆盖__repr__方法
**更多的信息可以看看python的文档。**   

用__init__初始化对象属性：  
```py
class CountFromBy:
    def __init__(self) -> None:  # 目前这个__init__啥也不做
        pass
    def increase(self) -> None:
        self.val += self.incr
```
可以向__init__传入任意数量的参数数据：  
```py
class CountFromBy:
    def __init__(self, v: int, i: int) -> None:  # 目前这个__init__啥也不做
        self.val = v
        self.incr = i
    def increase(self) -> None:
        self.val += self.incr
```
在python命令行试试：  
```py
h=CountFromBy(100, 10)
h.increase()
h
<__main__.CountFromBy object at 0x0000021DC6103E10>  # 直接显示对象h会报错
h.val
110
h.incr
10
g = CountFromBy()  # 不提供参数来构造也会报错
Traceback (most recent call last):
  File "<input>", line 1, in <module>
TypeError: __init__() missing 2 required positional arguments: 'v' and 'i'
```

为了定义CountFromBy对象如何展示（输入h不会报错），必须覆盖__repr__方法。  
我们定义如下：  
```py
def __repr__(self) -> str:
    return str(self.val)
```
同时，为了使g = CountFromBy()不报错，需要提供合适的默认值：  
```py
def __init__(self, v: int = 0, i: int = 1) -> None:  # 目前这个__init__啥也不做
    self.val = v
    self.incr = i
```
最终代码为：  
```py
class CountFromBy:
    def __init__(self, v: int = 0, i: int = 1) -> None:
        self.val = v
        self.incr = i

    def increase(self) -> None:
        self.val += self.incr

    def __repr__(self) -> str:
        return str(self.val)
```
小结一下关于类我们学习到的知识：  
 1. python类允许你共享行为(方法)和状态(属性)；  
 2. class用于定义一个新类；  
 3. pass表示空语句，什么也不做；  
 4. 类创建对象看上去非常像函数调用，因此必须遵循代码规范；  
 5. 多个类的对象之间共享方法，不共享属性；  
 6. 可以通过增加方法来增加类的行为；  
 7. 要为类增加属性，需要创建一个变量；  
 8. 方法调用时，解释器会传入对象的别名作为第一个参数，python约定坚持第一个参数名为self；  
 9. 在方法中，引用属性要加self前缀，确保方法结束后属性值任然存在；  
 10. __init__方法是所有python类提供的众多方法之一，用于对属性值初始化；  
 11. __repr__方法允许你控制在>>>提示符下显示时以及使用print打印时对象的表示。  

有了以上类的知识我们就可以创建上下文管理器，挂接with语句，来优雅的处理数据库了，下一章我们就这么做。  
