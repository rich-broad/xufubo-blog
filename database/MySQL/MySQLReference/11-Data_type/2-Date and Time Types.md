# 日期和时间类型
mysql支持的日期和时间类型有： DATE， TIME， DATETIME， TIMESTAMP，和 YEAR。每个类型都有一系列的有效值。   

日期和时间类型包括：DATE, TIME, DATETIME, TIMESTAMP, YEAR。每种类型都有一系列有效的值。当指定一个mysql不能代表的日期时间值时，可以使用0值。TIMESTAMP有特殊的自动更新功能。使用日期和时间类型时，请牢记这些一般的注意事项：   
 - 


各种类型的0值如下：  
|数据类型|0值|  
|-|-|  
|DATE|'0000-00-00'|  
|TIME|'00:00:00'|  
|DATETIME|'0000-00-00 00:00:00'|  
|TIMESTAMP|'0000-00-00 00:00:00'|  
|YEAR|0000|  

