#include "common.h"
#include "fs.h"
#include "memory.h"

#define DEFAULT_ENTRY ((void *)0x8048000)

extern uint8_t ramdisk_start;
extern uint8_t ramdisk_end;
#define RAMDISK_SIZE ((&ramdisk_end) - (&ramdisk_start))
extern void ramdisk_read(void *buf, off_t* offset, size_t len);

uintptr_t loader(_Protect *as, const char *filename) {
  int fd = fs_open(filename, 0, 0);
  Log("filename=%s, fd=%d", filename, fd);

  //分页机制实现后增加的部分
  int size=fs_filesz(fd);
  int ppnum=size/PGSIZE;
  if (size%PGSIZE!=0)
  {
    ppnum++;
  }
  void *pa=NULL;
  void *va=DEFAULT_ENTRY;
  for (int i = 0; i < ppnum; i++)
  {
    pa=new_page();
    _map(as,va,pa);
    Log("va=0x%x,pa=0x%x",va,pa);
    fs_read(fd,pa,PGSIZE);
    va+=PGSIZE;
  }
  Log("loader加载完成");
  //fs_read(fd, DEFAULT_ENTRY, fs_filesz(fd));
  fs_close(fd);
  return (uintptr_t)DEFAULT_ENTRY;
}
