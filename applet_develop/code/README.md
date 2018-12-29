# 概述

本目录是小程序的后台代码，整体架构是基于腾讯开源的tars框架的微服务架构。为了编译通过，需要安装tars框架。    
目录介绍如下：  
1. AppletCommon：IDL定义、通用宏定义。   
2. AppletProxyServer：接入层服务，用于将http + json转换成tars协议。  
3. AppletAuthServer：与微信后台交互的服务。  
 

