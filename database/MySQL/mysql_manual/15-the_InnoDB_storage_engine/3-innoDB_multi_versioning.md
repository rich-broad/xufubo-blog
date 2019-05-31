---
title: InnoDB 多版本技术
---
# 3、InnoDB多版本
InnoDB是一个[多版本存储引擎](https://dev.mysql.com/doc/refman/8.0/en/glossary.html#glos_mvcc),通过多版本技术，可以提高数据库的并发能力。为了支持事务功能，如并发和回滚，它会保存有关已更改行的旧版本。这些信息被存储在表空间中被称为[回滚段(rollback segment)](https://dev.mysql.com/doc/refman/8.0/en/glossary.html#glos_rollback_segment)的数据结构中。InnoDB 使用回滚段中的信息执行事务回滚中所需的撤消操作。它还使用这些信息构建一个行的早期版本以保证[一致性读取(consistent read)](https://dev.mysql.com/doc/refman/8.0/en/glossary.html#glos_consistent_read)。
在内部，InnoDB为存储在数据库中的每一行添加三个字段：
 - DB_TRX_ID：6字节，表示插入或更新行的最后一个事务的事务标识符。
 - DB_ROLL_PTR：
 - DB_ROW_ID：
在回滚段中的undo logs(存储在回滚段这种数据结构中)被分为插入和更新undo logs。
