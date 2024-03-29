# 概述

本目录主要记录在开发一款微信小程序的过程中需要解决的各种问题，在此进行总结。  
开发小程序主要需要解决如下几个问题：  
 1. 准备服务器,可以在腾讯云或者阿里云购买，也可以自己买服务器，我主要使用云服务器。  
 2. 申请域名。  
 3. 网站备案，如果在腾讯云上购买，可以在腾讯云官网上完成备案，**注意：在使用腾讯云快速备案的时候要求至少购买三个月以上的云服务器**。  
 4. 微信平台上申请小程序。  
 5. 搭建开发环境，购买的云服务器上各种软件几乎什么都没有。需要自己安装（gcc，openssl，curl等基础软件，mysql，redis等）。  
 6. 微信小程序要求https，因此域名必须支持https协议。可以买腾讯云的负载均衡器实现全站https，也可以自己通过nginx作为代理来支持https。  
 7. 做完如上几件事情就选择开发框架，我这里选择腾讯的tars，tars的部署可以参考[tars安装部署](../distributed_system/rpc/tars/1-tars_install.md)。  
 8. 开发与微信交互的登录模块。  
 9. 开发业务模块。  
 10. 上线。  

 

