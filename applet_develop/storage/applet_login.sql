
SET names utf8;

CREATE DATABASE `db_applet_data` IF NOT EXISTS `db_applet_data` DEFAULT CHARACTER SET utf8;
/*
-- 用户信息表，该表只增加和查询，不删除，不修改
CREATE TABLE `t_user_base_data` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `open_id` varchar(64) NOT NULL DEFAULT '' COMMENT '用户在小程序中微信的openid',
  `create_time` bigint(20) NOT NULL,
  `update_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_open_id` (`contract_no`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='用户基本信息表';
*/
-- 包含自定义skey到微信open_id和sessionkey的映射,其实这块可以用k-v系统，不过我们的用户很少，因此用了数据库
CREATE TABLE `t_user_login_data` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `custom_session_key` varchar(128) NOT NULL DEFAULT '' COMMENT '自定义sessionkey',
  `open_id` varchar(64) NOT NULL DEFAULT '' COMMENT '用户在小程序中微信的openid',
  `session_key` varchar(64) NOT NULL DEFAULT '' COMMENT '微信返回的sessionkey',
  `session_key_time` bigint(20) NOT NULL DEFAULT '0' COMMENT '自定义sessionkey的创建时间',
  PRIMARY KEY (`open_id`),
  UNIQUE KEY `uk_custom_session_key` (`custom_session_key`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='用户登录信息表';






