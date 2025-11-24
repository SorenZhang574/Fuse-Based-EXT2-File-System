#ifndef _TYPES_H_
#define _TYPES_H_

#define MAX_NAME_LEN    128    

typedef int          boolean;
typedef uint16_t     flag16;

#define TRUE                    1
#define FALSE                   0
#define UINT32_BITS             32
#define UINT8_BITS              8

#define NEWFS_MAGIC_NUM           0x52415453  
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
#define NEWFS_BLK_SZ()                    (newfs_super.sz_blk)
#define NEWFS_DISK_SZ()                   (newfs_super.sz_disk)
#define NEWFS_DRIVER()                    (newfs_super.driver_fd)

#define NEWFS_ROUND_DOWN(value, round)    ((value) % (round) == 0 ? (value) : ((value) / (round)) * (round))
#define NEWFS_ROUND_UP(value, round)      ((value) % (round) == 0 ? (value) : ((value) / (round) + 1) * (round))

#define NEWFS_BLKS_SZ(blks)               ((blks) * NEWFS_BLK_SZ())
#define NEWFS_ASSIGN_FNAME(pnewfs_dentry, _fname)\ 
                                          memcpy(pnewfs_dentry->name, _fname, strlen(_fname))
#define NEWFS_INO_OFS(ino)                (newfs_super.inode_offset + ino * NEWFS_BLK_SZ())
#define NEWFS_DATA_OFS(i)                 (newfs_super.data_offset + i * NEWFS_BLK_SZ())

#define NEWFS_IS_DIR(pinode)              (pinode->dentry->ftype == NEWFS_DIR)
#define NEWFS_IS_REG(pinode)              (pinode->dentry->ftype == NEWFS_REG_FILE)
#define NEWFS_IS_SYM_LINK(pinode)         (pinode->dentry->ftype == NEWFS_SYM_LINK)

typedef enum newfs_file_type {
    NEWFS_REG_FILE,
    NEWFS_DIR,
    NEWFS_SYM_LINK
} NEWFS_FILE_TYPE;

struct custom_options {
	const char*        device;
};

struct newfs_super {
    uint32_t magic;
    int      driver_fd;
    /* TODO: Define yourself */
    int           sz_usage;
    int           sz_io;
    int           sz_disk;
    int           sz_blk;
    
    int           max_ino;
    uint8_t*      map_inode;
    int           map_inode_blks;
    int           map_inode_offset;

    int           max_data;
    uint8_t*      map_data;
    int           map_data_blks;
    int           map_data_offset;

    int           data_offset;
    int           inode_offset;

    boolean       is_mounted;

    struct newfs_dentry* root_dentry;
};

struct newfs_inode {
    int ino;
    /* TODO: Define yourself */
    int                size;                            /* 文件已占用空间 */
    char               target_path[NEWFS_MAX_FILE_NAME];/* store traget path when it is a symlink */
    int                dir_cnt;
    struct newfs_dentry* dentry;                        /* 指向该inode的dentry */
    struct newfs_dentry* dentrys;                       /* 兄弟 */
    NEWFS_FILE_TYPE      ftype;   
    uint8_t*           data;
    int                blkno[NEWFS_DATA_PER_FILE];
    int                blknum;                          /* 使用到的data的块号 */
};

struct newfs_dentry {
    char     name[MAX_NAME_LEN];
    /* TODO: Define yourself */
    NEWFS_FILE_TYPE      ftype;
    struct newfs_dentry* parent;                        /* 父亲Inode的dentry */
    struct newfs_dentry* brother;                       /* 兄弟 */
    int                  ino;
    struct newfs_inode*  inode;                         /* 指向inode */
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

struct newfs_super_d
{
    uint32_t           magic_num;
    uint32_t           sz_usage;
    
    uint32_t           max_ino;
    uint32_t           map_inode_blks;
    uint32_t           map_inode_offset;

    uint32_t           max_data;
    uint32_t           map_data_blks;
    uint32_t           map_data_offset;

    uint32_t           data_offset;
    uint32_t           inode_offset;
};

struct newfs_inode_d
{
    uint32_t           ino;                           /* 在inode位图中的下标 */
    uint32_t           size;                          /* 文件已占用空间 */
    char               target_path[NEWFS_MAX_FILE_NAME];/* store traget path when it is a symlink */
    uint32_t           dir_cnt;
    NEWFS_FILE_TYPE    ftype;   
    uint32_t           blkno[NEWFS_DATA_PER_FILE];
    uint32_t           blknum;
};  

struct newfs_dentry_d
{
    char               fname[NEWFS_MAX_FILE_NAME];
    NEWFS_FILE_TYPE    ftype;
    uint32_t           ino;                           /* 指向的ino号 */
};  

#endif /* _TYPES_H_ */