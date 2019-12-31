# 1. 概述
上一章我们利用Flask框架编写了一个web服务器，不过我们的web服务器只是处理请求返回数据，有的时候我们还需要记录一些日志信息来统计某些数据，比如：  
 - 已经响应了多少个请求？  
 - 最常用的字母列表是什么？  
 - 请求来自哪个IP地址？  
 - 哪个浏览器用的最多？  

为了回答这些问题，我们需要记录web应用处理的请求信息。需要将这些数据记录到日志文件。接下来学习python对文件的处理。  

# 2. python文件处理
python处理文件包含三步:  
- 打开；  
- 处理；  
- 关闭。  
例如：  
```py
todos = open('todos.txt', 'a')   # a表示追加模式
print('put out the line1.', file=todos) # 将内容写入文件todos
print('put out the line2.', file=todos) # 将内容写入文件todos
print('put out the line3.', file=todos) # 将内容写入文件todos
todos.close()  # 处理完文件一定记得close，否则可能丢数据
```
**从现有文件读数据：**  
```py
tasks = open('todos.txt') # 读是open的默认模式，因此这里不再指定打开模式
# 一行行的处理数据
for line in tasks:
    print(line)
tasks.close()
"""
以上的输出如下：  
put out the line1.

put out the line2.

put out the line3.

你会看到多了一个换行符。
"""
```
这是因为print默认会追加一个换行符，文件中的内容有换行符，print又追加了一个换行符，因此中间有一个空行。为了没有多余的空行，需要将  
```py
print(line)  改为  print(line, end = '') 这就避免了print追加换行符。  
```  
python打开文件的模式有：  
 - r：打开文件来读数据，默认打开模式。该模式假设文件已经存在。如果文件不存在，该模式不会创建新文件，其他模式会创建新的空文件。  
 - w：打开文件写数据，如果文件中已有数据，写之前将数据清空。  
 - a：追加模式打开文件，向文件尾写数据，不会清空文件。  
 - x：打开一个新文件来写数据。如果文件存在则失败。  

默认以文本模式打开。这里假设文件包含文本数据行。如果需要以二进制方式打开处理数据，需要为打开模式增加b(例如：'wb'表示写二进制数据)。如果第二个参数中还包含+，则会打开文件来完成读写。例如：'x+b'表示读写一个新的二进制文件。更多信息可以参考python文档。那里是最权威的。  

每次打开文件都得关闭，万一忘记了怎么办？
不要慌，我们接下来看看更好的处理方式：with语句。  
```py
tasks = open('todos.txt') # 读是open的默认模式，因此这里不再指定打开模式
for line in tasks:
    print(line, end = '')
tasks.close()
```
与  
```py
with open('todos.txt') as tasks:
    for line in tasks:
        print(line, end = '')
```
with语句足够聪明，会帮你调用close。  
with语句管理上下文：with语句符合python中内置的一个编码约定，这称为上下文管理协议。后边会详细介绍，现在只需要知道它会管理其代码组运行的上下文，对于open来说，它会帮你调用close。  

接下来来完善我们的web应用。上一章最后的web程序为：  
```py
from flask import Flask, render_template, request  # 不再需要导入redirect
from vsearch import search4letters

app = Flask(__name__)

@app.route('/search4', methods=['POST'])
def do_search() -> 'html':
    phrase = request.form['phrase']
    letters = request.form['letters']
    title = 'Here are your results:'
    results = str(search4letters(phrase, letters))
    return render_template('results.html',
                           the_title=title,
                           the_phrase=phrase,
                           the_letters=letters,
                           the_results=results,) # 这个逗号对于python是合法的，看起来有点怪，不过也可以没有

@app.route('/')
@app.route('/entry')
def entry_page() -> 'html':
    return render_template('entry.html',  # 模板
                           the_title='Welcome to search4letters on the web!')  # 指定entry.html模板中的the_title参数。
if __name__ == '__main__': 
    app.run(debug=True)
```
现在我们需要写一个名为：log_request函数，该函数没有返回值，它有两个参数：req，res。req代表当前的Flask请求对象，res为search4letters的结果。该函数将req和res的值输出到文件vsearch.log中，用一行表示,代码如下：  
```py
from flask import Flask, render_template, request  # 不再需要导入redirect
from vsearch import search4letters

app = Flask(__name__)

# req的注解可能会令你困惑，这里你要知道：注解的目的是增加代码的可读性，
# 解释器并不关心，它们只是文档，你可以使用你喜欢的任意注解描述符
def log_request(req: 'flask_request', res: str) -> None:
    with open('vsearch.log', 'a') as log:
        print(req, res, file = log)

@app.route('/search4', methods=['POST'])
def do_search() -> 'html':
    phrase = request.form['phrase']
    letters = request.form['letters']
    title = 'Here are your results:'
    results = str(search4letters(phrase, letters))
    log_request(request, results)
    return render_template('results.html',
                           the_title=title,
                           the_phrase=phrase,
                           the_letters=letters,
                           the_results=results,) # 这个逗号对于python是合法的，看起来有点怪，不过也可以没有

@app.route('/')
@app.route('/entry')
def entry_page() -> 'html':
    return render_template('entry.html',  # 模板
                           the_title='Welcome to search4letters on the web!')  # 指定entry.html模板中的the_title参数。
if __name__ == '__main__': 
    app.run(debug=True)
```
运行，然后通过浏览器访问，提交表单数据，我搜索了两次，在服务器的vsearch.log中看到如下：  
```
<Request 'http://127.0.0.1:5000/search4' [POST]> set()
<Request 'http://127.0.0.1:5000/search4' [POST]> {'i', 'u'}
```
# 3. 远程日志
我们当然可以登录到服务器上查看我们的日志，如果我想看远程日志呢？  
我们的web应用需要支持/viewlog来返回日志文件内容：  
```py
#在上边的程序加入如下代码
@app.route('/viewlog')
def view_the_log() -> str:
    with open('vsearch.log') as log:
        contents = log.read()   #这里我们不是一行行的读，而是通过read一次性读取整个文件内容
    return contents
```
对应上边的两行日志，输入 http://127.0.0.1:5000/viewlog 在浏览器看到的结果是：  
```
set() {'i', 'u'}
```
在谷歌浏览器选择“查看网页源代码”才能看到：  
```
<Request 'http://127.0.0.1:5000/search4' [POST]> set()
<Request 'http://127.0.0.1:5000/search4' [POST]> {'i', 'u'}
```
这是因为浏览器会把<>中的内容作为html解析，由于Request不是一个合法的html标记，浏览器将忽略它，不会展示它的任何内容。为了展示这些内容，我们需要做些转意，好在Flask提供了一个有用的函数。  

为了使web浏览器展示<>之间的文本，而不将他们作为html标记，需要使用escape函数转意，该函数Markup对象。你就理解成转意后的字符串即可。现在看看我们的代码：  
```py
from flask import Flask, render_template, request, escape
@app.route('/viewlog')
def view_the_log() -> str:
    with open('vsearch.log') as log:
        contents = log.read()   #这里我们不是一行行的读，而是通过read一次性读取整个文件内容
    return escape(contents)
```
这时看到了这个：  
```
<Request 'http://127.0.0.1:5000/search4' [POST]> set() <Request 'http://127.0.0.1:5000/search4' [POST]> {'i', 'u'}
```
看到了完成日志，不过把我们的换行符不见了。  
最原始的数据为（右击鼠标-->查看网页源代码）：  
```
&lt;Request &#39;http://127.0.0.1:5000/search4&#39; [POST]&gt; set()
&lt;Request &#39;http://127.0.0.1:5000/search4&#39; [POST]&gt; {&#39;i&#39;, &#39;u&#39;}
```
可以看到被escape转移后的字符串。  

不过我们从这个输出看不出请求中是什么，为此，我们需要进一步了解Flask的请求。   

日志中的请求对我们帮助不大，因为每个请求都一样：  
```
<Request 'http://127.0.0.1:5000/search4' [POST]> set()
<Request 'http://127.0.0.1:5000/search4' [POST]> {'i', 'u'}
``` 
因为这里只是在对象层次记录这些请求，我们实际上需要看看请求里的内容，之前我们学习了，可以通过dir来看看对象的属性和方法，现在我们就了解下flask_request的属性和方法。我们稍微调整下函数do_request:  
```py
def log_request(req: 'flask_request', res: str) -> None:
    with open('vsearch.log', 'a') as log:
        print(str(dir(req)), res, file = log) # 列出req的属性和方法列表并转换为字符串
```
请求之后，现在看看输出：  
```
['__class__', '__delattr__', '__dict__', '__dir__', '__doc__', '__enter__', '__eq__', '__exit__', '__format__', '__ge__', '__getattribute__', '__gt__', '__hash__', '__init__', '__init_subclass__', '__le__', '__lt__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_cached_json', '_get_data_for_json', '_get_file_stream', '_get_stream_for_parsing', '_load_form_data', '_parse_content_type', '_parsed_content_type', 'accept_charsets', 'accept_encodings', 'accept_languages', 'accept_mimetypes', 'access_route', 'application', 'args', 'authorization', 'base_url', 'blueprint', 'cache_control', 'charset', 'close', 'content_encoding', 'content_length', 'content_md5', 'content_type', 'cookies', 'data', 'date', 'dict_storage_class', 'disable_data_descriptor', 'encoding_errors', 'endpoint', 'environ', 'files', 'form', 'form_data_parser_class', 'from_values', 'full_path', 'get_data', 'get_json', 'headers', 'host', 'host_url', 'if_match', 'if_modified_since', 'if_none_match', 'if_range', 'if_unmodified_since', 'input_stream', 'is_json', 'is_multiprocess', 'is_multithread', 'is_run_once', 'is_secure', 'is_xhr', 'json', 'list_storage_class', 'make_form_data_parser', 'max_content_length', 'max_form_memory_size', 'max_forwards', 'method', 'mimetype', 'mimetype_params', 'on_json_loading_failed', 'parameter_storage_class', 'path', 'pragma', 'query_string', 'range', 'referrer', 'remote_addr', 'remote_user', 'routing_exception', 'scheme', 'script_root', 'shallow', 'stream', 'trusted_hosts', 'url', 'url_charset', 'url_root', 'url_rule', 'user_agent', 'values', 'view_args', 'want_form_data_parsed'] {'o', 'i'}
```
里边很多属性，对于我们来说，form、remote_addr、user_agent是我们想要的：  
 - form：表单提交的数据  
 - remote_addr：web浏览器的IP地址  
 - user_agent：提交数据的浏览器标识  

下边看看修改后的函数（一个请求记录在单行）：  
```py
def log_request(req: 'flask_request', res: str) -> None:
    with open('vsearch.log', 'a') as log:
        print(req.form, file = log, end='|')
        print(req.remote_addr, file = log, end='|')
        print(req.user_agent, file = log, end='|')
        print(res, file = log)  # 默认追加一个换行符
```
也可以:   
```py
def log_request(req: 'flask_request', res: str) -> None:
    with open('vsearch.log', 'a') as log:
        print(req.form, req.remote_addr, req.user_agent, res, file = log, sep='|')
```
这时候会看到这样一行数据：  
```
ImmutableMultiDict([('phrase', 'rgtfhyjio'), ('letters', 'aeiou')])|127.0.0.1|Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.139 Safari/537.36|{'o', 'i'}
```
功能是完成了，不过不太好看，如何把我们的数据展示的好看点？  
为了把各个部分读出来，我们需要将每一行数据的各个部分提取出来，存储到一个列表里，然后进行处理：  
```py
@app.route('/viewlog')
def view_the_log() -> str:
    contents = []  # contents为一个空列表
    with open('vsearch.log') as log:
        for line in log:
            contents.append([])  # contents为一个二级列表
            for item in line.split('|'):  # 将每一行数据转换为一个列表，item为列表的元素
                contents[-1].append(escape(item))  # 将列表元素追加到最后一个子列表中 ,escape是避免浏览器过滤某些字符，例如：空格、回车、换行等
    return str(contents)
```
这时，如果有两次请求，在vsearch.log中为两行，会在浏览器上看到如下日志：  
```
[[Markup('ImmutableMultiDict([('phrase', 'rgtfhyjio'), ('letters', 'aeiou')])'), Markup('127.0.0.1'), Markup('Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.139 Safari/537.36'), Markup('{'o', 'i'}\n')], [Markup('ImmutableMultiDict([('phrase', 'rgtfhyjio'), ('letters', 'aeiou')])'), Markup('127.0.0.1'), Markup('Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.139 Safari/537.36'), Markup('{'i', 'o'}\n')]]
```
看上去很相似，其实不一样了，这个已经是一个嵌套的二级列表了。如果在配合jinja2，可以得到不错的展示。  
接下里我们就是要使用html生成可读的输出，为此，先定义html的模板（viewlog.html）：  
```html
{% extends 'base.html' %}

{% block body %}

<h2>{{ the_title }}</h2>  <!-- the_title 需要我们赋值 -->

<table>
    <tr>
        {% for row_title in the_row_titles %}  <!-- the_row_titles 需要我们赋值 -->
            <th>{{row_title}}</th>
        {% endfor %}
    </tr>
    {% for log_row in the_data %}  <!-- the_data 需要我们赋值 -->
        <tr>
            {% for item in log_row %}
                <td>{{item}}</td>
            {% endfor %}
        </tr>
    {% endfor %}
</table>

{% endblock %}
```
并将viewlog.html放在templates下，结构为：  
```
                |-------vsearch4web.py   # web应用代码
webapp folder---|
                |
                |-------static folder--------hf.css   # 样式表在单独的文件夹中
                |
                |-------templates folder----|
                                            |--------base.html
                                            |--------entry.html
                                            |--------results.html
                                            |--------viewlog.html
```
函数view_the_log如下：  
```py
@app.route('/viewlog')
def view_the_log() -> 'html': # 修改下注解
    contents = []  # contents为一个空列表
    with open('vsearch.log') as log:
        for line in log:
            contents.append([])  # contents为一个二级列表
            for item in line.split('|'):  # 将每一行数据转换为一个列表，item为列表的元素
                contents[-1].append(escape(item))  # 将列表元素追加到最后一个子列表中 ,escape是避免浏览器过滤某些字符，例如：空格、回车、换行等
    titles = ('form data', 'remote_addr', 'user_agent', 'results')
    return render_template('viewlog.html',
                            the_title='View Log',
                            the_row_titles=titles,
                            the_data=contents,)
```
由此得到了版本如下：  
```py
from flask import Flask, render_template, request, escape  # 不再需要导入redirect
from vsearch import search4letters

app = Flask(__name__)

# req的注解可能会令你困惑，这里你要知道：注解的目的是增加代码的可读性，
# 解释器并不关心，它们只是文档，你可以使用你喜欢的任意注解描述符
def log_request(req: 'flask_request', res: str) -> None:
    with open('vsearch.log', 'a') as log:
        print(req.form, req.remote_addr, req.user_agent, res, file = log, sep='|')

@app.route('/search4', methods=['POST'])
def do_search() -> 'html':
    phrase = request.form['phrase']
    letters = request.form['letters']
    title = 'Here are your results:'
    results = str(search4letters(phrase, letters))
    log_request(request, results)
    return render_template('results.html',
                           the_title=title,
                           the_phrase=phrase,
                           the_letters=letters,
                           the_results=results,) # 这个逗号对于python是合法的，看起来有点怪，不过也可以没有

@app.route('/')
@app.route('/entry')
def entry_page() -> 'html':
    return render_template('entry.html',  # 模板
                           the_title='Welcome to search4letters on the web!')  # 指定entry.html模板中的the_title参数。
@app.route('/viewlog')
def view_the_log() -> 'html':
    contents = []  # contents为一个空列表
    with open('vsearch.log') as log:
        for line in log:
            contents.append([])  # contents为一个二级列表
            for item in line.split('|'):  # 将每一行数据转换为一个列表，item为列表的元素
                contents[-1].append(escape(item))  # 将列表元素追加到最后一个子列表中 ,escape是避免浏览器过滤某些字符，例如：空格、回车、换行等
    titles = ('form data', 'remote_addr', 'user_agent', 'results')
    return render_template('viewlog.html',
                            the_title='View Log',
                            the_row_titles=titles,
                            the_data=contents,)

if __name__ == '__main__':
    app.run(debug=True)
```
这时候测试，你会发现日志被显示在一个表格里，还不错，但是这还是不足以回答本章开头的几个问题：  
 - 已经响应了多少个请求？  
 - 最常用的字母列表是什么？  
 - 请求来自哪个IP地址？  
 - 哪个浏览器用的最多？  
 这些问题不好回答，你还需要对数据做进一步的处理分析和计算。我们可以用python来处理这些数据，输出我们想要的结果。实际上，我们可以借助数据库技术，将日志保存在数据库中，就可以利用数据库查询技术的强大功能来回答几乎所有问题。  

这一章学习了文件操作，下一章就学习数据库技术来解答我们的问题。

