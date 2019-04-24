# 1. 概述
上一章我们利用Flask框架编写了一个web服务器，不过我们的web服务器只是处理请求返回数据，有的时候我们还需要记录一些日志信息来统计某些数据，比如：  
 - 已经响应了多少个请求？  
 - 最常用的字母列表是什么？  
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

