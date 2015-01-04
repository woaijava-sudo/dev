#include <getopt.h>
#include "vmm.h"

/////////////////////////////////////////////////////////////

#define BIT_TEST(n,a)  (((n)&(1<<(a)))!=0)
#define BIT_SET1(n,a)  (n|=(1<<(a)))
#define BIT_SET0(n,a)  (n&=~(1<<(a)))

#define PAGE_FAULT_ACTION_FIFO          0
#define PAGE_FAULT_ACTION_LRU           1

/* 作业总数 */
UINT JobCount = JOB_COUNT;
/* 缺页中断处理方式 */
UINT PageFaultActionType = PAGE_FAULT_ACTION_FIFO;
/* 作业请求次数 */
UINT JobRequestCount = JOB_REQUEST_COUNT;
/* 是否休眠 */
bool JobSleep = true;
/* 是否输出请求细节 */
bool JobDetail = true;
/* 正在运行的作业总数 */
UINT RunningJobCount = 0;
/* 运行完毕的作业总数 */
UINT DieJobCount = 0;
/* 物理页面数 */
UINT PageCount = 0;
/* 空闲页面数 */
UINT FreePageCount = 0;
/* 物理内存空间 */
PhysicalMemory* Memory = NULL;
/* 内存位图 */
BYTE* MemoryBitmap = NULL;
/* 内存位图指针 */
UINT MemoryBitmapPtr = 0;
/* 内存位图大小 */
UINT MemoryBitmapSize = 0;
/* 作业 */
Job* Jobs = NULL;
/* 访存请求缓冲区 */
MemoryAccessRequest* RequestBuffer = NULL;
/* 作业等待队列 */
std::queue<UINT> JobQueue;
/* 查找判定树 */
tree_node* SearchNode;
/* 随机数阈值 */
double MaxRandValue;

/* 互斥体 */
pthread_mutex_t spec_mutex;
/* 条件变量 */
pthread_cond_t spec_cond;
/* 作业信号量 */
sem_t spec_sem;
/* 输出自旋锁 */
pthread_spinlock_t spec_spinlock;

/////////////////////////////////////////////////////////////

/* 报错 */
bool error(const char* name, const char* varible, UINT max_range, UINT n)
{
    if (n >= max_range)
    {
        pthread_spin_lock(&spec_spinlock);
        std::cerr << "Error" << ':' << ' ' << name << ' ' << "invalid argument" << std::endl;
        std::cerr << '\t' << varible << ' ' << '=' << ' ' << n << std::endl;
        std::cerr << '\t' << "range" << ' ' << ':' << ' ' << 0 << ' ' << '~' << ' ' << max_range << std::endl;
        pthread_spin_unlock(&spec_spinlock);
        return true;
    }
    return false;
}

/* 二进制测试 */
bool bit_test(UINT n)
{
    if (error("bit_test", "n", PageCount, n))
        return false;
    UINT p = n / 8;
    UINT q = n % 8;
    BYTE b = MemoryBitmap[p];
    return BIT_TEST(b, q);
}

/* 二进制找空闲位 */
bool bit_get_free_and_set(UINT* n)
{
    if (n == NULL)
        return false;
    UINT cnt = 0;
    bool test = false;
    while (true)
    {
        MemoryBitmapPtr %= PageCount;
        test = bit_test(MemoryBitmapPtr);
        if (test)
        {
            if (cnt < PHYSICAL_PAGE_COUNT)
            {
                cnt++;
                MemoryBitmapPtr++;
            }
            else
            {
                break;
            }
        }
        else
        {
            *n = MemoryBitmapPtr;
            if (!bit_set(MemoryBitmapPtr, true))
                return false;
            MemoryBitmapPtr++;
            break;
        }
    }

    return !test;
}

/* 二进制置位 */
bool bit_set(UINT n, bool b)
{
    if (error("bit_test", "n", PageCount, n))
        return false;
    if (!(b ^ bit_test(n)))
        return false;
    UINT p = n / 8;
    UINT q = n % 8;
    if (b)
    {
        FreePageCount--;
        BIT_SET1(MemoryBitmap[p], q);
    }
    else
    {
        FreePageCount++;
        BIT_SET0(MemoryBitmap[p], q);
    }
    return true;
}

/* 二进制初始化 */
void bit_init()
{
    memset(MemoryBitmap, 0, MemoryBitmapSize);
    MemoryBitmapPtr = 0;
}

/////////////////////////////////////////////////////////////

#define TREE_NODE_UNUSED    0
#define TREE_NODE_VALUE     1
#define TREE_NODE_POINTER   2

#define TREE_NOT_FOUND		-1

void* tree_init_recursive( TreeKeyType* s, int count, int start, unsigned char* type )
{
    tree_node* node;

    if (count == 0)
    {
        if (type)
            *type = TREE_NODE_VALUE;
        return (void*)start;
    }

    node = (tree_node*)malloc(sizeof(tree_node));
    if (type)
        *type = TREE_NODE_POINTER;

    switch (count)
    {
    case 1:
    {
        node->type[0] = TREE_NODE_VALUE;
        node->type[1] = TREE_NODE_VALUE;
        node->type[2] = TREE_NODE_UNUSED;
        node->key[0] = s[0];
        node->value[0] = (void*)(start);
        node->value[1] = (void*)(start + 1);
    }
    break;
    case 2:
    {
        node->type[0] = TREE_NODE_VALUE;
        node->type[1] = TREE_NODE_VALUE;
        node->type[2] = TREE_NODE_VALUE;
        node->key[0] = s[0];
        node->key[1] = s[1];
        node->value[0] = (void*)(start);
        node->value[1] = (void*)(start + 1);
        node->value[2] = (void*)(start + 2);
    }
    break;
    default:
    {
        int a = count / 3;
        switch (count % 3)
        {
        case 0:
        {
            node->key[0] = s[a - 1];
            node->key[1] = s[a * 2];
            node->value[0] = tree_init_recursive(&s[0], a - 1, start, &node->type[0]);
            node->value[1] = tree_init_recursive(&s[a], a, start + a, &node->type[1]);
            node->value[2] = tree_init_recursive(&s[a * 2 + 1], a - 1, start + a * 2 + 1, &node->type[2]);
        }
        break;
        case 1:
        {
            node->key[0] = s[a - 1];
            node->key[1] = s[a * 2 + 1];
            node->value[0] = tree_init_recursive(&s[0], a - 1, start, &node->type[0]);
            node->value[1] = tree_init_recursive(&s[a], a + 1, start + a, &node->type[1]);
            node->value[2] = tree_init_recursive(&s[a * 2 + 2], a - 1, start + a * 2 + 2, &node->type[2]);
        }
        break;
        case 2:
        {
            node->key[0] = s[a];
            node->key[1] = s[a * 2 + 1];
            node->value[0] = tree_init_recursive(&s[0], a, start, &node->type[0]);
            node->value[1] = tree_init_recursive(&s[a + 1], a, start + a + 1, &node->type[1]);
            node->value[2] = tree_init_recursive(&s[a * 2 + 2], a, start + a * 2 + 2, &node->type[2]);
        }
        break;
        }
    }
    }

    return (void*)node;
}

tree_node* tree_init( TreeKeyType* s, int count, int start )
{
    return (tree_node*)tree_init_recursive(s, count, start, NULL);
}

int tree_find_recursive(tree_node* node, TreeKeyType s)
{
    if (s < node->key[0])
    {
        if (node->type[0] == TREE_NODE_VALUE)
            return (int)node->value[0];
        else if (node->type[0] == TREE_NODE_POINTER)
            return (int)tree_find_recursive((tree_node*)node->value[0], s);
        else
            return TREE_NOT_FOUND;
    }
    if (node->type[2] == TREE_NODE_UNUSED || s < node->key[1])
    {
        if (node->type[1] == TREE_NODE_VALUE)
            return (int)node->value[1];
        else if (node->type[1] == TREE_NODE_POINTER)
            return (int)tree_find_recursive((tree_node*)node->value[1], s);
        else
            return TREE_NOT_FOUND;
    }
    {
        if (node->type[2] == TREE_NODE_VALUE)
            return (int)node->value[2];
        else if (node->type[2] == TREE_NODE_POINTER)
            return (int)tree_find_recursive((tree_node*)node->value[2], s);
        else
            return TREE_NOT_FOUND;
    }
}

int tree_find(tree_node* node, TreeKeyType s)
{
    if (!node) return TREE_NOT_FOUND;
    return tree_find_recursive(node, s);
}

int tree_size_recursive(tree_node* node)
{
    int size = 1;
    if (node->type[0] == TREE_NODE_POINTER)
        size += tree_size_recursive((tree_node*)node->value[0]);
    if (node->type[1] == TREE_NODE_POINTER)
        size += tree_size_recursive((tree_node*)node->value[1]);
    if (node->type[2] == TREE_NODE_POINTER)
        size += tree_size_recursive((tree_node*)node->value[2]);
    return size;
}

int tree_size(tree_node* node)
{
    if (!node) return TREE_NOT_FOUND;
    return tree_size_recursive(node);
}

void tree_destroy_recursive( tree_node* node )
{
    if (node->type[0] == TREE_NODE_POINTER)
        tree_destroy((tree_node*)node->value[0]);
    if (node->type[1] == TREE_NODE_POINTER)
        tree_destroy((tree_node*)node->value[1]);
    if (node->type[2] == TREE_NODE_POINTER)
        tree_destroy((tree_node*)node->value[2]);
    free(node);
}

void tree_destroy( tree_node* node )
{
    if (!node) return;
    tree_destroy_recursive(node);
}

/////////////////////////////////////////////////////////////

/* 发出访存请求 */
bool SendRequest(UINT n, UINT addr)
{
    if (error("SendRequest", "n", JobCount, n))
        return false;

    Job* job = GetJob(n);
    MemoryAccessRequest* request = GetRequest(n);

    /* 写入访存请求 */
    pthread_spin_lock(&request->Spin);
    request->JobId = n;
    request->VirtualAddress = addr;
    pthread_spin_unlock(&request->Spin);

    /* 通知处理 */
    sem_post(&spec_sem);

    /* 等待处理 */
    sem_wait(&job->Sem);
    return true;
}

/* 载入页面 */
bool LoadPage(Job* job, UINT vId, UINT pId)
{
    std::fstream* file = job->VirtualMemoryFile;
    file->seekg(vId * DEFAULT_PAGE_SIZE, std::ios::beg);
    file->read((char*)GetPage(job->Pages[pId]), DEFAULT_PAGE_SIZE);
    return true;
}

/* 查询空闲页面 */
UINT FreePage(PageTableItem* table, TCBItem* item)
{
    UINT i;
    BYTE f[JOB_PAGE_COUNT] = { 0 };
    for (i = 0; i < JOB_PAGE_COUNT; i++)
    {
        if (BIT_TEST(item[i].Tag, 0))
        {
            f[table[item[i].VirtaulPageId].PhysicalPageId] = 1;
        }
    }

    for (i = 0; i < JOB_PAGE_COUNT; i++)
    {
        if (f[i] == 0)
        {
            return i;
        }
    }

    error("FreePage", "item", 0, 0);

    return UINT_MAX;
}


/* FIFO页面替换策略 */
bool FIFO(Job* job, UINT page, UINT offset, bool hit, UINT tcbid)
{
    if (hit)
        return true;

    TCBItem* tcb = (TCBItem*)(job->Tcb.Items);
    PageTableItem* table = ((PageTable*)GetPage(job->Table))->Items;

    TCBItem item = tcb[0];
    memcpy(&tcb[0], &tcb[1], (JOB_PAGE_COUNT - 1) * sizeof(TCBItem));

    // 淘汰的页是否有效
    if (BIT_TEST(item.Tag, 0))
    {
        BIT_SET0(table[item.VirtaulPageId].Tag1, 0);
        BIT_SET1(table[page].Tag1, 0);
        table[page].PhysicalPageId = table[item.VirtaulPageId].PhysicalPageId;
    }
    else
    {
        table[page].PhysicalPageId = FreePage(table, tcb);
    }

    BIT_SET1(tcb[JOB_PAGE_COUNT - 1].Tag, 0);
    tcb[JOB_PAGE_COUNT - 1].VirtaulPageId = page;

    if (JobDetail)
    {
        pthread_spin_lock(&spec_spinlock);
        std::cout << '\t' << "Allocated page" << ' ' << '#' << (UINT)table[page].PhysicalPageId << std::endl;
        pthread_spin_unlock(&spec_spinlock);
    }

    return LoadPage(job, table[page].VirtaulPageId, table[page].PhysicalPageId);
}

/* LRU页面替换策略 */
bool LRU(Job* job, UINT page, UINT offset, bool hit, UINT tcbid)
{
    TCBItem* tcb = (TCBItem*)(job->Tcb.Items);
    PageTableItem* table = ((PageTable*)GetPage(job->Table))->Items;

    // 命中处理
    if (hit)
    {
        TCBItem item = tcb[tcbid];
        memcpy(&tcb[tcbid], &tcb[tcbid + 1], (JOB_PAGE_COUNT - 1 - tcbid) * sizeof(TCBItem));
        tcb[JOB_PAGE_COUNT - 1] = item;
        return true;
    }

    TCBItem item = tcb[0];
    memcpy(&tcb[0], &tcb[1], (JOB_PAGE_COUNT - 1) * sizeof(TCBItem));

    // 淘汰的页是否有效
    if (BIT_TEST(item.Tag, 0))
    {
        BIT_SET0(table[item.VirtaulPageId].Tag1, 0);
        BIT_SET1(table[page].Tag1, 0);
        table[page].PhysicalPageId = table[item.VirtaulPageId].PhysicalPageId;
    }
    else
    {
        table[page].PhysicalPageId = FreePage(table, tcb);
    }

    BIT_SET1(tcb[JOB_PAGE_COUNT - 1].Tag, 0);
    tcb[JOB_PAGE_COUNT - 1].VirtaulPageId = page;

    if (JobDetail)
    {
        pthread_spin_lock(&spec_spinlock);
        std::cout << '\t' << "Allocated page" << ' ' << '#' << (UINT)table[page].PhysicalPageId << std::endl;
        pthread_spin_unlock(&spec_spinlock);
    }

    return LoadPage(job, table[page].VirtaulPageId, table[page].PhysicalPageId);
}

/* 处理请求 */
bool HandleRequest(Job* job, UINT page, UINT offset)
{
    if (error("LoadPage", "page", PageCount, page))
        return false;
    std::fstream* f = job->VirtualMemoryFile;
    if (f == NULL)
        return false;

    if (JobDetail)
    {
        pthread_spin_lock(&spec_spinlock);
        std::cout << '\t' << "Page" << ' ' << '#' << page << std::endl;
        std::cout << '\t' << "Offset" << ':' << ' ' << offset << std::endl;
        pthread_spin_unlock(&spec_spinlock);
    }

    // 查询是否命中
    bool hit = false;
    UINT i;
    TCBItem* tcb = (TCBItem*)(job->Tcb.Items);
    for (i = 0; i < JOB_PAGE_COUNT; i++)
    {
        if (tcb[i].VirtaulPageId == page && BIT_TEST(tcb[i].Tag, 0))
        {
            hit = true;
            break;
        }
    }

    if (JobDetail)
    {
        pthread_spin_lock(&spec_spinlock);
        if (hit)
        {
            std::cout << '\t' << "---" << ' ' << "HIT" << ' ' << "---" << std::endl;
        }
        else
        {
            std::cout << '\t' << "---" << ' ' << "PAGE FAULT" << ' ' << "---" << std::endl;
        }
        pthread_spin_unlock(&spec_spinlock);
    }

    if (!hit)
    {
        job->PageFaultCount++;
    }

    switch (PageFaultActionType)
    {
    case PAGE_FAULT_ACTION_FIFO:
        return FIFO(job, page, offset, hit, i);
    case PAGE_FAULT_ACTION_LRU:
        return LRU(job, page, offset, hit, i);
    }

    return false;
}

/* 获取作业信息 */
Job* GetJob(UINT n)
{
    if (error("GetJob", "n", JobCount, n))
        return NULL;
    return &Jobs[n];
}

/* 获取页面基址 */
Page* GetPage(UINT n)
{
    if (error("GetPage", "n", PageCount, n))
        return NULL;
    return &Memory->Pages[n];
}

/* 获取访存请求 */
MemoryAccessRequest* GetRequest(UINT n)
{
    if (error("GetRequest", "n", JobCount, n))
        return NULL;
    return &RequestBuffer[n];
}

/* 初始化 */
void Init()
{
    /* 初始化 */
    std::cout << "Initializing..." << std::endl;

    PageCount = MEMORY_SIZE / DEFAULT_PAGE_SIZE;
    FreePageCount = PageCount;

    /* 内存 */
    {
        Memory = (PhysicalMemory*)malloc(MEMORY_SIZE);
        show_alloc_info("Memory", MEMORY_SIZE, (void*)Memory);
        memset(Memory, 0, MEMORY_SIZE);
    }

    /* 内存位图 */
    {
        MemoryBitmapSize = PHYSICAL_PAGE_COUNT / 8;
        MemoryBitmap = (BYTE*)malloc(MemoryBitmapSize);
        show_alloc_info("Memory bitmap", MemoryBitmapSize, (void*)MemoryBitmap);
        bit_init();
    }

    /* 作业 */
    {
        Jobs = (Job*)malloc(JobCount * sizeof(Job));
        show_alloc_info("Jobs", JobCount * sizeof(Job), (void*)Jobs);
        memset(Jobs, 0, JobCount * sizeof(Job));
    }

    /* 访存请求缓冲区 */
    {
        RequestBuffer = (MemoryAccessRequest*)malloc(JobCount * sizeof(MemoryAccessRequest));
        show_alloc_info("Memory access request buffer", JobCount * sizeof(MemoryAccessRequest), (void*)RequestBuffer);
        memset(RequestBuffer, -1, JobCount * sizeof(MemoryAccessRequest));
        for (UINT i = 0; i < JobCount; i++)
        {
            pthread_spin_init(&RequestBuffer[i].Spin, PTHREAD_PROCESS_PRIVATE);
        }
    }

    /* 查找判定树 */
    {
        double* sum = (double*)malloc(PageCount * sizeof(double));
        sum[0] = 1;
        for (UINT i = 1; i < PageCount; i++)
        {
            sum[i] = sum[i - 1] + (double)(1 / sqrt(i + 1));
        }
        MaxRandValue = sum[PageCount - 1] - 0.00001;
        SearchNode = tree_init(sum, PageCount, 0);
        show_alloc_info("Search node", tree_size(SearchNode) * sizeof(SearchNode), (void*)SearchNode);
        free(sum);
    }

    pthread_mutex_init(&spec_mutex, NULL);
    pthread_cond_init(&spec_cond, NULL);
    sem_init(&spec_sem, 0, 0);
    pthread_spin_init(&spec_spinlock, PTHREAD_PROCESS_PRIVATE);

    /* 初始化完成 */
    std::cout << "Initialized successfully" << std::endl;
}

/* 清理 */
void Clean()
{
    /* 清理 */
    std::cout << "Cleaning..." << std::endl;

    /* 内存 */
    {
        free(Memory);
        std::cout << "Memory" << ' ' << "destroyed" << std::endl;
    }

    /* 内存位图 */
    {
        MemoryBitmapSize = 0;
        free(MemoryBitmap);
        std::cout << "Memory bitmap" << ' ' << "destroyed" << std::endl;
    }

    /* 作业 */
    {
        free(Jobs);
        std::cout << "Jobs" << ' ' << "destroyed" << std::endl;
    }

    /* 访存请求缓冲区 */
    {
        for (UINT i = 0; i < JobCount; i++)
        {
            pthread_spin_destroy(&RequestBuffer[i].Spin);
        }
        free(RequestBuffer);
        std::cout << "Request buffer" << ' ' << "destroyed" << std::endl;
    }

    /* 查找判定树 */
    {
        tree_destroy(SearchNode);
        std::cout << "Search tree" << ' ' << "destroyed" << std::endl;
    }

    pthread_mutex_destroy(&spec_mutex);
    pthread_cond_destroy(&spec_cond);
    sem_destroy(&spec_sem);
    pthread_spin_destroy(&spec_spinlock);

    /* 清理完成 */
    std::cout << "Cleaned successfully" << std::endl;
}

/* 运行 */
void Run()
{
    /* 运行 */
    std::cout << "Running..." << std::endl;
    std::cout << "Main thread" << ' ' << pthread_self() << std::endl;

    pthread_t worker;
    pthread_create(&worker, NULL, HandleRequestCallback, NULL);
    pthread_join(worker, NULL);
}

/* 作业回调 */
void* JobCallback(void* cnt)
{
    UINT id = (UINT)cnt;
    Job* job = GetJob(id);
    bool openfile = false;

    /* 等待信号 */
    pthread_mutex_lock(&spec_mutex);
    pthread_cond_wait(&spec_cond, &spec_mutex);
    pthread_mutex_unlock(&spec_mutex);

    pthread_spin_lock(&spec_spinlock);
    std::cout << "Job" << ' ' << '#' << id << ' ' << "started" << std::endl;
    std::cout << '\t' << "tid" << ' ' << pthread_self() << std::endl;
    pthread_spin_unlock(&spec_spinlock);

    /* 作业初始化 */
    {
        pthread_mutex_lock(&spec_mutex);
        job->Stat = READY;
        RunningJobCount++;
        sem_init(&job->Sem, 0, 0);
        pthread_mutex_unlock(&spec_mutex);
    }

    /** 竞取运行资格
     *
     * 自旋锁适用于CPU密集型（控制台输出流）
     * 管程适用于IO密集型（本例中页面文件调入调出，作业运行周期长）
     * 因此这里采用管理思想，用PV操作实现
     * 每个作业都有自己的状态，以及用于阻塞自己的信号量
     * 故都是异步唤醒
     */
    while (true)
    {
        pthread_mutex_lock(&spec_mutex);

        if (FreePageCount >= JOB_PAGE_COUNT + 1)
        {
            pthread_spin_lock(&spec_spinlock);
            std::cout << "Job" << ' ' << '#' << id << ' ' << "acquired lock" << std::endl;
            std::cout << "Current free page count" << ' ' << ':' << ' ' << FreePageCount << std::endl;
            pthread_spin_unlock(&spec_spinlock);

            job->Stat = RUNNING;

            pthread_spin_lock(&spec_spinlock);
            std::cout << '\t' << "allocating pages..." << std::endl;
            pthread_spin_unlock(&spec_spinlock);

            bool test = true;
            UINT n = 0;

            for (UINT i = 0; i <= JOB_PAGE_COUNT && test; i++)
            {
                test = bit_get_free_and_set(&n);
                if (test)
                {
                    pthread_spin_lock(&spec_spinlock);
                    if (i == 0)
                    {
                        job->Table = n;
                        std::cout << '\t' << "page table" << ' ' << ':' << ' ' << n << std::endl;
                    }
                    else
                    {
                        job->Pages[i - 1] = n;
                        std::cout << '\t' << "page" << ' ' << '#' << (i - 1) << ' ' << ':' << ' ' << n << std::endl;
                    }
                    pthread_spin_unlock(&spec_spinlock);
                }
            }

            {
                pthread_spin_lock(&spec_spinlock);
                std::cout << "Page allocation:" << std::endl;
                for (UINT i = 0; i < PageCount; i++)
                {
                    if (i % 8 == 0)
                        std::cout << '\t';
                    std::cout << bit_test(i) << ' ';
                    if (i % 8 == 7)
                        std::cout << std::endl;
                }
                pthread_spin_unlock(&spec_spinlock);
            }
        }

        pthread_mutex_unlock(&spec_mutex);

        if (job->Stat == RUNNING)
        {
            break;
        }

        pthread_spin_lock(&spec_spinlock);
        std::cout << "Job" << ' ' << '#' << id << ' ' << "waiting..." << std::endl;
        JobQueue.push(id);
        pthread_spin_unlock(&spec_spinlock);

        /* 等待信号 */
        sem_wait(&job->Sem);

        pthread_spin_lock(&spec_spinlock);
        std::cout << "Job" << ' ' << '#' << id << ' ' << "received signal" << std::endl;
        pthread_spin_unlock(&spec_spinlock);
    }

    time_t StartTime = time(NULL);

    /* 创建虚拟内存文件 */
    std::fstream file;
    {
        pthread_spin_lock(&spec_spinlock);
        std::cout << "Job" << ' ' << '#' << id << ' ' << "creating mapping file..." << std::endl;
        pthread_spin_unlock(&spec_spinlock);

        std::stringstream ss;
        ss << "vmm_pagefile_" << id;
        file.open(ss.str().c_str(), std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
        ss.clear();
        ss.str("");
        if (file)
        {
            openfile = true;
            job->VirtualMemoryFile = &file;

            char* buffer = new char[DEFAULT_PAGE_SIZE];
            for (UINT i = 0; i < PageCount; i++)
            {
                memset(buffer, 0, DEFAULT_PAGE_SIZE);
                ss << '<' << id << ',' << i << '>';
                sprintf(buffer, "%s", ss.str().c_str());
                ss.clear();
                ss.str("");
                file.write(buffer, DEFAULT_PAGE_SIZE);
            }
            delete[] buffer;
        }
        file.flush();
    }

    /* 发出访存请求 */
    {
        pthread_spin_lock(&spec_spinlock);
        std::cout << "Job" << ' ' << '#' << id << ' ' << "performing memory access requests..." << std::endl;
        pthread_spin_unlock(&spec_spinlock);
        srand((UINT)time(NULL) + id);
        for (UINT i = 0; i < JobRequestCount; i++)
        {
            int PAGE_NO = tree_find(SearchNode, (double)rand() / RAND_MAX * MaxRandValue);
            SendRequest(id, PAGE_NO * 256 + rand() % 256);
            if (JobSleep)
            {
                usleep(rand() % 1000000);
            }
            if (!JobDetail && (i % 100) == 0)
            {
                pthread_spin_lock(&spec_spinlock);
                std::cout << "Job" << ' ' << '#' << id << ' ' << "requests" << ' ' << i << std::endl;
                pthread_spin_unlock(&spec_spinlock);
            }
        }
    }

    /* 退出 */
    {
        pthread_mutex_lock(&spec_mutex);
        if (!openfile)
        {
            pthread_spin_lock(&spec_spinlock);
            std::cout << "Job" << ' ' << '#' << id << ' ' << "open file failed" << std::endl;
            pthread_spin_unlock(&spec_spinlock);
        }

        pthread_spin_lock(&spec_spinlock);
        std::cout << "Job" << ' ' << '#' << id << ' ' << "exit" << std::endl;
        pthread_spin_unlock(&spec_spinlock);

        job->RunningTime = time(NULL) - StartTime;
        bit_set(job->Table, false);
        job->Table = 0;
        for (UINT i = 0; i < JOB_PAGE_COUNT; i++)
        {
            bit_set(job->Pages[i], false);
            job->Pages[i] = 0;
        }

        file.close();
        RunningJobCount--;
        DieJobCount++;
        job->Stat = DIE;
        job->VirtualMemoryFile = NULL;
        sem_destroy(&job->Sem);

        pthread_spin_lock(&spec_spinlock);
        if (!JobQueue.empty())
        {
            UINT other_job = JobQueue.front();
            std::cout << "Job" << ' ' << '#' << other_job << ' ' << "notified" << std::endl;
            sem_post(&GetJob(other_job)->Sem);
            JobQueue.pop();
        }
        pthread_spin_unlock(&spec_spinlock);

        pthread_mutex_unlock(&spec_mutex);
    }

    return 0;
}

/* 请求处理回调 */
void* HandleRequestCallback(void*)
{
    std::cout << "Handling thread" << ' ' << pthread_self() << std::endl;

    /* 创建作业 */
    std::cout << "Creating jobs..." << std::endl;

    {
        for (UINT i = 0; i < JobCount; i++)
        {
            pthread_create(&Jobs[i].JobId, NULL, JobCallback, (void*)i);
        }
    }

    /* 创建作业监听器 */
    pthread_t listener;
    pthread_create(&listener, NULL, JobListenerCallback, NULL);

    /* 处理内存访问 */
    pthread_spin_lock(&spec_spinlock);
    std::cout << "Dealing with memory access requests..." << std::endl;
    pthread_spin_unlock(&spec_spinlock);

    /* 通知作业开始运行 */
    pthread_cond_broadcast(&spec_cond);

    /* 创建缓冲区读指针 */
    UINT RequestBufferReadPtr = 0;

    /* 处理访存请求 */
    while (DieJobCount < JobCount)
    {
        /* 等待访存请求 */
        sem_wait(&spec_sem);

        UINT FailedCount = 0;
        bool handled = false;
        while (FailedCount < JobCount && !handled)
        {
            MemoryAccessRequest* request = GetRequest(RequestBufferReadPtr);

            pthread_spin_lock(&request->Spin);

            /* 缓冲区非空 */
            if (request->JobId != UINT_MAX)
            {
                Job* job;
                UINT page, offset;
                MemoryAccessRequest req;

                job = GetJob(request->JobId);
                req = *request;

                /* 清空缓冲区 */
                request->JobId = UINT_MAX;
                pthread_spin_unlock(&request->Spin);

                /* 输出请求信息 */
                if (JobDetail)
                {
                    pthread_spin_lock(&spec_spinlock);
                    std::cout << "Received request" << std::endl;
                    std::cout << '\t' << "Job" << ' ' << '#' << req.JobId << std::endl;
                    std::cout << '\t' << "Address" << ':' << ' ' << (void*)req.VirtualAddress << std::endl;
                    pthread_spin_unlock(&spec_spinlock);
                }

                /* 计算页面及偏移 */
                page = (req.VirtualAddress >> 8) & 0x3F;
                offset = req.VirtualAddress & 0xFF;

                /* 载入页面（页面替换） */
                HandleRequest(job, page, offset);

                /* 已处理 */
                handled = true;

                /* 通知作业 */
                sem_post(&job->Sem);
            }
            else
            {
                pthread_spin_unlock(&request->Spin);
            }

            FailedCount++;
            RequestBufferReadPtr++;
            RequestBufferReadPtr %= JobCount;
        }
    }

    /* 打印作业信息 */
    {
        time_t total_time;
        double total_pf;

        std::cout << "Job information:" << std::endl;
        std::cout << '\t' << "Job" << "\t\t" << "Exec" << "\t\t" << "PageFault" << std::endl;
        for (UINT i = 0; i < JobCount; i++)
        {
            Job* job = GetJob(i);
            std::cout << '\t' << i << "\t\t" << job->RunningTime << "\t\t" << job->PageFaultCount << std::endl;
        }

        for (UINT i = 0; i < JobCount; i++)
        {
            Job* job = GetJob(i);
            total_time += job->RunningTime;
            total_pf += (double)job->PageFaultCount;
        }
        total_time /= JobCount;
        total_pf /= JobCount;
        total_pf = total_pf * 100 / JobRequestCount;
        std::cout << "Average:" << std::endl;
        std::cout << '\t' << '-' << "\t\t" << total_time << "\t\t" << total_pf << '%' << std::endl;
    }

    /* 所有作业已完成 */
    std::cout << "All jobs exit" << std::endl;

    return 0;
}

/* 作业监听器 */
void* JobListenerCallback(void*)
{
    pthread_spin_lock(&spec_spinlock);
    std::cout << "Listening thread" << ' ' << pthread_self() << std::endl;
    pthread_spin_unlock(&spec_spinlock);

    for (UINT i = 0; i < JobCount; i++)
    {
        pthread_join(Jobs[i].JobId, NULL);
    }

    pthread_spin_lock(&spec_spinlock);
    std::cout << "All jobs died, notifying main handling thread..." << std::endl;
    pthread_spin_unlock(&spec_spinlock);

    sem_post(&spec_sem);

    return 0;
}

/////////////////////////////////////////////////////////////

void usage()
{
    std::cout << "Usage:" << std::endl;
    std::cout << '\t' << "-h --help" << '\t' << "Show help message." << std::endl;
    std::cout << '\t' << "-j --job" << '\t' << "Count of jobs. Must be larger than zero." << std::endl;
    std::cout << '\t' << "-t --type" << '\t' << "Page fault type. e.g. FIFO / LRU" << std::endl;
    std::cout << '\t' << "-r --request" << '\t' << "Job requests." << std::endl;
    std::cout << '\t' << "-q --quick" << '\t' << "No sleep time." << std::endl;
    std::cout << '\t' << "-c --clear" << '\t' << "No request display." << std::endl;
    std::cout << std::endl;
}

void show_alloc_info(const char* name, int size, void* ptr)
{
    std::cout << name << ' ' << "allocated" << std::endl;
    std::cout << '\t' << "ptr" << ':' << ' ' << (void*)ptr
              << ' ' << "size" << ':' << ' ' << size << std::endl;
}

/////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    bool input_corrent = true;
    int c;

    /* 缺页处理 */
    const char* page_fault[] =
    {
        "FIFO",
        "LRU"
    };

    /* BOOL */
    const char* bool_value[] =
    {
        "false",
        "true"
    };

    /* 开始界面 */
    std::cout << std::endl;
    std::cout << "    **********************************************" << std::endl;
    std::cout << "    *                                            *" << std::endl;
    std::cout << "    *      Virtual Memory Management System      *" << std::endl;
    std::cout << "    *                                            *" << std::endl;
    std::cout << "    **********************************************" << std::endl;
    std::cout << std::endl;

    /* 解析命令行参数 */
    const char* short_options = "hj:t:r:qc";
    const struct option long_options[] =
    {
        {  "help",      no_argument,        NULL,       'h'  },
        {  "job",       required_argument,  NULL,       'j'  },
        {  "type",      required_argument,  NULL,       't'  },
        {  "request",   required_argument,  NULL,       'r'  },
        {  "quick",     no_argument,        NULL,       'q'  },
        {  "clear",     no_argument,        NULL,       'c'  },
        {  NULL,        no_argument,        NULL,        0   }
    };

    while((c = getopt_long(argc, argv, short_options, long_options, NULL)) != -1)
    {
        switch (c)
        {
        case '?':
        case 'h':
        {
            usage();
            input_corrent = false;
        }
        break;
        case 'j':
        {
            int count = atoi(optarg);
            if (count > 0)
            {
                JobCount = (UINT)count;
                std::cout << "-j --job" << ' ' << ':' << ' ' << "valid" << std::endl;
            }
            else
            {
                std::cerr << "-j --job" << ' ' << ':' << ' ' << "invalid argument" << ' ' << optarg << std::endl;
                input_corrent = false;
            }
        }
        break;
        case 't':
        {
            std::string opt = optarg;
            std::transform(opt.begin(), opt.end(), opt.begin(), toupper);
            if (opt == "FIFO")
            {
                PageFaultActionType = PAGE_FAULT_ACTION_FIFO;
                std::cout << "-t --type" << ' ' << ':' << ' ' << "valid" << std::endl;
            }
            else if (opt == "LRU")
            {
                PageFaultActionType = PAGE_FAULT_ACTION_LRU;
                std::cout << "-t --type" << ' ' << ':' << ' ' << "valid" << std::endl;
            }
            else
            {
                std::cerr << "-t --type" << ' ' << ':' << ' ' << "invalid argument" << ' ' << optarg << std::endl;
                input_corrent = false;
            }
        }
        break;
        case 'r':
        {
            int count = atoi(optarg);
            if (count > 0)
            {
                JobRequestCount = (UINT)count;
                std::cout << "-r --request" << ' ' << ':' << ' ' << "valid" << std::endl;
            }
            else
            {
                std::cerr << "-r --request" << ' ' << ':' << ' ' << "invalid argument" << ' ' << optarg << std::endl;
                input_corrent = false;
            }
        }
        break;
        case 'q':
        {
            std::cout << "-q --quick" << ' ' << ':' << ' ' << "valid" << std::endl;
            JobSleep = false;
        }
        break;
        case 'c':
        {
            std::cout << "-c --clear" << ' ' << ':' << ' ' << "valid" << std::endl;
            JobDetail = false;
        }
        break;
        }
    }

    if (optind < argc)
    {
        std::cerr << "Invalid arguments:" << std::endl;
        while (optind < argc)
        {
            std::cerr << argv[optind++] << std::endl;
        }
        input_corrent = false;
    }

    if (!input_corrent)
    {
        return 1;
    }

    {
        std::cout << "Options:" << std::endl;
        std::cout << '\t' << "Job count" << ':' << ' ' << JobCount << std::endl;
        std::cout << '\t' << "Page fault action" << ':' << ' ' << page_fault[PageFaultActionType] << std::endl;
        std::cout << '\t' << "Job requests" << ':' << ' ' << JobRequestCount << std::endl;
        std::cout << '\t' << "Job sleep" << ':' << ' ' << bool_value[JobSleep] << std::endl;
        std::cout << '\t' << "Job detail" << ':' << ' ' << bool_value[JobDetail] << std::endl;
    }

    /* 初始化 */
    Init();

    /* 运行 */
    Run();

    /* 清理 */
    Clean();

    /* 退出 */
    std::cout << "Exit" << std::endl;

    return 0;
}
