# 1. 概述
本章我们要构建一个web应用，具体的说，要完成以下工作：  
1. 利用第四章我们的search4letters函数；  
2. 让人们在web上访问这个函数；  
3. 人们需要输入phrase和letters，我们返回search4letters的结果。  

为了实现以上功能，除了前面学习的知识外，还需要了解**web应用框架**，它提供了一组通用的基础技术，可以基于这些技术构建自己的应用。你也可以使用python的标准库从头构建web应用，不过这费时费力。已经有很多优秀的web框架了。比如:Flask、Django。Django有强大的管理功能，是web社区非常流行的web框架。我们这里只需要简单的功能，因此选用流行的轻量级框架Flask。使用Django有点大材小用了。  

上一章提到，python社区维护了一个集中管理第三方模块的网站：**PyPI(python package index)**。这里维护着Flask的最新版本。第三方模块需要先安装和导入才能使用。因此先安装Flask。  

## 1.1 安装Flask
用pip命令行安装Flask：  
Linux/UNIX下：  
```
sudo -H python3 -m pip install flask
```
Windows上：  
```
py -3 -m pip install flask
```
稍作等待，一会就安装完成了.  
**接下来确认flask是否安装成功：**   
为此，先开发一个程序，命名为hello_flask.py，如下：  
```py
# flask为模块名，Flask为类名，你也可以import flask，然后flask.Flask这么使用类Flask
from flask import Flask
app = Flask(__name__)
@app.route('/')
def hello() -> str:
    return 'Hello world from Flask!'
app.run()
```
然后在在操作系统命令行运行这个web程序：  
```
py -3 hello_flask.py
python3 hello_flask.py
```
如果Flask安装成功，会输出:  
```
 * Debug mode: off
 * Running on http://127.0.0.1:5000/ (Press CTRL+C to quit)
```
这时在浏览器输入http://127.0.0.1:5000/，会发现浏览器显示如下信息:  
```
Hello world from Flask!
```
就是   
```py
return 'Hello world from Flask!'
```
返回的。如此证明我们的Flask安装成功了。  

# 2. Flask快速入门
```py
from flask import Flask
app = Flask(__name__)
@app.route('/')
def hello() -> str:
    return 'Hello world from Flask!'
app.run()
```
1. 第二行代码创建了一个Flask对象。并赋值给app。其中__name__由python解释器维护，在程序代码中的任何地方使用这个值时，它会设置为当前活动模块的名字。创建Flask对象时，Flask需要知道__name__的当前值。
2. 第三行代码引入了python的新语法：**修饰符**。函数修饰符(这个代码中就是函数修饰符)可以调整一个现有函数的行为，而无需修改这个函数的代码(也就是说函数得到了修饰)，本质上说，修饰符允许你根据需要为已有的代码增加额外的行为。修饰符不仅可以用于函数，也可以用于类。不过主要用来修饰类。以@开头。你也可以创建自己的修饰符(*后边学习，现在先使用*)python中提供了一组内置的修饰符，很多第三方模块(Flask)针对特定用途也提供了修饰符(route就是其中之一，这里就是用URL修饰函数)。**route修饰符允许你将一个URL路径与一个已有的python函数关联。**这里，/路径将与下一行代码定义的函数hello关联。当一个指向/的URL请求到达服务器时，route就会安排Flask调用这个函数。然后route会等待所修饰的函数生成的输出。再将输出返回给服务器，服务器将输出返回给浏览器。    
3. app.run()用于启动web应用。此时Flask会启动它的内置web服务器，并在这个服务器中运行你的代码。  

此时，输入http://127.0.0.1:5000/会展示：Hello world from Flask!，其他路径(如：http://127.0.0.1:5000/fdf)会展示：  
```
Not Found{404错误}
The requested URL was not found on the server. If you entered the URL manually please check your spelling and try again.
```
接下来为web应用提供功能：为web支持路径/search4，来调用第四章的模块vsearch(前面已经将该模块安装在site-packages了，如果你没有安装，请看第四章)中的函数search4letters，完成在"life, the universe, and everything!"中搜索 'eiru,!'。将结果返回浏览器。我们只需要使用route修饰符来为/search与一个函数建立关联即可，这个函数调用search4letters。如下为代码：  
```py
from flask import Flask
from vsearch import search4letters  # 要先安装在site-packages中，不会安装去学习第四章

app = Flask(__name__)

@app.route('/')
def hello() -> str:
    return 'Hello world from Flask!'

@app.route('/search4')
def do_search() -> str:
    return str(search4letters('life, the universe, and everything', 'eiru,!'))
app.run()
```
浏览器输入：http://127.0.0.1:5000/search4 。返回：{'i', 'e', 'r', ',', 'u'}   

以上只是简单的输出服务器的计算结果，我们的需求是需要接收浏览器的输入的。search4letters函数的两个参数都要从浏览器获取用户的输入，然后计算结果，并返回，该怎么接收浏览器输入呢？   

**具体来说，我们需要在用户输入http://127.0.0.1:5000后返回一个html页面，这个页面可以接收两个参数。点击按钮提交用户输入，然后返回一个html作为计算结果。即：需要返回两个html页面给用户：一个用于输入，一个用于返回结果。接下来看看该怎么做。**    

我们需要创建一个html返回给浏览器。创建html有多种方式：  
1. 可以把html放在字符串里，返回这些字符串，这样的话，所有的一切都在我的python代码中；  
2. 上边的当然可以完成，不过随着web应用变大，页面变多，上边的方式显然不行，为此，我们使用一个模板引擎来基于模板生成html页面。这里我们就用这种方式。  

## 2.1 Flask呈现模板
Flask提供的模板引擎是**Jinja2**，它易于使用，而且功能强大。Jinja2的更多信息见：http://jinja.pocoo.org/docs/dev/  
下边我们的web应用基于基模板base.html：  
```html
<!doctype html>
<html>
    <head>
        <title>{{ the_title }}</title>
        <link rel="stylesheet" href="static/hf.css" />  <!--引入一个样式-->
    </head>
    <body>
        {% block body %}

        {% endblock %}  <!--{% block body %} 和 {% endblock %}之间的内容由继承该模板的页面提供-->
    </body>
</html>
```
其中{{ 和 }}包起来的，{% 和 %}包起来的是针对Jinja2的。  
{{ 和 }}包起来的是Jinja2的指令，指示在呈现之前提供的一个值，可以理解成模板的参数，需要python程序设置。  
{% 和 %}指示要在呈现之前替换这里的html块。这要**由继承这个模板的页面提供**。  

第一个接收用户输入的Jinja2的html模板为：entry.html:  
```html
{% extends 'base.html' %} <!--继承自base.html-->

{% block body %}

<h2>{{ the_title }}</h2> <!--这个由python程序设置-->

<form method='POST' action='/search4'>   <!-- 这个表单以post形式提交给 /search4 路径  -->
<table>
<p>Use this form to submit a search request:</p>
<tr><td>Phrase:</td><td><input name='phrase' type='TEXT' width='60'></td></tr>
<tr><td>Letters:</td><td><input name='letters' type='TEXT' value='aeiou'></td></tr>
</table>
<p>When you're ready, click this button:</p>
<p><input value='Do it!' type='SUBMIT'></p> <!--按钮上的值为Do it!,点击之后提交表单，以post方式提交到/search4-->
</form>

{% endblock %} 
```
类似的，另一个结果页面results.html:  
```html
{% extends 'base.html' %}

{% block body %}

<h2>{{ the_title }}</h2> <!--这个由python程序设置-->

<p>You submitted the following data:</p>
<table>
<tr><td>Phrase:</td><td>{{ the_phrase }}</td></tr>  <!--这个the_phrase由python程序设置-->
<tr><td>Letters:</td><td>{{ the_letters }}</td></tr>  <!--这个the_letters由python程序设置-->
</table>

<p>When "{{the_phrase }}" is searched for "{{ the_letters }}", the following results are returned:</p>
<h3>{{ the_results }}</h3> <!--这个the_results由python程序设置-->

{% endblock %}
```
有了模板接下来看看Flask如何呈现模板。  
Flask提供了一个render_template的函数。为了呈现entry.html给用户，需要做几件事：  
 - 导入render_template的函数。  
 - 创建新的url路径/entry。  
 - 用/entry修饰某个函数，返回entry.html。  
代码如下：  
```py
from flask import Flask, render_template
from vsearch import search4letters

app = Flask(__name__)

@app.route('/')
def hello() -> str:
    return 'Hello world from Flask!'

@app.route('/search4')
def do_search() -> str:
    return str(search4letters('life, the universe, and everything', 'eiru,!'))

@app.route('/entry')
def entry_page() -> 'html':
    return render_template('entry.html',  # 模板
                           the_title='Welcome to search4letters on the web!')  # 指定entry.html模板中的the_title参数。
app.run()
``` 
我们的代码路径如下:  
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
```
运行：  
```
python3 vsearch4web.py
py -3 vsearch4web.py
```
这时候输入http://127.0.0.1:5000/entry就会返回一个表单页面。   
**但是点击按钮Do it!会出现如下错误页面，并不会调用到我们的函数do_search**:   
```
Method Not Allowed
The method is not allowed for the requested URL.
```
服务器会出现如下错误信息：  
```
127.0.0.1 - - [18/Apr/2019 23:00:41] "POST /search4 HTTP/1.1" 405 -
```
详细的http错误码这里不解释。405表示客户端(浏览器)使用了服务器不允许的一个http方法来发送请求。因为前面在entry.html中你会看到，点击[Do it!]会以POST方法提交表单：  
```html
<form method='POST' action='/search4'>   <!-- 这个表单以post形式提交给 /search4 路径  -->
<table>
.......
```
而Flask默认为GET方法，为此需要为修饰符route增加methods参数:   
```py
@app.route('/search4')  # 默认为GET
@app.route('/search4', methods=['POST'])  # /search4现在只支持POST
@app.route('/search4', methods=['GET', 'POST'])  # /search4现在只支持GET和POST flask更多信息见: http://flask.pocoo.org
```
修改完之后就可以停止，重新运行了。不过这个有点麻烦，你可以开启debug模式，这样就不用一直这个停止运行，每次Flask注意到代码改了，就会自动重启它的web服务，节省了不少事。为了开启debug模式，需要：  
```py
app.run(debug=True)
```
如此代码为:  
```py

from flask import Flask, render_template  # 需要导入render_template
from vsearch import search4letters

app = Flask(__name__)

@app.route('/')
def hello() -> str:
    return 'Hello world from Flask!'

@app.route('/search4', methods=['POST'])  # 仅支持POST
def do_search() -> str:
    return str(search4letters('life, the universe, and everything', 'eiru,!'))

@app.route('/entry')
def entry_page() -> 'html':
    return render_template('entry.html',  # 模板
                           the_title='Welcome to search4letters on the web!')  # 指定entry.html模板中的the_title参数。
app.run(debug=True)
```
点击按钮Do it!就会出现：  
```
{'r', 'u', 'e', ',', 'i'}   # 在"life, the universe, and everything" 中搜索 "eiru,!"的结果。不论你输入什么结果哦都一样。
```
上边的例子我们利用模板返回的表单，只不过服务端并没有获取表单数据，接下来看看服务端如何获取表单数据并处理。  

## 2.2 Flask处理表单数据
为了获取表单数据，Flask提供了一个对象request。该对象包含form的字典属性，用于获取表单数据。之前我们的entry.html中表单项的名字是：  
```html
<tr><td>Phrase:</td><td><input name='phrase' type='TEXT' width='60'></td></tr>
<tr><td>Letters:</td><td><input name='letters' type='TEXT' value='aeiou'></td></tr>
```
即：phrase和letters，因此你可以这么获取：  
```py
phrase = request.form['phrase']
letters = request.form['letters']
```
接下来看看我们的代码实现:  
```html
{% extends 'base.html' %}

{% block body %}

<h2>{{ the_title }}</h2> <!--这个由python程序设置-->

<p>You submitted the following data:</p>
<table>
<tr><td>Phrase:</td><td>{{ the_phrase }}</td></tr>  <!--这个the_phrase由python程序设置-->
<tr><td>Letters:</td><td>{{ the_letters }}</td></tr>  <!--这个the_letters由python程序设置-->
</table>

<p>When "{{the_phrase }}" is searched for "{{ the_letters }}", the following results are returned:</p>
<h3>{{ the_results }}</h3> <!--这个the_results由python程序设置-->

{% endblock %}
```
the_title、the_phrase、the_letters、the_results都需要我们去设置。  
```py
from flask import Flask, render_template, request  # 需要导入request
from vsearch import search4letters

app = Flask(__name__)

@app.route('/')
def hello() -> str:
    return 'Hello world from Flask!'

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

@app.route('/entry')
def entry_page() -> 'html':
    return render_template('entry.html',  # 模板
                           the_title='Welcome to search4letters on the web!')  # 指定entry.html模板中的the_title参数。
app.run(debug=True)
```

## 2.3 Flask重定向
现在我们这个web应用可以接受三个url: /, /entry, /search4。这时我们觉得/没有必要，希望重定向到/entry，可以这么做： 
```py
from flask import Flask, render_template, request, redirect  # 需要导入request
@app.route('/')
def hello() -> str:
    return 'Hello world from Flask!'
# 改为
@app.route('/')
def hello() -> '302':
    return redirect('/entry')   # 指示浏览器请求另一个URL
```
这时输入/和/entry都会返回entry.html这个表单页面。  

作为一个策略，使用重定向是可以满足需求的。不过有些浪费，每次指向/的请求都会变成两个：  
 - / 请求到hello。  
 - 依据hello的响应重新请求/entry  

实际上Flask可以让一个函数与多个URL关联,如果有多个关联，Flask会依次与各个URL匹配，如果找到一个匹配，就会执行该函数。代码如下：      
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
app.run(debug=True)
```

我们想要把web应用部署到云上怎么办?你可以部署到PythonAnyWhere。与几乎所有云托管的部署方案一样，PythonAnyWhere喜欢控制web应用的启动，这意味着你不用调用app.run。因为PythonAnyWhere会去调用，如果你调用了，PythonAnyWhere会拒绝运行你的web应用。  
那么在部署到PythonAnyWhere时需要删掉代码app.run。不过在调试时又需要加上。这有时候显得麻烦。不是吗？那么怎么解决呢？？？  

还记得：__name__吗？__name__值由解释器维护，指出了当前活动的命名空间，值为活动模块的名字。我们可以利用这一点。为此，先理解下它：  
我们有个文件:hunder.py内容如下：  
```py
print('We start off in:', __name__)
if __name__ == '__main__':
    print('And end up in:', __name__)
```
直接运行：py -3 hunder.py
会显示：  
```
We start off in: __main__
And end up in: __main__
```
__name__的值为__main__  
如果import dunder则结果如下：  
```
>>> import dunder
We start off in: dunder
>>>
```
__name__的值为dunder(模块名字)   
实际上，PythonAnyWhere运行你的python应用时也是作为模块导入后自己运行app.run。因此你可以这么做（**很多python程序都这么做**）：  
```py
app.run(debug=True)
# 改为
if __name__ == '__main__': 
    app.run(debug=True)
```

到此，本章的讨论就结束了，不过在实际应用中，web服务器都会记录日志以及其他数据，这些数据怎么处理呢？接下来一章就来学习python存储和管理数据。  

