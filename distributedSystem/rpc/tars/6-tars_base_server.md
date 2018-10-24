# 概述
tars是一个高性能RPC框架，除了实现了RPC核心部分外，还为运营、运维、发布、监控、扩容等提供了支持，因此，除了RPC核心的实现之外，还实现了一系列的基础服务用于支撑这些功能的运行，本节主要介绍这些基础服务的功能。  

# 基础服务用到的tars协议
在 [tars install](./tars_install.md) 中已经知道：  
 - 手工部署的核心基础服务：tarsAdminRegistry, tarsregistry, tarsnode, tarsconfig, tarspatch  
 - 通过管理平台部署的普通基础服务：tarsstat, tarsproperty,tarsnotify, tarslog，tarsquerystat，tarsqueryproperty   

