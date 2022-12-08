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
  /* Initialize the lock for the file system. */
  lock_init(&lock_filesys);

  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

/* Handles a system call initiated by a user program. */
static void
syscall_handler (struct intr_frame *f UNUSED)
{
    /* First ensure that the system call argument is a valid address. If not, exit immediately. */
    check_valid_addr((const void *) f->esp);

    /* Holds the stack arguments that directly follow the system call. */
    int args[3];

    /* Stores the physical page pointer. */
    void * phys_page_ptr;

	/* Get the value of the system call (based on enum) and call corresponding syscall function. */
	switch(*(int *) f->esp)
	{
		case SYS_HALT:
		/* Call the halt() function, which requires no arguments */
			halt();
			break;

		case SYS_EXIT:
			/* Exit has exactly one stack argument, representing the exit status. */
			get_stack_arguments(f, &args[0], 1);
			/* We pass exit the status code of the process. */
			exit(args[0]);
			break;

		case SYS_WRITE:
			/* Get three arguments off of the stack. The first represents the fd, the second
				represents the buffer, and the third represents the buffer length. */
			get_stack_arguments(f, &args[0], 3);

			/* Transform the virtual address for the buffer into a physical address. */
			args[1] = (int) pagedir_get_page(thread_current()->pagedir, (const void *) args[1]);

			/* Return the result of the write() function in the eax register. */
			f->eax = write(args[0], (const void *) args[1], (unsigned) args[2]);
			break;

		case SYS_OPEN:
			/* The first argument is the name of the file to be opened. */
			get_stack_arguments(f, &args[0], 1);

			/* Ensures that converted address is valid. */
			phys_page_ptr = pagedir_get_page(thread_current()->pagedir, (const void *) args[0]);
			if (phys_page_ptr == NULL)
			{
				exit(-1);
			}
			args[0] = (int) phys_page_ptr;

			/* Return the result of the remove() function in the eax register. */
			f->eax = open((const char *) args[0]);
			break;

		default:
			/* If an invalid system call was sent, terminate the program. */
			exit(-1);
			break;
	}
}

/* Terminates Pintos, shutting it down entirely (bummer). */
void halt (void)
{
	shutdown_power_off();
}

/* Terminates the current user program. It's exit status is printed,
   and its status returned to the kernel. */
void exit (int status)
{
	//thread_current()->exit_status = status;
	printf("%s: exit(%d)\n", thread_current()->name, status);
  	// sema_up(&thread_current()->being_waited_on);
  	thread_exit ();
}

/* Writes LENGTH bytes from BUFFER to the open file FD. Returns the number of bytes actually written,
 which may be less than LENGTH if some bytes could not be written. */
int write (int fd, const void *buffer, unsigned length)
{
  /* If fd is equal to one, then we write to STDOUT (the console, usually). */
	if(fd == 1)
	{
		putbuf(buffer, length);
    return length;
	}
	else
	{
		/* do something else with other fd */
	}
}


/* Check to make sure that the given pointer is not in kernel space,
   is not null, and is not an valid page. We must exit the program
   and free its resources should any of these conditions be violated. */
void check_valid_addr (const void *ptr_to_check)
{
  if(is_kernel_vaddr(ptr_to_check) || ptr_to_check == NULL || !pagedir_get_page(thread_current()->pagedir, ptr_to_check))
	{
    /* Terminate the program and free its resources */
    exit(-1);
	}
}

/* Code inspired by GitHub Repo created by ryantimwilson (full link in Design2.txt).
   Get up to three arguments from a programs stack (they directly follow the system
   call argument). */
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

/* Opens a file with the given name, and returns the file descriptor assigned by the
   thread that opened it. Inspiration derived from GitHub user ryantimwilson (see
   Design2.txt for attribution link). */
int open (const char *file)
{
  /* Make sure that only one process can get ahold of the file system at one time. */
  lock_acquire(&lock_filesys);

  struct file* f = filesys_open(file);

  /* If no file was created, then return -1. */
  if(f == NULL)
  {
    lock_release(&lock_filesys);
    return -1;
  }

  /* Create a struct to hold the file/fd, for use in a list in the current process.
     Increment the fd for future files. Release our lock and return the fd as an int. */
  struct thread_file *new_file = malloc(sizeof(struct thread_file));
  new_file->file_addr = f;
  int fd = thread_current ()->cur_fd;
  thread_current ()->cur_fd++;
  new_file->file_descriptor = fd;
  list_push_front(&thread_current ()->file_descriptors, &new_file->file_elem);
  lock_release(&lock_filesys);
  return fd;
}