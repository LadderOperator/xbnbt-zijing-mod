# 编译记录

尝试编译的环境：Ubuntu 18.04 (64-bit)

## zlib

> client.cpp:28:10: fatal error: zlib.h: 没有那个文件或目录

安装zlib，`sudo apt install libghc-zlib-bindings-dev`

## mysql

> bnbt.cpp:670:9: error: ‘gpMySQL’ was not declared in this scope
> ......

1. 缺少`mysql.h`，`sudo apt-get install libmysqlclient-dev`
>2. 在`src/bnbt_mysql.h` 中添加：
>
>
>```C++
>
>#ifndef BNBT_MYSQL_H
> #define BNBT_MYSQL_H
>
>#ifndef BNBT_MYSQL
> #define BNBT_MYSQL
>#endif
>
>#ifndef XBNBT_MYSQL
> #define XBNBT_MYSQL
>#endif
>
>
>```
>
>3. 在`src/bnbt_mysql.cpp` 开头添加：
>
>```C++
>#ifndef BNBT_MYSQL
> #define BNBT_MYSQL
>#endif
>
>#ifndef XBNBT_MYSQL
> #define XBNBT_MYSQL
>#endif
>```

2. 本来以为漏了打算如上更改的，后来发现这两个宏定义是Makefile里面可控制的编译开关（捂脸）。把所有的相应参数里看情况加上`-DBNBT_MYSQL -DXBNBT_MYSQL -XBNBT_GD`（但是没有确定原先是什么逻辑，见下方的Makefile部分记录）


```Makefile
%.o: %.cpp
	$(C++) -o $@ $(CFLAGS) -DBNBT_MYSQL -DXBNBT_MYSQL -XBNBT_GD -c $<

%.mysql.o: %.cpp
	$(C++) -o $@ $(CFLAGS) -DBNBT_MYSQL -DXBNBT_MYSQL -c $<

%.gd.o: %.cpp
	$(C++) -o $@ $(CFLAGS) -DXBNBT_GD -DBNBT_MYSQL -DXBNBT_MYSQL -c $<

%.gdmysql.o: %.cpp
	$(C++) -o $@ $(CFLAGS) -DBNBT_MYSQL -DXBNBT_GD -c $<
	
%.users.o: %.cpp
	$(C++) -o $@ $(CFLAGS) -DXBNBT_MYSQL -DBNBT_MYSQL -c $<
	
%.umysql.o: %.cpp
	$(C++) -o $@ $(CFLAGS) -DBNBT_MYSQL -DXBNBT_MYSQL -c $<	
	
%.x.o: %.cpp
	$(C++) -o $@ $(CFLAGS) -DXBNBT_GD -DXBNBT_MYSQL -DBNBT_MYSQL -c $<
	
%.xmysql.o: %.cpp
	$(C++) -o $@ $(CFLAGS) -DBNBT_MYSQL -DXBNBT_GD -DXBNBT_MYSQL -c $<	
```


## gd

>tracker_file.cpp:36:10: fatal error: gd.h: 没有那个文件或目录

安装gd图形库，`sudo apt install libgd-dev`

## m_strForumLink

>tracker_signup.cpp:317:103: error: ‘m_strForumLink’ was not declared in this scope

在`src/tracker.h`中添加声明`string m_strForumLink;`

```C++
private:
	static void *threadSeedBonus( void *arg );
	static void *threadExpire( void *arg );

	string m_strAllowedDir;
	string m_strOfferDir;
	string m_strRulesDir;
	string m_strFAQDir;
	string m_strArchiveDir;
	string m_strFileDir;
	string m_strStaticHeaderFile;
	string m_strStaticHeader;
	string m_strStaticFooterFile;
	string m_strStaticFooter;
	string m_strForumLink;
```

## strInfoHash

>link.cpp:293:10: error: ‘struct announce_t’ has no member named ‘strInfoHash’

在`src/tracker.h`中`struct announce_t`内取消`string strInfoHash;`的注释

```C++
struct announce_t
{
	string strInfoHash;
	string strID;
	string strIP;
	string strEvent;
	unsigned int uiPort;
	int64 iUploaded;
	int64 iDownloaded;
	int64 iLeft;
	string strPeerID;
	string strKey;
	string strUserAgent;
	string strUID;
	string strUsername;
};
```

## strIgnore与strIgnored

>tracker.cpp:7903:32: error: ‘struct torrent_t’ has no member named ‘strIgnore’
>tracker.cpp:7914:32: error: ‘struct torrent_t’ has no member named ‘strIgnored’


在`src/tracker.h`中`struct torrent_t`内取消`string strIgnore;`和`string strIgnored;`的注释

```C++
struct torrent_t
{
	string strInfoHash;
	/*blahblah......*/
	string strOrder;
 	string strIgnore;
 	string strIgnored;
};
```

## m_pDFile

>tracker.cpp:7802:9: error: ‘m_pDFile’ was not declared in this scope

在`src/tracker.h`中添加声明`CAtomDicti *m_pDFile;`

```C++
private:
	static void *threadSeedBonus( void *arg );
	static void *threadExpire( void *arg );

	string m_strAllowedDir;
	string m_strOfferDir;
	string m_strRulesDir;
	string m_strFAQDir;
	string m_strArchiveDir;
	string m_strFileDir;
	string m_strStaticHeaderFile;
	string m_strStaticHeader;
	string m_strStaticFooterFile;
	string m_strStaticFooter;
	string m_strForumLink;
	/*blahblah......*/
	CAtomDicti *m_pDFile;

```

## ulKey

> tracker.cpp:7870:123: error: ‘ulKey’ was not declared in this scope

修改`src/tracker.h`，


```C++
	CMySQLQuery *pQueryTags = new CMySQLQuery( "SELECT bid,btag,bname FROM tags WHERE bhash=\'" + UTIL_StringToMySQL( (*ulKey).first ) + "\'" );
```

改为

```C++
	CMySQLQuery *pQueryTags = new CMySQLQuery( "SELECT bid,btag,bname FROM tags WHERE bhash=\'" + UTIL_StringToMySQL( (*it).first ) + "\'" );

## Makefile

```Makefile
./bnbt: $(OBJS) $(OBJS_BNBT)
	$(C++) -o ./bnbt $(OBJS) $(OBJS_BNBT) $(LFLAGS) -lgd
```

改为

```Makefile
./bnbt: $(OBJS) $(OBJS_BNBT)
	$(C++) -o ./bnbt $(OBJS) $(OBJS_BNBT) $(LFLAGS) -lgd -lmysqlclient 

```


## Makefile

这个文件大量文件链接问题，目前尝试修改只能编译通过前两个……
那几个开关的逻辑没有搞清楚，有空再说.jpg

目前修改了`OBJS`和`OBJS_BNBT`，并且添加了下面缺失的部分文件。

目测几个开关`-DBNBT_MYSQL -DXBNBT_MYSQL -XBNBT_GD`有其自己的控制作用。

有一说一，代码真的乱……
