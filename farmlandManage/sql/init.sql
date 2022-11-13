-- keno.sys_user definition

CREATE TABLE `sys_user` (
  `id` bigint(20) NOT NULL COMMENT 'id',
  `username` varchar(32) NOT NULL COMMENT '用户名',
  `usercnname` varchar(64) DEFAULT NULL COMMENT '中文名称',
  `password` varchar(64) NOT NULL COMMENT '密码',
  `salt` varchar(64) NOT NULL COMMENT '盐',
  `locked` varchar(1) DEFAULT NULL COMMENT '是否锁定 1 锁定 0 未锁',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


-- keno.sys_role definition

CREATE TABLE `sys_role` (
  `id` bigint(20) NOT NULL,
  `name` varchar(64) NOT NULL,
  `available` varchar(1) NOT NULL DEFAULT '0' COMMENT '是否可用,1:可用0:不可用',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


-- keno.sys_user_role definition

CREATE TABLE `sys_user_role` (
  `id` bigint(20) NOT NULL,
  `sys_user_id` bigint(20) NOT NULL,
  `sys_role_id` bigint(20) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `sys_user_role_sys_user_id_IDX` (`sys_user_id`) USING BTREE,
  KEY `sys_user_role_sys_role_id_IDX` (`sys_role_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


-- keno.sys_permission definition

CREATE TABLE `sys_permission` (
  `id` bigint(20) NOT NULL,
  `name` varchar(64) NOT NULL,
  `type` varchar(32) NOT NULL DEFAULT '0' COMMENT '资源类型：menu,button,',
  `url` varchar(256) DEFAULT NULL,
  `percode` varchar(64) DEFAULT NULL COMMENT '权限代码字符串',
  `pid` bigint(20) DEFAULT NULL,
  `sort_index` int(4) DEFAULT NULL,
  `available` char(1) DEFAULT NULL COMMENT '是否可用,1：可用，0不可用',
  PRIMARY KEY (`id`),
  KEY `sys_permission_pid_IDX` (`pid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


-- keno.sys_role_permission definition

CREATE TABLE `sys_role_permission` (
  `id` bigint(20) NOT NULL,
  `sys_role_id` bigint(20) NOT NULL,
  `sys_permission_id` bigint(20) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;