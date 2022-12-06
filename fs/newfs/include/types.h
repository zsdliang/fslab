#ifndef _TYPES_H_
#define _TYPES_H_

#define MAX_NAME_LEN    128     
#define NEWFS_ERROR_NONE          0
typedef int          boolean;

struct custom_options {
	const char*        device;
};
/******************************************************************************
* SECTION: macro debug
*******************************************************************************/
#define NEWFS_DBG(fmt, ...) do { printf("NEWFS_DBG: " fmt, ##__VA_ARGS__); } while(0) 
/******************************************************************************
* SECTION: Macro
*******************************************************************************/
#define TRUE                    1
#define FALSE                   0
#define UINT32_BITS             32
#define UINT8_BITS              8

#define NEWFS_MAGIC_NUM           0x12345678 
#define NEWFS_SUPER_OFS           0
#define NEWFS_ROOT_INO            0



#define NEWFS_ERROR_NONE          0
#define NEWFS_ERROR_ACCESS        EACCES
#define NEWFS_ERROR_SEEK          ESPIPE     
#define NEWFS_ERROR_ISDIR         EISDIR
#define NEWFS_ERROR_NOSPACE       ENOSPC
#define NEWFS_ERROR_EXISTS        EEXIST
#define NEWFS_ERROR_NOTFOUND      ENOENT
#define NEWFS_ERROR_UNSUPPORTED   ENXIO
#define NEWFS_ERROR_IO            EIO     /* Error Input/Output */
#define NEWFS_ERROR_INVAL         EINVAL  /* Invalid Args */

#define NEWFS_MAX_FILE_NAME       128
#define NEWFS_INODE_PER_FILE      1
#define NEWFS_DATA_PER_FILE       6
#define NEWFS_DEFAULT_PERM        0777

#define NEWFS_IOC_MAGIC           'S'
#define NEWFS_IOC_SEEK            _IO(NEWFS_IOC_MAGIC, 0)

#define NEWFS_FLAG_BUF_DIRTY      0x1
#define NEWFS_FLAG_BUF_OCCUPY     0x2
/******************************************************************************
* SECTION: Macro Function
*******************************************************************************/
#define NEWFS_IO_SZ()                     (newfs_super.sz_io)
#define NEWFS_DISK_SZ()                   (newfs_super.sz_disk)
#define NEWFS_DRIVER()                    (newfs_super.driver_fd)

#define NEWFS_ROUND_DOWN(value, round)    (value % round == 0 ? value : (value / round) * round)
#define NEWFS_ROUND_UP(value, round)      (value % round == 0 ? value : (value / round + 1) * round)

#define NEWFS_BLKS_SZ(blks)               (blks * NEWFS_IO_SZ())
#define NEWFS_ASSIGN_FNAME(psfs_dentry, _fname)\ 
                                        memcpy(psfs_dentry->fname, _fname, strlen(_fname))
#define NEWFS_INO_OFS(ino)                newfs_super.inode_offset + NEWFS_BLKS_SZ(ino)
#define NEWFS_DATA_OFS(ino)               newfs_super.data_offset + ino*NEWFS_BLKS_SZ(NEWFS_DATA_PER_FILE)

#define NEWFS_IS_DIR(pinode)              (pinode->dentry->ftype == NEWFS_DIR)
#define NEWFS_IS_REG(pinode)              (pinode->dentry->ftype == NEWFS_REG_FILE)
#define NEWFS_IS_SYM_LINK(pinode)         (pinode->dentry->ftype == NEWFS_SYM_LINK)

typedef enum file_type {
    NEWFS_FILE,
    NEWFS_DIR,
    NEWFS_REG_FILE,
    NEWFS_SYM_LINK
} NEWFS_FILE_TYPE;

struct newfs_super {
    uint32_t magic;
    int      driver_fd;
    /* TODO: Define yourself */
    int                sz_io;
    int                sz_disk;
    int                sz_usage;
    
    int                max_ino;
    uint8_t*           map_inode;
    uint8_t*           map_data;
    
    int                map_inode_blks;
    int                map_inode_offset;

    int                map_data_blks;
    int                map_data_offset;

    int                inode_blks;
    int                inode_offset;
    
    int                data_offset;

    boolean            is_mounted;

    struct newfs_dentry* root_dentry;
};

struct newfs_inode {
    uint32_t ino;
    /* TODO: Define yourself */
    int                size;                          /* 文件已占用空间 */
    char               target_path[MAX_NAME_LEN];/* store traget path when it is a symlink */
    int                dir_cnt;
    struct newfs_dentry* dentry;                        /* 指向该inode的dentry */
    struct newfs_dentry* dentrys;                       /* 所有目录项 */
    uint8_t*           data;  
};

struct newfs_dentry {
    char     fname[MAX_NAME_LEN];
    uint32_t ino;
    /* TODO: Define yourself */
    struct newfs_dentry* parent;                        /* 父亲Inode的dentry */
    struct newfs_dentry* brother;                       /* 兄弟 */
    struct newfs_inode*  inode;                         /* 指向inode */
    NEWFS_FILE_TYPE      ftype;
};

static inline struct newfs_dentry* new_dentry(char * fname, NEWFS_FILE_TYPE ftype) {
    struct newfs_dentry * dentry = (struct newfs_dentry *)malloc(sizeof(struct newfs_dentry));
    memset(dentry, 0, sizeof(struct newfs_dentry));
    NEWFS_ASSIGN_FNAME(dentry, fname);
    dentry->ftype   = ftype;
    dentry->ino     = -1;
    dentry->inode   = NULL;
    dentry->parent  = NULL;
    dentry->brother = NULL;                                            
}

/******************************************************************************
* SECTION: FS Specific Structure - Disk structure
*******************************************************************************/
struct newfs_super_d
{
    uint32_t           magic_num;
    int                sz_usage;
    
    int                max_ino;

    int                map_inode_blks;
    int                map_inode_offset;

    int                map_data_blks;
    int                map_data_offset;

    int                inode_blks;
    int                inode_offset;

    int                data_offset;
};

struct newfs_inode_d
{
    int                ino;                           /* 在inode位图中的下标 */
    int                size;                          /* 文件已占用空间 */
    char               target_path[MAX_NAME_LEN];     /* store traget path when it is a symlink */
    int                dir_cnt;
    NEWFS_FILE_TYPE      ftype;   
};  

struct newfs_dentry_d
{
    char               fname[MAX_NAME_LEN];
    NEWFS_FILE_TYPE      ftype;
    int                ino;                           /* 指向的ino号 */
};  

#endif /* _TYPES_H_ */