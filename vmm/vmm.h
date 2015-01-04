#ifndef VMM_H
#define VMM_H

#include <sys/types.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <memory.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <algorithm>

#include <pthread.h>
#include <semaphore.h>

/* 页面大小（字节）*/
#define DEFAULT_PAGE_SIZE 256
/* 存储空间大小（字节） */
#define MEMORY_SIZE (16 * 1024)
/* 页面数 */
#define PHYSICAL_PAGE_COUNT 64
/* 作业所需页面数 */
#define JOB_PAGE_COUNT 9
/* 作业数目 */
#define JOB_COUNT 12
/* 作业请求次数 */
#define JOB_REQUEST_COUNT 200

/* 字节 */
typedef unsigned char BYTE;
/* 单字 */
typedef unsigned short WORD;
/* 双字 */
typedef unsigned int UINT;
/* 双字 */
typedef unsigned long DWORD;

/* 页表项 */
typedef struct
{
    /* 虚拟页号 */
	BYTE VirtaulPageId;
	/* 物理页号 */
	BYTE PhysicalPageId;
	/* 标 志 位 */
	BYTE Tag1;
	/* 标 志 位 */
	BYTE Tag2;
} PageTableItem;

/* 页表 */
typedef struct
{
    /* 页 表 项 */
    PageTableItem Items[PHYSICAL_PAGE_COUNT];
} PageTable;

/* 页面 */
typedef struct
{
    BYTE Bytes[DEFAULT_PAGE_SIZE];
} Page;

/* TCBItem */
typedef struct
{
    /* 虚拟页号 */
    BYTE VirtaulPageId;
	/* 标 志 位 */
	BYTE Tag;
} TCBItem;

/* TCB */
typedef struct
{
    TCBItem Items[JOB_PAGE_COUNT];
} TCB;

/* 物理内存 */
typedef struct
{
    /* 页    面 */
    Page Pages[PHYSICAL_PAGE_COUNT];
} PhysicalMemory;

/* 作业状态 */
typedef enum
{
    /* 新建 */
    READY,
    /* 运行 */
    RUNNING,
    /* 终止 */
    DIE
} JobStat;

/* 作业 */
typedef struct
{
    /* 作业标识 */
    pthread_t JobId;
    /* 页    表 */
    UINT Table;
    /* 页    面 */
    UINT Pages[JOB_PAGE_COUNT];
    /* 快    表 */
    TCB Tcb;
    /* 虚存文件 */
    std::fstream* VirtualMemoryFile;
    /* 信 号 量 */
    sem_t Sem;
    /* 状    态 */
    JobStat Stat;
    /* 缺 页 数 */
    UINT PageFaultCount;
    /* 开始时间 */
    time_t RunningTime;
} Job;

/* 访存请求 */
typedef struct
{
    /* 作业标识 */
    UINT JobId;
    /* 虚拟地址 */
	UINT VirtualAddress;
    /* 自 旋 锁 */
    pthread_spinlock_t Spin;
} MemoryAccessRequest;

/////////////////////////////////////////////////////////////

/* 报错 */
bool error(const char*, const char*, UINT, UINT);

/* 二进制测试 */
bool bit_test(UINT);

/* 二进制找空闲位 */
bool bit_get_free_and_set(UINT*);

/* 二进制置位 */
bool bit_set(UINT, bool);

/* 二进制初始化 */
void bit_init();

/////////////////////////////////////////////////////////////

typedef double TreeKeyType;
typedef struct
{
	unsigned char type[4];
	TreeKeyType key[2];
	void* value[3];
} tree_node;

/* 创建查找树 */
tree_node* tree_init(TreeKeyType* s, int count, int start);

/* 查找判定树 */
int tree_find(tree_node* node, TreeKeyType s);

/* 判定树大小 */
int tree_size(tree_node* node);

/* 销毁树 */
void tree_destroy(tree_node* node);

/////////////////////////////////////////////////////////////

/* 初始化 */
void Init();

/* 清理 */
void Clean();

/* 发出访存请求 */
bool SendRequest(UINT, UINT);

/* 载入页面 */
bool LoadPage(Job*, UINT, UINT);

/* 查询空闲页面 */
UINT FreePage(PageTableItem*, TCBItem*);

/* FIFO页面替换策略 */
bool FIFO(Job*, UINT, UINT, bool, UINT);

/* LRU页面替换策略 */
bool LRU(Job*, UINT, UINT, bool, UINT);

/* 处理请求 */
bool HandleRequest(Job*, UINT, UINT);

/* 获取作业信息 */
Job* GetJob(UINT);

/* 获取页面基址 */
Page* GetPage(UINT);

/* 获取访存请求 */
MemoryAccessRequest* GetRequest(UINT);

/* 运行 */
void Run();

/* 作业回调 */
void* JobCallback(void*);

/* 请求处理回调 */
void* HandleRequestCallback(void*);

/* 作业监听器 */
void* JobListenerCallback(void*);

/////////////////////////////////////////////////////////////

void usage();

void show_alloc_info(const char*, int, void*);

#endif
