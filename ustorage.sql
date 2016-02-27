/*
Navicat MySQL Data Transfer

Source Server         : alcp
Source Server Version : 50709
Source Host           : localhost:3306
Source Database       : ustorage

Target Server Type    : MYSQL
Target Server Version : 50709
File Encoding         : 65001

Date: 2016-02-27 01:37:55
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for itemgroups
-- ----------------------------
DROP TABLE IF EXISTS `itemgroups`;
CREATE TABLE `itemgroups` (
  `itemGroupId` int(11) NOT NULL AUTO_INCREMENT,
  `itemGroupName` varchar(64) NOT NULL,
  `itemGroupComment` varchar(64) DEFAULT NULL,
  PRIMARY KEY (`itemGroupId`),
  UNIQUE KEY `itemGroupName_UNIQUE` (`itemGroupName`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of itemgroups
-- ----------------------------
INSERT INTO `itemgroups` VALUES ('1', 'Канцелярия', null);
INSERT INTO `itemgroups` VALUES ('2', 'Мебель', null);

-- ----------------------------
-- Table structure for items
-- ----------------------------
DROP TABLE IF EXISTS `items`;
CREATE TABLE `items` (
  `itemId` int(11) NOT NULL AUTO_INCREMENT,
  `itemName` varchar(64) NOT NULL,
  `itemInventoryNum` varchar(64) NOT NULL,
  `itemGroup` int(11) NOT NULL,
  `itemComment` varchar(64) DEFAULT NULL,
  PRIMARY KEY (`itemId`),
  UNIQUE KEY `itemInventoryNum_UNIQUE` (`itemInventoryNum`),
  KEY `fk_itemGroups_idx` (`itemGroup`),
  CONSTRAINT `fk_itemGroups` FOREIGN KEY (`itemGroup`) REFERENCES `itemgroups` (`itemGroupId`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of items
-- ----------------------------
INSERT INTO `items` VALUES ('1', 'ручки', 'и123154', '1', null);
INSERT INTO `items` VALUES ('2', 'столы', 'и65422', '2', null);

-- ----------------------------
-- Table structure for itemshalls
-- ----------------------------
DROP TABLE IF EXISTS `itemshalls`;
CREATE TABLE `itemshalls` (
  `hallId` int(11) NOT NULL,
  `itemId` int(11) NOT NULL,
  `itemCount` int(10) unsigned NOT NULL DEFAULT '1',
  `recordComment` varchar(64) DEFAULT NULL,
  PRIMARY KEY (`hallId`,`itemId`),
  KEY `kf_storageItems_2_idx` (`itemId`),
  CONSTRAINT `fk_storageItems` FOREIGN KEY (`hallId`) REFERENCES `storagehalls` (`hallId`),
  CONSTRAINT `kf_storageItems_2` FOREIGN KEY (`itemId`) REFERENCES `items` (`itemId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of itemshalls
-- ----------------------------
INSERT INTO `itemshalls` VALUES ('1', '1', '10', null);
INSERT INTO `itemshalls` VALUES ('2', '2', '5', null);

-- ----------------------------
-- Table structure for storagehalls
-- ----------------------------
DROP TABLE IF EXISTS `storagehalls`;
CREATE TABLE `storagehalls` (
  `hallId` int(11) NOT NULL AUTO_INCREMENT,
  `hallName` varchar(64) NOT NULL,
  `hallAddress` varchar(64) NOT NULL,
  `hallRoomNum` int(11) NOT NULL,
  `hallComment` varchar(64) DEFAULT NULL,
  PRIMARY KEY (`hallId`),
  UNIQUE KEY `hallName_UNIQUE` (`hallName`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of storagehalls
-- ----------------------------
INSERT INTO `storagehalls` VALUES ('1', 'Склад1', 'Колледж', '115', null);
INSERT INTO `storagehalls` VALUES ('2', 'Склад2', 'Двор', '1', null);

-- ----------------------------
-- Table structure for usergroups
-- ----------------------------
DROP TABLE IF EXISTS `usergroups`;
CREATE TABLE `usergroups` (
  `userGroupId` int(11) NOT NULL AUTO_INCREMENT,
  `userGroupName` varchar(64) NOT NULL,
  `userAccess` tinyint(1) NOT NULL DEFAULT '0',
  `hallAccess` tinyint(1) NOT NULL DEFAULT '0',
  `itemsAccess` tinyint(1) NOT NULL DEFAULT '0',
  `groupComment` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`userGroupId`),
  UNIQUE KEY `userGroupName_UNIQUE` (`userGroupName`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of usergroups
-- ----------------------------

-- ----------------------------
-- Table structure for users
-- ----------------------------
DROP TABLE IF EXISTS `users`;
CREATE TABLE `users` (
  `userId` int(11) NOT NULL AUTO_INCREMENT,
  `userLogin` varchar(64) NOT NULL,
  `userPassword` varchar(64) NOT NULL,
  `userGroup` int(11) NOT NULL,
  `userName` varchar(64) NOT NULL,
  `userSurname` varchar(64) NOT NULL,
  `userFathername` varchar(64) DEFAULT NULL,
  `userComment` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`userId`),
  UNIQUE KEY `userLogin_UNIQUE` (`userLogin`),
  KEY `fk_userGroups_idx` (`userGroup`),
  CONSTRAINT `fk_userGroups` FOREIGN KEY (`userGroup`) REFERENCES `usergroups` (`userGroupId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of users
-- ----------------------------
