# 安全性
安全性是一个非常大的话题，包括了方方面面，本章主要学习与mysql安全相关的知识。  
在考虑MySQL安装中的安全性时，你应该考虑各种可能的主题以及它们如何影响MySQL服务器和相关应用程序的安全性：  
 - 1、影响安全性的一般因素。其中包括选择好的密码，不向用户授予不必要的权限，通过防止SQL注入和数据损坏来确保应用程序的安全性等。具体见[ Section 6.1, General Security Issues](https://dev.mysql.com/doc/refman/8.0/en/general-security-issues.html)。
 - 2、安装本身的安全性。见[Section 2.9, Postinstallation Setup and Testing”.](https://dev.mysql.com/doc/refman/8.0/en/postinstallation.html)。  
 - 3、数据库系统本身的访问控制和安全性。本章的主要内容。  
 - 4、安全插件相关的特性。  
 - 5、MySQL和系统的网络安全性。安全性与个人用户的授权有关，你可能还希望限制MySQL的访问，使其仅在安装MySQL服务器的主机上可用，或者在一组有限的其他主机上可用。  
 - 6、做好数据的备份。设计好备份方案，做充分的测试。  

# 1、一般安全问题
## 1.1 安全指南
在连接到Internet的计算机上使用MySQL的任何人都应阅读本节以避免最常见的安全错误。在讨论安全性时，有必要考虑完全保护整个服务器主机（不仅仅是MySQL服务器）免受所有类型的攻击，例如：窃听，更改，回放和拒绝服务等。  

MySQL使用基于访问控制列表（ACL）的方式来处理用户可以尝试执行的所有连接，查询和其他操作。为了安全的传输数据，MySQL客户端和服务器之间也支持SSL加密连接。这里讨论的许多概念不是针对MySQL的，很多概念在其他应用程序上也是相同的。  

运行MySQL时，请遵循以下准则：  
 - **不要让任何人（MySQL root帐户除外 ）访问系统数据库mysql中的 user表！这很关键**。
 - 了解和学习MySQL访问权限系统的工作原理（本章会学习），使用 GRANT和 REVOKE语句来控制对MySQL的访问。不要授予多余的权限。永远不会授予所有主机权限。
  > 尝试mysql -u root，如果能够登录，这将是非常危险的，这时要查看MySQL安装说明，特别注意有关设置root密码的信息。具体见[2.9.4 Securing the Initial MySQL Account](https://dev.mysql.com/doc/refman/8.0/en/default-privileges.html).
  > 使用show grants语句检查用户拥有的权限，通过revoke删除不必要的权限。  
 - 不要在数据库中存储明文密码。否则，如果你的计算机受到攻击，入侵者可以获取完整的密码列表并使用它们。相反，应该使用 SHA2()或一些其他单向散列函数并存储散列值。同时为了避免使用rainbow tables破解密码，要对密码加盐后使用这些散列函数，即：hash(passwd + salt)。   
 - 不要使用容易猜测的密码，比如生日、英文单词等。  
 - 将MySQL运行在防火墙之后。不应从不受信任的主机访问MySQL的监听端口。  
 - 访问MySQL的应用程序不应该信任用户输入的任何数据，应该使用适当的防御性编程技术编写。  
 - 不要通过Internet传输普通（未加密）数据。推荐使用MySQL内置的SSL连接，加密传输数据。  
 - 学习使用tcpdump和 字符串实用程序。在大多数情况下，您可以通过发出如下命令来检查MySQL数据流是否未加密：
  > shell> tcpdump -l -i eth0 -w - src or dst port 3306 | strings

## 1.2 保持密码安全
密码出现在MySQL中的几个上下文中。以下各节提供了一些准则，使最终用户和管理员能够保护这些密码的安全，并避免暴露这些密码。此外，validate_password插件可用于对可接受的密码实施策略。  

### 1.2.1 终端用户密码安全指南
普通终端用户应该遵循如下指南：  
当你通过mysql客户端程序连接到mysql服务器时，最好不要以一种公开来的方式给其他用户获取密码。此处列出了运行客户端程序时可用于指定密码的方法，以及每种方法的风险评估。简而言之，最安全的方法是让客户端程序提示输入密码或在受适当保护的配置文件中指定密码。  
 - 1、使用mysql_config_editor实用程序，该程序可以将登录身份信息存储在用户home目录的 .mylogin.cnf文件中，该文件经过了加密，因此，密码是不可见的。MySQL客户端程序稍后可以读取该文件以获取用于连接到MySQL服务器的身份验证凭据。这种比较安全  
```shell
shell> mysql_config_editor set --login-path=client
         --host=localhost --user=localuser --password
Enter password: enter password "localpass" here
shell> mysql_config_editor set --login-path=remote
         --host=remote.example.com --user=remoteuser --password
Enter password: enter password "remotepass" here

shell> mysql_config_editor print --all
[client]
user = localuser
password = *****
host = localhost
[remote]
user = remoteuser
password = *****
host = remote.example.com
```
 - 2、在命令行上：mysql -u francis -pfrank db_name  这个非常不安全，不推荐
 - 3、这种是安全的
 ```sql
 shell> mysql -u francis -p db_name
 Enter password: ********
 ```
 但是，这种输入密码的方法仅适用于以交互方式运行的程序。如果要从非交互式运行的脚本调用客户端，则无法从键盘输入密码。这时就要用到.mylogin.cnf这种加密文件的方式了。  
 - 4、将密码存储在选项文件中。例如，在Unix上，您可以在主目录[client]中的.my.cnf文件部分 列出您的密码 ：
 ```sql
 [client]
 password=your_pass
 ```
 这个不安全，不推荐。如果非要使用，除了你自己之外，任何人都无法访问该文件：shell> chmod 600 .my.cnf  
 - 5、将密码存储在MYSQL_PWD 环境变量中。这种是最不安全的，坚决不能使用。

总的来说就是不要在系统上留下明文密码，如果万不得已留下了，就要限制这些文件的访问权限为自己只读。例如mysql客户端用于记录sql操作的历史文件：
.mysql_history（位于用户home目录），linux的命令历史文件~/.bash_history等，都要设置成只有自己可读。  

### 1.2.2 管理员密码安全指南
数据库管理员应该遵循如下指南：  
 - 1、MySQL在mysql.user表中存储用户帐户的密码。永远不应该向任何非管理帐户授予对此表的访问权限。  
 - 2、帐户密码可以过期，以便用户必须重置它们。  
 - 3、validate_password插件可用于对可接受的密码强制实施策略（哪一类密码是合理的，比如密码长度、必须包含字母，数字等策略）。  
 - 4、应该保护可能写入密码的诸如日志文件等文件。  
 - 5、有权修改插件目录（plugin_dir系统变量的值）或my.cnf文件（其中可以指定插件目录的位置）的用户可以替换插件并修改插件提供的功能，包括身份验证插件。**这一点要特别注意**。  

### 1.2.3 密码和日志


# 3、MySQL用户帐户管理
## 3.1 用户名和密码
mysql将账户信息存储在系统数据库mysql的user表中。MySQL支持身份验证插件，因此帐户可能使用某种外部身份验证方法进行身份验证。  
mysql的用户名和密码有如下几点需要注意：  
 - MySQL用户名最长可达32个字符。  
 - 要对使用MySQL本机身份验证（由mysql_native_password身份验证插件实现）的帐户进行客户端连接身份验证，服务器将使用存储在user表中的密码。如果服务器使用其他插件对客户端进行身份验证，则插件实现的身份验证方法可能会也可能不会使用存储在user表中的密码。在这种情况下，外部密码也可能用于向MySQL服务器进行身份验证。  
 - 存储在user表中的密码使用特定于插件的算法进行加密。  
 - 如果用户名和密码仅包含ASCII字符，则无论字符集设置如何，都可以连接到服务器。要在用户名或密码包含非ASCII字符时进行连接，则要使用MYSQL_SET_CHARSET_NAME 选项和相应的字符集名称作为参数来调用C API函数mysql_options()。这会使用指定的字符集进行身份认证。标准MySQL客户端程序支持一个--default-character-set选项，该选项就会导致调用mysql_options()。  

 ## 3.2 添加用户帐户
 添加用户要使用对应的用户操作语句，例如：CREATE USER和 GRANT。这些语句使服务器对基础授权表进行适当的修改。这些语句的详细描述见：[Section 13.7.1, “Account Management Statements”](https://dev.mysql.com/doc/refman/8.0/en/account-management-sql.html)。  
**注意：** 不要直接使用insert、update、delete等语句直接修改授权表。避免不必要的问题。  

添加账户举例如下：
```sql
mysql> CREATE USER 'finley'@'localhost' IDENTIFIED BY 'password';
mysql> GRANT ALL PRIVILEGES ON *.* TO 'finley'@'localhost'
    ->     WITH GRANT OPTION;
mysql> CREATE USER 'finley'@'%' IDENTIFIED BY 'password';
mysql> GRANT ALL PRIVILEGES ON *.* TO 'finley'@'%'
    ->     WITH GRANT OPTION;
mysql> CREATE USER 'admin'@'localhost' IDENTIFIED BY 'password';
mysql> GRANT RELOAD,PROCESS ON *.* TO 'admin'@'localhost';
mysql> CREATE USER 'dummy'@'localhost';
```
这些语句创建了如下账号：  
 - 两个名为finley的账户。二者都是具有所有权限的超级账户。'finley'@'localhost'仅能从本机连接mysql服务器，'finley'@'%'可以从任意机器连接mysql服务器。  
 - 'admin'@'localhost'，表示admin只能从本机连接mysql服务器，admin具有RELOAD 和PROCESS管理权限。这些特权使 admin用户能够执行 mysqladmin reload。  
 - 'dummy'@'localhost'帐户没有密码（这是不安全的，不推荐）。此帐户只能用于从本地主机进行连接。没有授予任何权限。  

查看账户权限：  
```sql
mysql> SHOW GRANTS FOR 'admin'@'localhost';
+-----------------------------------------------------+
| Grants for admin@localhost                          |
+-----------------------------------------------------+
| GRANT RELOAD, PROCESS ON *.* TO 'admin'@'localhost' |
+-----------------------------------------------------+
```
查看账户的非权限属性：  
```sql
mysql> SHOW CREATE USER 'admin'@'localhost'\G
*************************** 1. row ***************************
CREATE USER for admin@localhost: CREATE USER 'admin'@'localhost'
IDENTIFIED WITH 'mysql_native_password'
AS '*67ACDEBDAB923990001F0FFB017EB8ED41861105'
REQUIRE NONE PASSWORD EXPIRE DEFAULT ACCOUNT UNLOCK
```
下一个示例创建三个帐户并授予他们访问特定数据库的权限:   
```sql
mysql> CREATE USER 'custom'@'localhost' IDENTIFIED BY 'password';
mysql> GRANT SELECT,INSERT,UPDATE,DELETE,CREATE,DROP
    ->     ON bankaccount.*
    ->     TO 'custom'@'localhost';
mysql> CREATE USER 'custom'@'host47.example.com' IDENTIFIED BY 'password';
mysql> GRANT SELECT,INSERT,UPDATE,DELETE,CREATE,DROP
    ->     ON expenses.*
    ->     TO 'custom'@'host47.example.com';
mysql> CREATE USER 'custom'@'%.example.com' IDENTIFIED BY 'password';
mysql> GRANT SELECT,INSERT,UPDATE,DELETE,CREATE,DROP
    ->     ON customer.*
    ->     TO 'custom'@'%.example.com';
```
 - 第一个帐户可以访问bankaccount数据库，但只能从本地主机访问数据库服务器。  
 - 第二个帐户可以访问expenses 数据库，但只能从主机 host47.example.com访问数据库服务器。  
 - 第三个账户可以访问customer数据库，但只能从example.com 域中的任何主机访问数据库服务器。  

## 3.3 删除用户账户
mysql> DROP USER 'jeffrey'@'localhost';  
主机名部分可以省略。  

## 3.4 使用角色
mysql角色是一组权限的集合。理解角色的时候，可以与用户账户类比，即：这个账户有一个权限集，你可以将这个特殊账户赋予其他账户，其他账户就有了这个特殊账户（角色）的所有权限。创建的角色信息也位于mysql.user表中。  
有了角色的概念，就可以通过业务概念将权限分组了，例如：read权限分配一个角色（包含select权限），write分配一个角色（包含insert，delete，update权限）。  
以下总结了mysql提供的角色管理功能：  
 - CREATE ROLE和DROP ROLE可以创建和删除角色。  
 - GRANT和REVOKE为用户帐户和角色进行权限分配和撤销。  
 - SHOW GRANTS 显示用户帐户和角色的权限和角色分配情况。  
 - SET DEFAULT ROLE指定默认情况下哪些帐户角色处于活动状态。  
 - SET ROLE更改当前会话中的活动角色。  
 - CURRENT_ROLE（）函数显示当前会话中的活动角色。   
 - mandatory_roles和activate_all_roles_on_login系统变量用于控制在用户登录服务器时，允许定义强制角色并自动激活授予的角色。  

接下来从几个方面学习角色的使用：  
 - 创建角色并授予他们权限  
 - 定义强制性角色  
 - 检查角色权限  
 - 激活角色  
 - 撤消角色或角色权限  
 - 删除角色  
 - 用户和角色互换性  

### 创建角色并授予他们权限  
这里我们假设有三类用户需要对app_db进行造作：
 - 1、做统计分析的只读用户  
 - 2、普通的修改用户  
 - 3、所有权限的用户  

为了避免每次授权时列出很多权限，我们决定使用角色管理，同时，好的角色名字，使得权限集的意义更加明确。为此，首先需要创建角色。  
**创建角色：**  
```sql
CREATE ROLE 'app_developer', 'app_read', 'app_write';
-- 这时在mysql.user中看到如下记录：
select user,host from user;
+------------------+-----------+
| user             | host      |
+------------------+-----------+
| app_developer    | %         |
| app_read         | %         |
| app_write        | %         |
| test             | %         |
| mysql.infoschema | localhost |
| mysql.session    | localhost |
| mysql.sys        | localhost |
| root             | localhost |
+------------------+-----------+
8 rows in set (0.00 sec)
```
角色名称与用户帐户名称非常相似，由用户部分和主机部分组成：'user_name'@'host_name'。主机部分，如果省略，默认为'％'。  
要为角色​​分配权限，请使用与为用户帐户分配权限相同的语法执行GRANT：  
```sql
GRANT ALL ON app_db.* TO 'app_developer';
GRANT SELECT ON app_db.* TO 'app_read';
GRANT INSERT, UPDATE, DELETE ON app_db.* TO 'app_write';
```
现在假设我们需要一个开发人员帐户，两个只读帐户，以及一个读/写帐户。使用CREATE USER创建帐户：  
```sql
CREATE USER 'dev1'@'localhost' IDENTIFIED BY 'dev1pass';
CREATE USER 'read_user1'@'localhost' IDENTIFIED BY 'read_user1pass';
CREATE USER 'read_user2'@'localhost' IDENTIFIED BY 'read_user2pass';
CREATE USER 'rw_user1'@'localhost' IDENTIFIED BY 'rw_user1pass';
```
使用GRANT为用户分配角色：  
```sql
GRANT 'app_developer' TO 'dev1'@'localhost';
GRANT 'app_read' TO 'read_user1'@'localhost', 'read_user2'@'localhost';
GRANT 'app_read', 'app_write' TO 'rw_user1'@'localhost';
```

### 定义强制性角色  
通过设置mandatory_roles系统变量的值可以将角色指定为强制角色。*服务器将强制角色视为授予所有用户*，因此不需要将其明确授予任何帐户。要在服务器启动时指定强制角色，请在my.cnf文件中配置mandatory_roles配置项的值：  
```
[mysqld]
mandatory_roles='role1,role2@localhost,r3@%.example.com'
```
要在运行时设置和持久化mandatory_roles，请使用如下语句：  
```sql
SET PERSIST mandatory_roles = 'role1,role2@localhost,r3@%.example.com';
```
SET PERSIST设置正在运行的MySQL实例的值。它还保存了用于后续服务器重启的值; 请参见[第13.7.5.1节“变量赋值的SET语法”](https://dev.mysql.com/doc/refman/8.0/en/set-variable.html)。要更改正在运行的MySQL实例的值而不保存它以便后续重新启动，请使用GLOBAL关键字而不是 PERSIST。  
除了通常需要设置全局系统变量的SYSTEM_VARIABLES_ADMIN或SUPER权限外，设置mandatory_roles还需要ROLE_ADMIN权限。  
强制角色，确授予的角色，在激活之前不会生效。如果启用了activate_all_roles_on_login系统变量，则会在登录时对所有授予的角色进行角色激活，否则仅对设置为默认角色的角色进行角色激活。在运行时，通过SET ROLE激活角色。  
在mandatory_roles的值中命名的角色不能使用REVOKE撤消，也不能使用DROP ROLE或DROP USER删除。  
如果mysql.user系统表中不存在mandatory_roles中指定的角色，该角色未授予用户。当服务器尝试为用户激活角色时，它不会将不存在的角色视为必需角色，并向错误日志写入警告。如果角色稍后创建并因此变为有效，则需要执行FLUSH PRIVILEGES以使服务器将其视为必需角色。  

SHOW GRANTS显示强制角色的信息。  

### 检查角色权限  
要验证分配给帐户的权限，请使用SHOW GRANTS。例如：  
```sql
mysql> SHOW GRANTS FOR 'dev1'@'localhost';
+-------------------------------------------------+
| Grants for dev1@localhost                       |
+-------------------------------------------------+
| GRANT USAGE ON *.* TO `dev1`@`localhost`        |
| GRANT `app_developer`@`%` TO `dev1`@`localhost` |
+-------------------------------------------------+
```
这只会显示该账户被授予的角色，并不会显示角色拥有的权限，要显示角色权限，请添加一个 USING子句：  
```sql
mysql> SHOW GRANTS FOR 'dev1'@'localhost' USING 'app_developer';
+----------------------------------------------------------+
| Grants for dev1@localhost                                |
+----------------------------------------------------------+
| GRANT USAGE ON *.* TO `dev1`@`localhost`                 |
| GRANT ALL PRIVILEGES ON `app_db`.* TO `dev1`@`localhost` |
| GRANT `app_developer`@`%` TO `dev1`@`localhost`          |
+----------------------------------------------------------+
```
再看看别的账户：  
```sql
mysql> SHOW GRANTS FOR 'read_user1'@'localhost' USING 'app_read';
+--------------------------------------------------------+
| Grants for read_user1@localhost                        |
+--------------------------------------------------------+
| GRANT USAGE ON *.* TO `read_user1`@`localhost`         |
| GRANT SELECT ON `app_db`.* TO `read_user1`@`localhost` |
| GRANT `app_read`@`%` TO `read_user1`@`localhost`       |
+--------------------------------------------------------+
mysql> SHOW GRANTS FOR 'rw_user1'@'localhost' USING 'app_read', 'app_write';
+------------------------------------------------------------------------------+
| Grants for rw_user1@localhost                                                |
+------------------------------------------------------------------------------+
| GRANT USAGE ON *.* TO `rw_user1`@`localhost`                                 |
| GRANT SELECT, INSERT, UPDATE, DELETE ON `app_db`.* TO `rw_user1`@`localhost` |
| GRANT `app_read`@`%`,`app_write`@`%` TO `rw_user1`@`localhost`               |
+------------------------------------------------------------------------------+
```
SHOW GRANTS也可以显示强制角色，详见[第13.7.6.21节“SHOW GRANTS语法”](https://dev.mysql.com/doc/refman/8.0/en/show-grants.html).  

### 激活角色  
授予用户帐户的角色可以在帐户会话中处于活动状态或非活动状态。如果授予的角色在会话中处于活动状态，则其权限适用。要确定当前会话中哪些角色处于活动状态，请使用CURRENT_ROLE()函数。如下表示当前账户会话中没有活动角色：  
```sql
mysql> SELECT CURRENT_ROLE();
+----------------+
| CURRENT_ROLE() |
+----------------+
| NONE           |
+----------------+
```
默认情况下，向帐户授予的角色或在mandatory_roles系统变量值中命名的角色不会自动在帐户会话中变为活动状态。例如之前的rw_user1到目前为止还没有激活任何角色。  

要指定每次用户*连接到服务器并进行身份验证时*应激活哪些角色，请使用 SET DEFAULT ROLE。要将默认设置为先前创建的每个帐户的所有已分配角色，请使用以下语句：  
```sql
SET DEFAULT ROLE ALL TO
  'dev1'@'localhost',
  'read_user1'@'localhost',
  'read_user2'@'localhost',
  'rw_user1'@'localhost';
```
现在对于rw_user1，连接后初始激活的角色为：  
```sql
mysql> SELECT CURRENT_ROLE();
+--------------------------------+
| CURRENT_ROLE()                 |
+--------------------------------+
| `app_read`@`%`,`app_write`@`%` |
+--------------------------------+
```
要在用户连接到服务器时自动激活所有显式授权和强制角色，请启用activate_all_roles_on_login 系统变量。默认情况下禁用。  
在会话中，用户可以执行SET ROLE以更改活动角色集。例如，对于rw_user1：  
```sql
-- 停用所有角色
mysql> SET ROLE NONE; SELECT CURRENT_ROLE();
+----------------+
| CURRENT_ROLE() |
+----------------+
| NONE           |
+----------------+
-- 激活除app_write外的所以角色
mysql> SET ROLE ALL EXCEPT 'app_write'; SELECT CURRENT_ROLE();
+----------------+
| CURRENT_ROLE() |
+----------------+
| `app_read`@`%` |
+----------------+
-- 回复默认角色
mysql> SET ROLE DEFAULT; SELECT CURRENT_ROLE();
+--------------------------------+
| CURRENT_ROLE()                 |
+--------------------------------+
| `app_read`@`%`,`app_write`@`%` |
+--------------------------------+
```
存储程序和视图对象的有效用户受DEFINER和SQL SECURITY属性的约束，这些属性决定执行是在调用者还是定义者上下文中执行,参见 [第23.6节“存储的程序和视图的访问控制”](https://dev.mysql.com/doc/refman/8.0/en/stored-programs-security.html):  

 - 在调用者上下文中执行的存储程序和视图对象使用当前会话中的活动角色。  
 - 在定义上下文中执行的存储程序和视图对象使用DEFINER属性中指定的用户的默认角色执行。如果 activate_all_roles_on_login 启用，则此类对象将执行授予DEFINER用户的所有角色，包括强制角色。对于存储程序，如果执行的角色与默认值不同，则应执行程序体SET ROLE以激活所需的角色。  

### 撤消角色或角色权限  
正如可以将角色授予帐户一样，可以从帐户中撤消这些角色：  
```sql
REVOKE role FROM user;
```
在系统变量mandatory_roles的值中命名的角色无法撤消。  
REVOKE也可以应用于角色以修改授予它的权限。这不仅会影响角色本身，还会影响授予了该角色的任何帐户。例如：  
```sql
REVOKE INSERT, UPDATE, DELETE ON app_db.* FROM 'app_write';
```
这使得app_write角色没有任何权限，正如SHOW GRANTS展示的那样(这表明该语句不仅可以用于账户，也可以与角色一起使用)：  
```sql
mysql> SHOW GRANTS FOR 'app_write';
+---------------------------------------+
| Grants for app_write@%                |
+---------------------------------------+
| GRANT USAGE ON *.* TO `app_write`@`%` |
+---------------------------------------+
-- 没有任何权限
```
这时rw_user1已经没有表修改权限了：  
```sql
mysql> SHOW GRANTS FOR 'rw_user1'@'localhost'
       USING 'app_read', 'app_write';
+----------------------------------------------------------------+
| Grants for rw_user1@localhost                                  |
+----------------------------------------------------------------+
| GRANT USAGE ON *.* TO `rw_user1`@`localhost`                   |
| GRANT SELECT ON `app_db`.* TO `rw_user1`@`localhost`           |
| GRANT `app_read`@`%`,`app_write`@`%` TO `rw_user1`@`localhost` |
+----------------------------------------------------------------+
```
可以看到，rw_user1已经成了只读用户。  
要恢复角色的权限，只需重新授予它们：  
```sql
GRANT INSERT, UPDATE, DELETE ON app_db.* TO 'app_write';
```
现在rw_user1再次拥有修改权限，授予了app_write角色的任何其他帐户也是如此。  

### 删除角色  
要删除角色，请使用DROP ROLE：  
```sql
DROP ROLE 'app_read', 'app_write';
```
删除角色会从授予它的每个帐户撤消该角色。  
在系统变量mandatory_roles的值中命名的角色无法删除。   

### 用户和角色互换性  
账户和角色可以互换使用。可以将用户帐户视为角色，并将该帐户授予其他用户或角色。其结果是将帐户的*权限和角色*授予其他*用户或角色*。  
举个例子：  
```sql
-- 创建用户/角色并将权限授予用户/角色，将用户/角色授予用户/角色
CREATE USER 'u1';
CREATE ROLE 'r1';
GRANT SELECT ON db1.* TO 'u1';
GRANT SELECT ON db2.* TO 'r1';
CREATE USER 'u2';
CREATE ROLE 'r2';
GRANT 'u1', 'r1' TO 'u2';
GRANT 'u1', 'r1' TO 'r2';

-- 看看用户u2和角色r2的权限
mysql> SHOW GRANTS FOR 'u2' USING 'u1', 'r1';
+-------------------------------------+
| Grants for u2@%                     |
+-------------------------------------+
| GRANT USAGE ON *.* TO `u2`@`%`      |
| GRANT SELECT ON `db1`.* TO `u2`@`%` |
| GRANT SELECT ON `db2`.* TO `u2`@`%` |
| GRANT `u1`@`%`,`r1`@`%` TO `u2`@`%` |
+-------------------------------------+
mysql> SHOW GRANTS FOR 'r2' USING 'u1', 'r1';
+-------------------------------------+
| Grants for r2@%                     |
+-------------------------------------+
| GRANT USAGE ON *.* TO `r2`@`%`      |
| GRANT SELECT ON `db1`.* TO `r2`@`%` |
| GRANT SELECT ON `db2`.* TO `r2`@`%` |
| GRANT `u1`@`%`,`r1`@`%` TO `r2`@`%` |
+-------------------------------------+
```
当然了，以上讨论是简单的使用，真实情况下也是有意义的，例如：之前有个用户账户，具有一堆权限：  
```sql
CREATE USER 'old_app_dev'@'localhost' IDENTIFIED BY 'old_app_devpass';
GRANT ALL ON old_app.* TO 'old_app_dev'@'localhost';
```
现在这个用户离职了，那么有两种方法解决这个问题：  
 - 1-不使用角色：更改帐户密码，以便原始开发人员无法使用它，并让新开发人员使用该帐户：  
```sql
ALTER USER 'old_app_dev'@'localhost' IDENTIFIED BY 'new_password';
```
 - 2-锁定帐户以防止任何人使用它来连接到服务器,然后将账户作为角色使用：  
```sql
-- 锁定老帐户
ALTER USER 'old_app_dev'@'localhost' ACCOUNT LOCK;
-- 创建新账户并分配权限
CREATE USER 'new_app_dev1'@'localhost' IDENTIFIED BY 'new_password';
GRANT 'old_app_dev'@'localhost' TO 'new_app_dev1'@'localhost';
```

## 3.5 预留的用户账户
mysql安装的一部分是数据目录初始化，在这期间，mysql会创建保留账户：  
 - 'root'@'localhost：用于管理目的。此帐户具有所有权限，可以执行任何操作。  
 - 'mysql.sys'@'localhost'：用作sys模式对象的DEFINER。使用mysql.sys帐户可以避免DBA重命名或删除root帐户时出现的问题。此帐户已锁定，因此无法用于客户端连接。  
 - 'mysql.session'@'localhost'：由插件在内部使用以访问服务器。此帐户已锁定，因此无法用于客户端连接。  
 - 'mysql.infoschema'@'localhost'：用作INFORMATION_SCHEMA视图的DEFINER。使用mysql.infoschema帐户可以避免DBA重命名或删除root帐户时出现的问题。此帐户已锁定，因此无法用于客户端连接。  

## 3.6 设置帐户资源限制
