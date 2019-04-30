# 1. 概述
本章主要学习python与数据库的交互，python提供了DB-API作为与数据库交互的接口，这些接口需要由数据库厂商去实现。  
本章我们要做如下几件事情来回答上一章遗留的问题。
1. 安装MySQL服务器  
2. 为python安装一个MySQL数据库驱动程序  
3. 为web应用创建数据库和表  
4. 创建代码来处理这个web应用的数据库和表  

# 2. 准备工作
本节主要是安装mysql和python的驱动程序，然后创建对应的用户和表。安装mysql和python的驱动程序可以看看mysql的官网：www.mysql.com.  
安装好之后，会创建root用户。我们使用root用户创建我们所需的用户：  
```sql
create database vsearchlogdb
create user 'vsearch'@'%' identified by 'vsearchpasswd';
grant all on vsearchlogdb.* to 'vsearch'@'%';
```
接下来使用我们创建的用户登录并创建表，为了创建表，先看看我们之前存储的日志的格式：  
```
ImmutableMultiDict([('phrase', 'rgtfhyjio'), ('letters', 'aeiou')])|127.0.0.1|Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.139 Safari/537.36|{'i', 'o'}
```
可以看到，我们需要几个字段：  
 - phrase：输入的字符串  
 - letters：带搜索的字符串  
 - ip：客户端ip地址  
 - browser_string：浏览器标识  
 - results：搜索结果  
同时我们增加个id作为主键，ts为记录的更新时间，建表语句如下：  
```sql
use vsearchlogdb; 
CREATE TABLE `log` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `ts` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `phrase` varchar(128) NOT NULL,
  `letters` varchar(32) NOT NULL,
  `ip` varchar(16) NOT NULL,
  `browser_string` varchar(256) NOT NULL,
  `results` varchar(64) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

desc log;
+----------------+--------------+------+-----+-------------------+-------------------+
| Field          | Type         | Null | Key | Default           | Extra             |
+----------------+--------------+------+-----+-------------------+-------------------+
| id             | int(11)      | NO   | PRI | NULL              | auto_increment    |
| ts             | timestamp    | YES  |     | CURRENT_TIMESTAMP | DEFAULT_GENERATED |
| phrase         | varchar(128) | NO   |     | NULL              |                   |
| letters        | varchar(32)  | NO   |     | NULL              |                   |
| ip             | varchar(16)  | NO   |     | NULL              |                   |
| browser_string | varchar(256) | NO   |     | NULL              |                   |
| results        | varchar(64)  | NO   |     | NULL              |                   |
+----------------+--------------+------+-----+-------------------+-------------------+
7 rows in set (0.00 sec)
```
如此就做好了基础工作。完成了1，2，3步。接下来就是编写python代码，处理这个web应用的数据库和表。  
python通过DB-API连接数据库的整体结构如下：  
```
你的代码 ---> Python DB-API ---> MySQL-Connector/Python驱动程序 ---> MySQL数据库服务器
```
通过使用Python DB-API，你可以轻松替换驱动程序和数据库服务器，而不需要修改你的python代码。  

# 3. python连接数据库
接下来看看我们的代码：  
1. 定义连接属性  
```py
dbconfig = { 'host': '127.0.0.1',
    'user': 'vsearch',
    'password': 'vsearchpasswd',
    'database': 'vsearchlogDB', 
    'port': 3309, # 默认为3306，我安装的数据库设置成了3309
    'charset':'utf8mb4',}
```
2. 导入数据库驱动程序  
```py
import mysql.connector
```
3. 与数据库服务器建立连接    
```py
conn = mysql.connector.connect(**dbconfig)
```
**注意：dbconfig的传入，之前有两个星号，如果你是C/C++程序员，别误以为这是指针，python没有指针的概念**。**记法告诉connect函数，我们提供的参数为一个字典。在后续章节会看到跟多**记法，现在先直接使用。  
4. 打开一个游标  
```py
cursor = conn.cursor() # 创建一个游标来向数据发送语句以及接受结果
```

5. 完成SQL查询  
```py
_SQL = """show tables"""
cursor.execute(_SQL)
res = cursor.fetchall()
res # 显示结果
[('log',)]
_SQL = """describe log"""
cursor.execute(_SQL)
res = cursor.fetchall()
res
[('id', 'int(11)', 'NO', 'PRI', None, 'auto_increment'), ('ts', 'timestamp', 'YES', '', 'CURRENT_TIMESTAMP', 'DEFAULT_GENERATED'), ('phrase', 'varchar(128)', 'NO', '', None, ''), ('letters', 'varchar(32)', 'NO', '', None, ''), ('ip', 'varchar(16)', 'NO', '', None, ''), ('browser_string', 'varchar(256)', 'NO', '', None, ''), ('results', 'varchar(64)', 'NO', '', None, '')]
for row in res:
    print(row)
('id', 'int(11)', 'NO', 'PRI', None, 'auto_increment')
('ts', 'timestamp', 'YES', '', 'CURRENT_TIMESTAMP', 'DEFAULT_GENERATED')
('phrase', 'varchar(128)', 'NO', '', None, '')
('letters', 'varchar(32)', 'NO', '', None, '')
('ip', 'varchar(16)', 'NO', '', None, '')
('browser_string', 'varchar(256)', 'NO', '', None, '')
('results', 'varchar(64)', 'NO', '', None, '')
# 和在mysql客户端上执行desc log显示的结果一样。  

# cursor.fetchall(): 获取所有结果  
# cursor.fetchmany(): 获取你指定的任意行结果
# cursor.fetchone(): 获取一行结果

# 接下来看看插入数据
_SQL = """insert into log
(phrase, letters, ip, browser_string, results)
values
('hitch-hiker', 'aeiou', '127.0.0.1', 'Firefox', "{'e', 'i'}")"""
cursor.execute(_SQL)
# 使用占位符插入数据
_SQL = """insert into log
(phrase, letters, ip, browser_string, results)
values
(%s, %s, %s, %s, %s)"""
cursor.execute(_SQL, ('hitch-hiker', 'xyz', '127.0.0.1', 'Safari', 'set()'))
# cursor.execute最后接受两个参数，因此我们用5个参数构造了一个元组。  
conn.commit() # 为了确保数据保存在了数据库中，执行commit。注意：不是cursor的commit，而是conn的commit

_SQL = """select * from log"""
cursor.execute(_SQL)
for row in cursor.fetchall():
    print(row)
    
(1, datetime.datetime(2019, 4, 30, 22, 59, 57), 'hitch-hiker', 'aeiou', '127.0.0.1', 'Firefox', "{'e', 'i'}")
(2, datetime.datetime(2019, 4, 30, 23, 0, 37), 'hitch-hiker', 'xyz', '127.0.0.1', 'Safari', 'set()')
```
6. 关闭游标和连接  
```py
cursor.close() # 关闭游标
True
conn.close() # 关闭连接
#关闭这些资源是必须的。
```
从mysql的客户端程序上查询表，确实也发现插入成功了：  
```sql
mysql> select * from log;
+----+---------------------+-------------+---------+-----------+----------------+------------+
| id | ts                  | phrase      | letters | ip        | browser_string | results    |
+----+---------------------+-------------+---------+-----------+----------------+------------+
|  1 | 2019-04-30 22:59:57 | hitch-hiker | aeiou   | 127.0.0.1 | Firefox        | {'e', 'i'} |
|  2 | 2019-04-30 23:00:37 | hitch-hiker | xyz     | 127.0.0.1 | Safari         | set()      |
+----+---------------------+-------------+---------+-----------+----------------+------------+
2 rows in set (0.00 sec)

mysql>
```
有了以上基础就可以编写我们的代码了：  
将写文件改成写数据库：  
```py
def log_request(req: 'flask_request', res: str) -> None:
    with open('vsearch.log', 'a') as log:
        print(req.form, req.remote_addr, req.user_agent, res, file=log, sep='|')
```
写数据库的代码：  
```py
def log_request(req: 'flask_request', res: str) -> None:
    dbconfig = { 'host': '127.0.0.1',
        'user': 'vsearch',
        'password': 'vsearchpasswd',
        'database': 'vsearchlogDB', 
        'port': 3309, # 默认为3306，我安装的数据库设置成了3309
        'charset':'utf8mb4',}
    import mysql.connector
    conn = mysql.connector.connect(**dbconfig)
    cursor = conn.cursor() # 创建一个游标来向数据发送语句以及接受结果
    _SQL = """insert into log
            (phrase, letters, ip, browser_string, results)
            values
            (%s, %s, %s, %s, %s)"""
    cursor.execute(_SQL, (req.form['phrase'], 
                    req.form['letters'], 
                    req.remote_addr, 
                    req.user_agent.browser, 
                    res,))
    conn.commit()
    cursor.close() # 关闭游标
    conn.close() # 关闭连接
```
这时候再搜索一下，通过mysql的客户端看看结果就插入到了数据库中：  
```sql
# 搜索前
mysql> select * from log;
+----+---------------------+-------------+---------+-----------+----------------+------------+
| id | ts                  | phrase      | letters | ip        | browser_string | results    |
+----+---------------------+-------------+---------+-----------+----------------+------------+
|  1 | 2019-04-30 22:59:57 | hitch-hiker | aeiou   | 127.0.0.1 | Firefox        | {'e', 'i'} |
|  2 | 2019-04-30 23:00:37 | hitch-hiker | xyz     | 127.0.0.1 | Safari         | set()      |
+----+---------------------+-------------+---------+-----------+----------------+------------+
2 rows in set (0.00 sec)

# 使用谷歌浏览器搜索后
mysql> select * from log;
+----+---------------------+-------------+---------+-----------+----------------+-----------------+
| id | ts                  | phrase      | letters | ip        | browser_string | results         |
+----+---------------------+-------------+---------+-----------+----------------+-----------------+
|  1 | 2019-04-30 22:59:57 | hitch-hiker | aeiou   | 127.0.0.1 | Firefox        | {'e', 'i'}      |
|  2 | 2019-04-30 23:00:37 | hitch-hiker | xyz     | 127.0.0.1 | Safari         | set()           |
|  3 | 2019-04-30 23:52:26 | thfhghiosa  | aeiou   | 127.0.0.1 | chrome         | {'a', 'o', 'i'} |
+----+---------------------+-------------+---------+-----------+----------------+-----------------+
```
这时候你就可以通过查询数据库来回答之前的几个问题了。  
# 4. 剩下的问题
上边解决了将请求存储到数据库中的问题，你还记得还有一个view_the_log函数吗？这个也要从数据库中查询获取日志。  
接下来，你可以将以上代码在view_the_log函数中再copy一遍。这其实不难，不过这个不够好。你应该考虑重用你的代码，因为链接数据库的代码是通用的。你可以抽象出来。同时要注意：  
```
千万不要在函数内部使用import，因为每次调用函数都会import，这是昂贵的操作。因此上面我们的函数功能虽然实现了，但是性能非常低。因此要将import放在外边。  
```
本质上，之前的代码是：建立、处理、清理模式，如果我们有办法重用这种模式就好了，还记得之前对文件的处理吗：  
```py
with open('todos.txt') as tasks:
    for line in tasks:
        print(line, end='')
```
还记得：with语句会管理运行其代码组代码的上下文。我们最好能把我们的数据库代码挂接到with语句中，让with语句负责处理数据库连接的建立和清理工作那就太好了，就像这样：  
```py
dbconfig = { 'host': '127.0.0.1',
        'user': 'vsearch',
        'password': 'vsearchpasswd',
        'database': 'vsearchlogDB', 
        'port': 3309,
        'charset':'utf8mb4',}
with UseDatabase(dbconfig) as cursor: # 这个with语句处理数据库而不是文件，并返回一个游标来处理
    _SQL = """insert into log
        (phrase, letters, ip, browser_string, results)
        values
        (%s, %s, %s, %s, %s)"""
    cursor.execute(_SQL, (req.form['phrase'],
                req.form['letters'],
                req.remote_addr,
                req.user_agent.browser,
                res, ))
```
**好消息是python提供了上下文管理协议，它允许程序员根据需要挂接with语句，不过为了做到这一点，通常你需要创建python类来成功挂接这个协议。**  
为了更好的处理数据库连接，我们需要先学习类的知识，然后我们再编写上下文管理器：UseDatabase。  
以下是本章完整的代码：  
```py
from flask import Flask, render_template, request, escape
from vsearch import search4letters

app = Flask(__name__)


def log_request(req: 'flask_request', res: str) -> None:
    dbconfig = { 'host': '127.0.0.1',
        'user': 'vsearch',
        'password': 'vsearchpasswd',
        'database': 'vsearchlogDB',
        'port': 3309, # 默认为3306，我安装的数据库设置成了3309
        'charset':'utf8mb4',}
    import mysql.connector
    conn = mysql.connector.connect(**dbconfig)
    cursor = conn.cursor() # 创建一个游标来向数据发送语句以及接受结果
    _SQL = """insert into log
            (phrase, letters, ip, browser_string, results)
            values
            (%s, %s, %s, %s, %s)"""
    cursor.execute(_SQL, (req.form['phrase'],
                    req.form['letters'],
                    req.remote_addr,
                    req.user_agent.browser,
                    res,))
    conn.commit()
    cursor.close() # 关闭游标
    conn.close() # 关闭连接


@app.route('/search4', methods=['POST'])
def do_search() -> 'html':
    """Extract the posted data; perform the search; return results."""
    phrase = request.form['phrase']
    letters = request.form['letters']
    title = 'Here are your results:'
    results = str(search4letters(phrase, letters))
    log_request(request, results)
    return render_template('results.html',
                           the_title=title,
                           the_phrase=phrase,
                           the_letters=letters,
                           the_results=results,)

@app.route('/')
@app.route('/entry')
def entry_page() -> 'html':
    """Display this webapp's HTML form."""
    return render_template('entry.html',
                           the_title='Welcome to search4letters on the web!')


@app.route('/viewlog')
def view_the_log() -> 'html':
    """Display the contents of the log file as a HTML table."""
    contents = []
    with open('vsearch.log') as log:
        for line in log:
            contents.append([])
            for item in line.split('|'):
                contents[-1].append(escape(item))
    titles = ('Form Data', 'Remote_addr', 'User_agent', 'Results')
    return render_template('viewlog.html',
                           the_title='View Log',
                           the_row_titles=titles,
                           the_data=contents,)


if __name__ == '__main__':
    app.run(debug=True)
```

下一章就学习python的类。  
