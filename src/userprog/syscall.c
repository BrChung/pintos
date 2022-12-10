#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"


static void syscall_handler (struct intr_frame *);

static struct thread *matching_thread;
static tid_t current_tid;

/* Creates a struct to insert files and their respective file descriptor into
   the file_descriptors list for the current thread. */
struct thread_file
{
    struct list_elem file_elem;
    struct file *file_addr;
    int file_descriptor;
};

/* Lock is in charge of ensuring that only one process can access the file system at one time. */
struct lock lock_filesys;

void
syscall_init (void)
{
  lock_init(&lock_filesys);

  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

/* Handles a system call initiated by a user program. */
static void
syscall_handler (struct intr_frame *f UNUSED)
{
    check_valid_addr((const void *) f->esp);

    int args[3];
    void * phys_page_ptr;

	/* Get the value of the system call (based on enum) and call corresponding syscall function. */
	switch(*(int *) f->esp)
	{
		case SYS_HALT:
			halt();
			break;

		case SYS_EXIT:
			get_stack_arguments(f, &args[0], 1);
			exit(args[0]);
			break;

		case SYS_WRITE:
			get_stack_arguments(f, &args[0], 3);

			args[1] = (int) pagedir_get_page(thread_current()->pagedir, (const void *) args[1]);

			f->eax = write(args[0], (const void *) args[1], (unsigned) args[2]);
			break;

		case SYS_OPEN:
			get_stack_arguments(f, &args[0], 1);

			phys_page_ptr = pagedir_get_page(thread_current()->pagedir, (const void *) args[0]);
			if (phys_page_ptr == NULL)
			{
				exit(-1);
			}
			args[0] = (int) phys_page_ptr;

			f->eax = open((const char *) args[0]);
			break;

		default:
			/* If an invalid system call was sent, terminate the program. */
			exit(-1);
			break;
	}
}

void halt (void)
{
	shutdown_power_off();
}

void exit (int status)
{
	printf("%s: exit(%d)\n", thread_current()->name, status);
  	thread_exit ();
}

int write (int fd, const void *buffer, unsigned length)
{
	if(fd == 1)
	{
		putbuf(buffer, length);
    	return length;
	}
}


void check_valid_addr (const void *ptr_to_check)
{
  if(is_kernel_vaddr(ptr_to_check) || ptr_to_check == NULL || !pagedir_get_page(thread_current()->pagedir, ptr_to_check))
	{
    	exit(-1);
	}
}

void get_stack_arguments (struct intr_frame *f, int *args, int num_of_args)
{
  int i;
  int *ptr;
  for (i = 0; i < num_of_args; i++)
    {
      ptr = (int *) f->esp + i + 1;
      check_valid_addr((const void *) ptr);
      args[i] = *ptr;
    }
}

int open (const char *file)
{
  lock_acquire(&lock_filesys);

  struct file* f = filesys_open(file);

  if(f == NULL)
  {
    lock_release(&lock_filesys);
    return -1;
  }

  // create file struct
  struct thread_file *new_file = malloc(sizeof(struct thread_file));
  new_file->file_addr = f;
  int fd = thread_current ()->cur_fd;
  thread_current ()->cur_fd++;
  new_file->file_descriptor = fd;
  list_push_front(&thread_current ()->file_descriptors, &new_file->file_elem);
  lock_release(&lock_filesys);
  return fd;
}