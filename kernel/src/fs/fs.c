#include "common.h"
#include <sys/ioctl.h>
#include <string.h>

typedef struct {
	char *name;
	uint32_t size;
	uint32_t disk_offset;
} file_info;

enum {SEEK_SET, SEEK_CUR, SEEK_END};

/* This is the information about all files in disk. */
 static const file_info file_table[] __attribute__((used)) = {
 	{"1.rpg", 188864, 1048576}, {"2.rpg", 188864, 1237440},
 	{"3.rpg", 188864, 1426304}, {"4.rpg", 188864, 1615168},
 	{"5.rpg", 188864, 1804032}, {"abc.mkf", 1022564, 1992896},
 	{"ball.mkf", 134704, 3015460}, {"data.mkf", 66418, 3150164},
 	{"desc.dat", 16027, 3216582}, {"fbp.mkf", 1128064, 3232609},
 	{"fire.mkf", 834728, 4360673}, {"f.mkf", 186966, 5195401},
 	{"gop.mkf", 11530322, 5382367}, {"map.mkf", 1496578, 16912689},
 	{"mgo.mkf", 1577442, 18409267}, {"m.msg", 188232, 19986709},
 	{"mus.mkf", 331284, 20174941}, {"pat.mkf", 8488, 20506225},
 	{"rgm.mkf", 453202, 20514713}, {"rng.mkf", 4546074, 20967915},
 	{"sss.mkf", 557004, 25513989}, {"voc.mkf", 1997044, 26070993},
 	{"wor16.asc", 5374, 28068037}, {"wor16.fon", 82306, 28073411},
 	{"word.dat", 5650, 28155717},
 };



typedef struct {
	int opened;
	uint32_t offset;
} Fstate;

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

Fstate file_state[3+NR_FILES];

int fs_ioctl(int fd, uint32_t request, void *p) {
	assert(request == TCGETS);
	return (fd >= 0 && fd <= 2 ? 0 : -1);
}

void ide_read(uint8_t *, uint32_t, uint32_t);
void ide_write(uint8_t *, uint32_t, uint32_t);

/* TODO: implement a simplified file system here. */

int fs_open(const char *pathname, int flags)    /* 在我们的实现中可以忽略flags */
{
	// printk("fs_open %s\n",pathname);
	int index=3;
	for (; index < 3+NR_FILES; index++)
        if (strcmp(pathname, file_table[index-3].name) == 0)
            break;
	assert(index!=3+NR_FILES);
	file_state[index].opened=1;
	file_state[index].offset=0;
	return index;
}
int fs_read(int fd, void *buf, int len)
{
	// printk("fs_read fd=%d len=%d\n",fd,len);

	assert(fd>=3);
	assert(file_state[fd].opened);


	len=((file_state[fd].offset+len)<=file_table[fd-3].size)?len:file_table[fd-3].size- file_state[fd].offset;
	ide_read(buf,file_table[fd-3].disk_offset+file_state[fd].offset,len);

	file_state[fd].offset+=len;
	return len;

}
int fs_write(int fd, void *buf, int len)
{
	// printk("fs_write fd=%d len=%d\n",fd,len);
	if(fd==0||fd==1||fd==2)
	{
		assert(0);
		return -1;
	}
	else{
		assert(file_state[fd].opened);


		len=((file_state[fd].offset+len)<=file_table[fd-3].size)?len:file_table[fd-3].size- file_state[fd].offset;
		ide_write(buf,file_table[fd-3].disk_offset+file_state[fd].offset,len);

		file_state[fd].offset+=len;
		return len;
	}
}
int fs_lseek(int fd, int offset, int whence)
{
	/**
	 * SEEK_SET
       		  The offset is set to offset bytes.

       SEEK_CUR
              The offset is set to its current location plus offset bytes.

       SEEK_END
              The offset is set to the size of the file plus offset bytes.

	 */
	// printk("fs_lseek fd=%d offset=%d whence=%d\n",fd,offset,whence);

	assert(fd>=3);
	assert(file_state[fd].opened);
	 switch (whence) 
	 {
		case SEEK_SET:
			file_state[fd].offset =offset;break;
		case SEEK_CUR:
			file_state[fd].offset =file_state[fd].offset+offset;break;
		case SEEK_END:
			file_state[fd].offset =file_table[fd-3].size+offset;break;
		default:
			assert(0);
	}
	assert(file_state[fd].offset<=file_table[fd-3].size);
	return file_state[fd].offset;
}
int fs_close(int fd)
{
	// printk("fs_close fd=%d\n",fd);

	assert(fd>=3);
	assert(file_state[fd].opened);
	file_state[fd].opened=0;	
	return 0;
}