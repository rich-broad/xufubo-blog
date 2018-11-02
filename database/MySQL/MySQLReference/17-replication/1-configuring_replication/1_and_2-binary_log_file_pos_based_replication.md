# 1 基于二进制日志文件位置的复制配置
作为主站的mysql数据库会将更新和更改作为**事件**写入二进制日志（binlog）。作为从站的mysql数据库会从主站读取二进制日志，并在从站中执行二进制日志中的事件。每个从站都会接受到二进制日志中的**全部内容**，你可以配置从站，来控制从站仅处理特定数据库或者表事件，不过你无法控制主站，主站会在二进制日志中记录全部事件。  
每个从站都会保存二进制日志坐标的数据：日志文件的名字和已经处理的日志文件的位置。这意味着可以将多个从站连接到主站并执行相同二进制日志的不同部分，从站可以断开与主站的连接，恢复后可以继续复制，而不影响主站的运行。  
主站和每个从站必须配置一个唯一的ID（server-id选项）。因为每个从站要主动去和主站建立连接，因此，每个从站必须配置有关主站名称，日志文件名以及该文件中的位置的信息（**即，从站要知道从哪里开始复制**）。这些信息可以在从站上使用CHANGE MASTER TO进行控制。  

# 2 设置基于二进制日志文件位置的复制
有多种设置方法，具体使用什么方法取决于你如何设置复制以及主站数据库中是否有数据。不过有一些设置是通用的：  
 - 确保主站配置了唯一的id，启用了二进制日志记录。  
 - 确保每个从站配置了唯一的id。  
 - 在主服务器上创建一个单独用于复制的账户，以便从站使用这个账户来连接主站。  
 - 在创建数据快照或启动复制过程之前，在主数据库上应记录二进制日志中的当前位置。在配置从站时需要此信息，以便从站知道从二进制日志内的哪个位置开始执行事件。  
 - 如果主站上已经有数据并希望使用它来同步从站，则需要创建数据快照以将数据复制到从站。正在使用的存储引擎会影响你创建快照的方式。如果使用的是myisam，则必须停止主站上的语句，或者读锁，然后获取其当前二进制日志坐标并转储其数据，然后再允许主站继续执行语句。如果不停止语句的执行，数据转储和主站状态信息将不匹配，从而导致从服务器上的数据库不一致或损坏。如果使用InnoDB则不需要读锁，一个足够长的传输数据快照的事务就可以了，更多信息见[Section 15.18, “InnoDB and MySQL Replication](https://dev.mysql.com/doc/refman/8.0/en/innodb-and-mysql-replication.html)。  
 - 用主站的相关信息配置从站，例如：主机名，登录凭证和二进制日志文件的名称和位置。  
 > - **注意：设置过程中的某些步骤需要 SUPER特权。如果您没有此权限，则可能无法启用复制。**  
如上是基于二进制日志位置进行复制的基本配置。  

## 2.1 设置主站配置
通过--log-bin选项启用二进制日志，并设置二进制日志文件的基本名称。*建议你指定此选项以为二进制日志文件指定非默认基本名称（因为默认值为主机名，当服务器主机名改变之后可能会出问题，因此要设置）*。  
通过--server-id设置服务器唯一id（范围是1~2^32 - 1）。如果在主站上设置服务器ID为0，则会拒绝从站的任何连接，如果在从站上设置服务器ID为0，则拒绝连接到主站。
 > **注意：以下选项也对主站有影响**
  - 对于InnoDB，为了最大程度（为什么不是完全，后边分析）的保持持久性和一致性，你要在主站的my.cnf中设置如下两个选项：  
  innodb_flush_log_at_trx_commit=1 和 sync_binlog=1 。
  - 确保复制主站上未启用 skip-networking选项。  

## 2.2 设置从站配置
通过--server-id设置服务器唯一id，例如：  
```
[mysqld]
server-id=2
```
如果你要设置多个从站，则每个从站都必须具有唯一的非零server-id值，该值不同于主站和任何其他从站的值。  
默认情况下，二进制日志记录在**所有**服务器上是启用的。一般而言，从站不需要启用二进制日志记录，不过启用二进制日志的从站可以构建更加复杂的复制拓扑结构。例如：  
```
A -> B -> C
```
这时，B既是主站又是从站。B从A接收的更新必须由B记录到其二进制日志中，以便传递给C。这时，B除了启用二进制日志之外，此复制拓扑还需要启用--log-slave-updates选项，使用此选项，从站不仅写入从主站接收到的更新，同时通过从机的SQL线程将更新写到到从机自己的二进制日志中。--log-slave-updates选项默认启用。  
在从站上指定--skip-log-bin 和 --skip-log-slave-updates 选项可以在从站上禁用二进制日志和从站更新日志。   

## 2.3 创建一个复制用户
在复制架构中，每一个*从站*会使用mysql用户名和密码连接到主站，因此，主站上必须有一个账户用于从站的复制。任何被授予REPLICATION SLAVE权限的账户都可以用于复制。在设置复制从站时，用户名由CHANGE MASTER TO命令中的MASTER_USER选项指定。  
虽然你不必专门为复制创建一个账户，但是，**你应该知道复制用户的用户名和密码以纯文本形式存储在主站信息存储库的mysql.slave_master_info表中**，因此，为了安全起见，你还是要创建一个仅具有复制权限的单独帐户。  
例如：  
```sql
mysql> CREATE USER 'repl'@'%.example.com' IDENTIFIED BY 'password';
mysql> GRANT REPLICATION SLAVE ON *.* TO 'repl'@'%.example.com';
```

#### 重要提示
 - 为了使通过caching_sha2_password插件进行身份验证的用户帐户连接到主站，你必须按照[Section 17.3.9 Setting Up Replication to Use Encrypted Connections](https://dev.mysql.com/doc/refman/8.0/en/replication-solutions-encrypted-connections.html)设置安全连接,或使用RSA密钥对启用未加密的连接以支持密钥交换。我在tars安装中就遇到了这个问题，不得不说，官方文档才是最为权威的。对于mysql8.0, caching_sha2_password身份验证插件是创建的新用户的默认设置。如果你创建的复制帐户（由MASTER_USER选项指定）使用此身份验证插件，并且你没有使用安全连接，那么你必须启用基于RSA密钥对的密钥交换才能成功连接。   

## 2.4 获取主站的二进制日志坐标
要将从站配置为在正确的位置启动复制过程，需要记录主站在其二进制日志中的当前坐标。此过程使用FLUSH TABLES WITH READ LOCK，它会阻止InnoDB表的COMMIT操作。  
要获取主站二进制日志坐标，请按照下列步骤进行：  
 - 1、启动客户端，连接主站，执行如下语句：
 ```sql
 FLUSH TABLES WITH READ LOCK;
 ```
 **注意：让你发出FLUSH TABLES WITH READ LOCK语句的客户端保持运行，以便读锁保持有效。如果退出客户端，则锁定被释放。**  
 - 2、启动一个新的会话，连接主站，使用SHOW MASTER STATUS语句确定当前主站的二进制日志文件名称和位置：  
 ```sql
 mysql > SHOW MASTER STATUS;
 +------------------+----------+--------------+------------------+
 | File             | Position | Binlog_Do_DB | Binlog_Ignore_DB |
 +------------------+----------+--------------+------------------+
 | mysql-bin.000003 | 73       | test         | manual,mysql     |
 +------------------+----------+--------------+------------------+
 ```
 你需要记录：mysql-bin.000003和73，一会设置从站需要他们。  
 **如果主站之前在禁用二进制日志记录的情况下运行，由SHOW MASTER STATUS或mysqldump --master-data显示的日志文件名称和位置值将为空。在这种情况下，稍后在指定从站的日志文件和位置时需要使用的值是空字符串（''）和4。**  
这时候你有了正确的二进制日志坐标。接下来的步骤取决于主站上是否已经存在数据。 
 - 如果复制之前，主站上已经存在数据，则需要先将数据同步到主站，这时需要保持刚刚的客户端运行，以***保持锁定***状态，以避免进一步的更改（见下一节）。  
 - 如果设置新的主从复制组，则可以退出第一个会话以释放读锁。  

## 2.5 选择数据快照的方法
如果主站已经存在数据，则需要先将数据复制到各个从站。有多种方法可以从主站转储数据。  
通常情况下有以下两种：  
 - 1、使用mysqldump工具来创建您想要复制的所有数据库的转储。这是推荐的方法，尤其是使用InnoDB时。  
 - 2、如果您的数据库存储在二进制可移植文件中，则可以将原始数据文件复制到从站。这可以比使用mysqldump并在每个从属设备上导入文件更高效。对于像InnoDB这样的存储引擎，这是不推荐的。  

***使用mysqldump创建数据快照***  
先使用mysqldump工具转储主站数据，在开始复制之前将该数据导入从站中。  
以下示例将所有数据库转储到名为dbdump.db的文件，并包含--master-data选项，它会自动附加从站上所需的CHANGE MASTER TO语句以启动复制过程：  
```shell
shell> mysqldump --all-databases --master-data > dbdump.db
```
 > **注意：如果不使用--master-data，则需要手动将所有表锁定在单独的会话中。如上一小节那样**  
你也可以使用mysqldump转储你需要的数据库或者表，有关更多信息，见[4.5.4 mysqldump — A Database Backup Program](https://dev.mysql.com/doc/refman/8.0/en/mysqldump.html)。  
要导入数据，请将转储文件复制到从站，或者在远程连接到从站时从主站访问该文件。  

***使用原始数据文件创建数据快照***  
该方法个人阅读了mysql8.0的官方手册，感觉不是很好，在此不再详细列出，具体的可以参考mysql的官方手册。  

通过这一小节的学习，我们会看到：最好在一开始就规划好自己的复制拓扑，而不要等着主站运行了一段时间后才开始规划，这样会很麻烦，搞不好，也容易出错。  

## 2.6 设置从站
经过前面的准备，我们已经做了如下工作：  
 - 1、为主站设置了server-id，
 - 2、为主站开启了相关的选项；
 - 3、创建好了账户；
 - 4、获取了主站数据的快照和此快照对应的二进制坐标；
这时候，需要执行如下步骤：
 - 1、在主站上释放所有表的读锁：
 ```sql
 UNLOCK TABLES;
 ```
 - 2、按照上边介绍的方法配置从站；
 - 3、接下来的步骤取决于是否需要有现成的数据导入到从站。  

1. 使用新的主站和从站设置复制 
如果没有要导入的以前数据库的快照，请将从站配置为从新主站启动复制。  
要设置主站和从站的复制，请执行以下操作：
 - 1、启动从站mysql服务器；  
 - 2、执行CHANGE MASTER TO设置主站的信息；  
 如果你需要将已dump的数据加载到新搭建的复制组中，那么你可以先配置好复制拓扑，然后通过将数据加载到新的主站中，数据会自动复制到从站。  

 ```shell
 shell> mysql -h master < fulldb.dump
 ```

2. 使用现有数据设置复制 

--------------------------------------------------------------------------------------------------------
使用现有数据设置复制时，请在开始复制之前将快照从主站传输到从站。将数据导入从站的过程取决于你获得快照的方法（见上一小节）  
**如果你使用mysqldump：**  
 - 1、使用 *--skip-slave-start* 选项启动从站，以便复制不会启动。  
 - 2、导入由mysqldump生成的转储文件：  
 ```shell
 shell> mysql < fulldb.dump
 ```
 这里我们假设mysqldump使用了--master-data生成转储文件。  

--------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------
**如果使用原始数据文件创建快照：**  
 - 1、将数据文件解压缩到从站数据目录中。例如：  
 ```shell
 shell> tar xvf dbdump.tar
 ```
 - 2、使用 *--skip-slave-start* 选项启动从站 以便复制不会启动。  
 - 3、使用主站的复制坐标配置从站（CHANGE MASTER TO语句）。  
 - 4、启动从属线程：  
 ```sql
 mysql> START SLAVE;
 ```
完成此过程后，从站连接到主站并复制自从创建快照以来在主站上发生的任何更新。如果出于任何原因无法复制，错误消息将发送到从站的错误日志中。  

--------------------------------------------------------------------------------------------------------

**从站**使用 “主站信息状态日志” 和 “中继日志的信息日志” 来跟踪它已经处理的主站二进制日志的进度（*这两份日志在主站中为空，在从从站中会记录详细的复制进度信息*）。从MySQL 8.0开始，默认情况下，这些从属状态日志的存储库是mysql数据库中名为slave_master_info和slave_relay_log_info的表。也可以设置--master-info-repository = FILE和--relay-log-info-repository = FILE，这时这些信息被存储在master.info和relay-log.info文件中，现在已被弃用，并且会在将来的版本中删除。  

除非你明确知道自己在做什么并完全理解其含义，否则不要删除或编辑这些表（或文件）。如果真的需要，也最好使用CHANGE MASTER TO语句来更改复制参数。从站使用语句中指定的值自动更新从站状态日志。更多信息见：[Section 17.2.4, “Replication Relay and Status Logs”](https://dev.mysql.com/doc/refman/8.0/en/slave-logs.html).    

**注意：主信息日志的内容会覆盖命令行或my.cnf中指定的一些服务器选项。更多信息见[Section 17.1.6, “Replication and Binary Logging Options and Variables”](https://dev.mysql.com/doc/refman/8.0/en/replication-options.html)**   

主站的一个快照足以满足多个从机的需求。要设置更多的从站，请使用相同的主站快照，并按照上述过程的从站部分进行操作。  

## 2.7 在从站上设置主站配置
要将从站设置为与主站通信以进行复制，请使用必要的连接信息配置从站。为此，请在从站上执行以下语句，将选项值替换为与系统相关的实际值：  
```sql
mysql> CHANGE MASTER TO
    ->     MASTER_HOST='master_host_name',
    ->     MASTER_USER='replication_user_name',
    ->     MASTER_PASSWORD='replication_password',
    ->     MASTER_LOG_FILE='recorded_log_file_name',
    ->     MASTER_LOG_POS=recorded_log_position;
```
**注意：注意 复制不能使用Unix套接字文件。您必须能够使用TCP /连接到主MySQL服务器**   

CHANGE MASTER TO语句也有其他选项。例如，可以使用SSL设置安全复制。有关选项的完整列表以及字符串值选项的最大允许长度的信息，请参见[Section 13.4.2.1, “CHANGE MASTER TO Syntax”](https://dev.mysql.com/doc/refman/8.0/en/change-master-to.html).   

#### 再次重要提示 
 - 如果没有使用安全连接，但是MASTER_USER选项中指定的帐户使用caching_sha2_password插件进行身份验证，这时，为了正常连接，你必须在CHANGE MASTER TO语句中指定MASTER_PUBLIC_KEY_PATH或GET_MASTER_PUBLIC_KEY选项以启用基于RSA密钥对的密钥交换。  

## 2.8 将从站添加到复制环境
 可以在不停止主站的情况下将另一个从站添加到现有复制配置中。为此，可以通过复制现有从站的数据目录来设置新从站，并为新从站提供不同的服务器ID（用户指定的）和服务器UUID（在启动时生成）。  
#### 复制现有从站的步骤如下：
1. 停止现有从站并记录从站状态信息(从站状态信息包括什么？)，特别是主站二进制日志文件和中继日志文件的位置。可以在Performance Schema复制表中查看从站状态，或通过SHOW SLAVE STATUS语句，如下：  
```sql
mysql> STOP SLAVE;
mysql> SHOW SLAVE STATUS\G
```
2. 关闭现有从站
```sql
shell> mysqladmin shutdown
```
3. 将数据目录从现有从站copy到新从站，包括日志文件和中继日志文件。您可以通过使用tar或WinZip创建存档，或使用cp或rsync等工具执行直接复制来完成此操作。  
**重要提示：**
 - > 1. copy之前，确认与现有从站相关的所有文件是否都存储在数据目录中。例如，InnoDB系统表空间，撤消表空间和重做日志可能存储在其他位置。InnoDB表空间文件和file-per-table表空间可能已在其他目录中创建。从站的二进制日志和中继日志可能位于数据目录外的其他目录中，你需要检查为现有从站设置的系统变量，并查找已指定的任何其他路径，如果找到任何内容，请同时复制这些目录（*总之就是把从站的所有字节都完整的拷贝到新的从站中*）。    
 - > 2. 在copy过程中，如果文件已经用于 “主站的信息” 和 “中继日志的信息” 存储库([Section 17.2.4, “Replication Relay and Status Logs”](https://dev.mysql.com/doc/refman/8.0/en/slave-logs.html))，也要确保将这些文件从现有从站复制到新从站，如果表已用于存储库，默认情况下，对于mysql 8.0，这些表存放在数据目录中，因此，复制数据目录就可以（还是强调了要将数据完全拷贝）。  
 - > 3. copy之后，从新从站上的数据目录副本中删除auto.cnf文件，这样新的从站就会以不同的UUID启动，服务器UUID必须是唯一的。  

添加新从站时遇到的一个常见问题是新从站失败并出现一系列警告和错误消息，如下所示：  
```sql
071118 16:44:10 [Warning] Neither --relay-log nor --relay-log-index were used; so
replication may break when this MySQL server acts as a slave and has his hostname
changed!! Please use '--relay-log=new_slave_hostname-relay-bin' to avoid this problem.
071118 16:44:10 [ERROR] Failed to open the relay log './old_slave_hostname-relay-bin.003525'
(relay_log_pos 22940879)
071118 16:44:10 [ERROR] Could not find target log during relay log initialization
071118 16:44:10 [ERROR] Failed to initialize the master info structure
```
如果未指定--relay-log选项，则会发生这种情况，因为中继日志文件包含主机名作为其文件名的一部分。如果未使用--relay-log-index选项，则中继日志索引文件也是如此。详见[Section 17.1.6, “Replication and Binary Logging Options and Variables”](https://dev.mysql.com/doc/refman/8.0/en/replication-options.html)。  

要避免此问题，请在新从站上使用与现有从站相同的--relay-log 值。如果未在现有从站上显式设置此选项，请使用 existing_slave_hostname-relay-bin（existing_slave_hostname为现有从站的主机名）。这也就是为什么在配置主站和从站时一定要配置这些信息的原因。如果无法做到这一点，请将现有从站的中继日志索引文件复制到新从站，并在新从站上将--relay-log-index选项的值设置为现有从站上该选项的值。如果未在现有从站上明确设置此选项，使用existing_slave_hostname-relay-bin.index（existing_slave_hostname为现有从站的主机名）。如果你已按照本节中的**其余步骤**尝试启动新从站并遇到前面描述的错误，请这么做：  
 - > a. 停止现有从站和新从站。  
 - > b. 将现有从站的中继日志索引文件的内容复制到新从站的中继日志索引文件中，确保覆盖该文件中已有的任何内容。  
 - > b. 继续本节中的其余步骤。  

看一下中继日志索引文件的内容和binlog索引文件的内容：  
```shell
[root@localhost data]# cat mysql-relay-bin.index 
/usr/local/mysql/data/mysql-relay-bin.000003
/usr/local/mysql/data/mysql-relay-bin.000004

[root@localhost data]# cat mysql-bin.index 
./mysql-bin.000001
./mysql-bin.000002
```

4. copy完成后，重新启动现有从站。  
5. 编辑新从站的配置，设置唯一的server-id（没有被主站和其他从站使用）。  
6. 指定--skip-slave-start选项启动新的从站以便复制暂时不要启动。通过查询Performance Schema复制表或者发出SHOW SLAVE STATUS语句来确认与现有从站相比，新从站具有正确的设置，同时要注意新从站的server-id和server-UUID正确并且唯一。  
7. 通过发出START SLAVE语句启动从站线程：  
```sql
mysql> START SLAVE;
```
完成了，新从站可以开始工作了。  

## 本节的疑问
1. 一台主站开始禁用了binlog，后来启用了binlog，这时从站该如何设置？需要先把主站的数据快照同步到从站然后开启复制，还是只需要开启主站日志，配置从站即可？（我的理解是需要先把主站的数据快照同步到从站然后开启复制，后边验证后来回答这个问题）。  
2. 如何获得主从之间延迟的详细信息。  
