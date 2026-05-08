DROP DATABASE IF EXISTS monitor_db;

CREATE DATABASE IF NOT EXISTS monitor_db;

USE monitor_db;

CREATE TABLE `admin_user` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_name` varchar(10) NOT NULL,
  `password` varchar(50) NOT NULL,
  `create_time` datetime DEFAULT CURRENT_TIMESTAMP,
  `last_login_time` datetime DEFAULT CURRENT_TIMESTAMP,
  `status` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `user_name` (`user_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE `camera_channel` (
  `id` int(11) NOT NULL,
  `channel_name` varchar(50) DEFAULT NULL,
  `camera_name` varchar(50) DEFAULT NULL,
  `is_online` int(11) NOT NULL,
  `create_time` datetime DEFAULT CURRENT_TIMESTAMP,
  `update_time` datetime DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE `exception_log` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `admin_name` varchar(10) NOT NULL,
  `channel_id` int(11) NOT NULL,
  `event_time` datetime DEFAULT NULL,
  `event_desc` varchar(255) DEFAULT NULL,
  `related_video_path` varchar(255) DEFAULT NULL,
  `create_time` datetime DEFAULT CURRENT_TIMESTAMP,
  `video_name` varchar(255) NOT NULL,
  `video_duration` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `channel_id` (`channel_id`),
  CONSTRAINT `exception_log_ibfk_1` FOREIGN KEY (`channel_id`) REFERENCES `camera_channel` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE `feature_image` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `image_name` varchar(255) DEFAULT NULL,
  `image_path` varchar(255) DEFAULT NULL,
  `channel_id` int(11) NOT NULL,
  `feature_type` int(11) DEFAULT NULL,
  `capture_time` datetime DEFAULT CURRENT_TIMESTAMP,
  `exception_id` int(11) DEFAULT NULL,
  `create_time` datetime DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `exception_id` (`exception_id`),
  KEY `channel_id` (`channel_id`),
  CONSTRAINT `feature_image_ibfk_1` FOREIGN KEY (`exception_id`) REFERENCES `exception_log` (`id`),
  CONSTRAINT `feature_image_ibfk_2` FOREIGN KEY (`channel_id`) REFERENCES `camera_channel` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE `normal_video_info` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `video_name` varchar(255) NOT NULL,
  `video_path` varchar(255) NOT NULL,
  `channel_id` int(11) NOT NULL,
  `create_time` datetime DEFAULT CURRENT_TIMESTAMP,
  `video_duration` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `video_name` (`video_name`),
  KEY `channel_id` (`channel_id`),
  CONSTRAINT `video_info_ibfk_1` FOREIGN KEY (`channel_id`) REFERENCES `camera_channel` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE `operation_log` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `admin_id` int(11) DEFAULT NULL,
  `admin_name` varchar(50) NOT NULL,
  `operation_time` datetime DEFAULT NULL,
  `operation_func` varchar(255) DEFAULT NULL,
  `operation_desc` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `admin_id` (`admin_id`),
  CONSTRAINT `operation_log_ibfk_1` FOREIGN KEY (`admin_id`) REFERENCES `admin_user` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE `sys_config` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `save_path` varchar(50) NOT NULL,
  `interval_time` int(11) NOT NULL,
  `update_time` datetime DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `channel_name_1` varchar(255) DEFAULT NULL,
  `channel_name_2` varchar(255) DEFAULT NULL,
  `channel_name_3` varchar(255) DEFAULT NULL,
  `channel_name_4` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

INSERT INTO admin_user(user_name, password, status) VALUES("admin", MD5("123456"), 1);