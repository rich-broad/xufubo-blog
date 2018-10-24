# 2、InnoDB和ACID模型
ACID模型是非常重要的，需要深入学习和理解，这里先简单的列出与ACID相关的MySQL特性。
 - A：原子性
 - C：一致性
 - I：隔离性
 - D：持久性  
**原子性**  
原子性方面主要涉及InnoDB事务。相关的MySQL功能包括：
 - 自动提交设置。
 - COMMIT语句。
 - ROLLBACK语句。
 - 来自INFORMATION_SCHEMA 表格的操作数据  
**一致性**  
一致性主要涉及InnoDB为崩溃而设计的保护机制，相关的MySQL功能包括：  
 - InnoDB 双写缓冲区。
 - InnoDB 崩溃恢复。  
**隔离性**  
隔离性主要涉及InnoDB事务，特别是适用于每个事务的隔离级别。相关的MySQL功能包括：  
 - 自动提交的设置。
 - SET ISOLATION LEVEL语句。
 - InnoDB锁的底层细节。在性能调优期间，可以通过INFORMATION_SCHEMA表格查看这些细节。  
**持久性**  
持久性方面涉及MySQL软件功能与你的特定硬件配置进行交互。由于硬件多种多样，因此，这方面是最为复杂的，提供具体的指导方针也是最复杂的。（这些方针可能是购买“新硬件”）。相关的MySQL功能包括：
 - InnoDB 双写缓冲区，由innodb_doublewrite 配置选项打开和关闭 。
 - 配置选项 innodb_flush_log_at_trx_commit。
 - 配置选项 sync_binlog。
 - 配置选项 innodb_file_per_table。
 - 存储设备中（如磁盘驱动器，SSD或RAID阵列）的写缓冲。
 - 存储设备中的电池备份缓存。
 - 保护运行MySQL服务器和存储MySQL数据的所有计算机服务器和存储设备的不间断电源（UPS）。
 - 你的备份策略，例如备份频率和类型以及备份保留期。
 - 对于分布式或托管数据应用程序，MySQL服务器硬件所在数据中心的特定特征以及数据中心之间的网络连接。  

 
