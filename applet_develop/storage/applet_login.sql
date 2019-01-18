
SET names utf8;

CREATE DATABASE `db_applet_data` IF NOT EXISTS `db_applet_data` DEFAULT CHARACTER SET utf8;

-- 用户信息表，该表只增加和查询，不删除，不修改
CREATE TABLE `t_user_base_data` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `union_id` varchar(64) NOT NULL DEFAULT '' COMMENT '用户在小程序中微信的openid',
  `update_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  UNIQUE KEY (`uid`),
  PRIMARY KEY `uk_union_id` (`union_id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='用户基本信息表';

-- 包含自定义skey到微信open_id和sessionkey的映射,其实这块可以用k-v系统，不过我们的用户很少，因此用了数据库
CREATE TABLE `t_user_login_data` (
  `uid` int(11) NOT NULL DEFAULT '0',
  `custom_session_key` varchar(128) NOT NULL DEFAULT '' COMMENT '自定义sessionkey',
  `open_id` varchar(64) NOT NULL DEFAULT '' COMMENT '用户在小程序中微信的openid',
  `union_id` varchar(64) NOT NULL DEFAULT '' COMMENT '同一个微信开放平台下的不同应用的用户唯一表示,详见: https://developers.weixin.qq.com/miniprogram/dev/framework/open-ability/union-id.html',
  `session_key` varchar(64) NOT NULL DEFAULT '' COMMENT '微信返回的sessionkey',
  `session_key_time` bigint(20) NOT NULL DEFAULT '0' COMMENT '自定义sessionkey的创建时间',
  PRIMARY KEY (`open_id`),
  UNIQUE KEY `uk_custom_session_key` (`custom_session_key`),
  CONSTRAINT `fk_uid` foreign key(`uid`) references `t_user_base_data`(`uid`),
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='用户登录信息表';






