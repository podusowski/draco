#ifndef __INLINES__H
#define __INLINES__H

inline static unsigned char inb(int port)
{
   register unsigned char r;
   
   __asm__ __volatile__( "inb %%dx,%%al\n\t" : "=a" (r) : "d" (port));
   return(r);
}

inline static void outb(int port, unsigned char data)
{
   __asm__ __volatile__("outb %%al,%%dx\n\t" :: "a" (data), "d" (port));
}

inline static unsigned short inw(int port)
{
   register unsigned short r;
   
   __asm__ __volatile__("inw %%dx,%%ax\n\t" : "=a" (r) : "d" (port));
   return(r);
}

inline static void outw(int port, unsigned short data)
{
   __asm__ __volatile__("outw %%ax,%%dx\n\t" :: "a" (data), "d" (port));
}

inline static void repinsw(int port, void* va, int count)
{
   __asm__ __volatile__ ("rep\n\t" \
               "insw" :: "d" (port), "D" (va), "c" (count));
}  

inline static void repoutsw(int port, void* va, int count)
{
   __asm__ __volatile__ ("rep\n\t" \
               "outsw" :: "d" (port), "D" (va), "c" (count));
}

inline static void sti()
{
   __asm__ __volatile__ ("sti"::);
}

inline static void cli()
{
   __asm__ __volatile__ ("cli"::);
}

inline static void fast_copy(void* src, void* dest, int count)
{
   __asm__ __volatile__ ("rep\n\t" \
               "movsl"   \
               :: "S" (src), "D" (dest), "c" (count));
}

inline static void ldcr3(unsigned long val)
{
   __asm__ __volatile__ ("movl %0, %%cr3\n\t" : : "r" (val));
}

inline static unsigned long get_cr3(void)
{
   register unsigned long res;
   
   __asm__ __volatile__( "movl %%cr3, %0\n\t" : "=r" (res) :); 
   return(res);
}

inline static void ldcr0(unsigned long val)
{
   __asm__ __volatile__ ("movl %0, %%cr0\n\t" : : "r" (val));
}

inline static unsigned long get_cr0(void)
{
   register unsigned long res;
   
   __asm__ __volatile__( "movl %%cr0, %0\n\t" : "=r" (res) :); 
   return(res);
}

inline static unsigned long get_esp()
{
	unsigned long __ret;
	__asm__ __volatile__("movl %%esp, %0":"=r"(__ret));
	return __ret;
}

inline static void ldesp(unsigned long val)
{
   __asm__ __volatile__ ("movl %0, %%esp\n\t" : : "r" (val));
}

static inline unsigned short get_ss()
{
	unsigned short __ret;
	__asm__ __volatile__("mov %%ss,%w0":"=r"(__ret));
	return __ret;
}

static inline unsigned short get_gs()
{
	unsigned short __ret;
	__asm__ __volatile__("mov %%gs,%w0":"=r"(__ret));
	return __ret;
}

static inline unsigned short get_fs()
{
	unsigned short __ret;
	__asm__ __volatile__("mov %%fs,%w0":"=r"(__ret));
	return __ret;
}

static inline unsigned short get_es()
{
	unsigned short __ret;
	__asm__ __volatile__("mov %%es,%w0":"=r"(__ret));
	return __ret;
}


static inline unsigned short get_cs()
{
	unsigned short __ret;
	__asm__ __volatile__("mov %%cs,%w0":"=r"(__ret));
	return __ret;
}

static inline unsigned short get_ds()
{
	unsigned short __ret;
	__asm__ __volatile__("mov %%ds,%w0":"=r"(__ret));
	return __ret;
}

inline static unsigned long get_eax()
{
	unsigned long __ret;
	__asm__ __volatile__("movl %%eax,%0":"=r"(__ret));
	return __ret;
}

inline static void ldeax(unsigned long val)
{
   __asm__ __volatile__ ("movl %0, %%eax\n\t" : : "r" (val));
}

inline static unsigned long get_ebx()
{
	unsigned long __ret;
	__asm__ __volatile__("movl %%ebx,%0":"=r"(__ret));
	return __ret;
}

inline static unsigned long get_ecx()
{
	unsigned long __ret;
	__asm__ __volatile__("movl %%ecx,%0":"=r"(__ret));
	return __ret;
}

inline static unsigned long get_edx()
{
	unsigned long __ret;
	__asm__ __volatile__("movl %%edx,%0":"=r"(__ret));
	return __ret;
}

inline static unsigned long get_ebp()
{
	unsigned long __ret;
	__asm__ __volatile__("movl %%ebx,%0":"=r"(__ret));
	return __ret;
}

inline static unsigned long get_esi()
{
	unsigned long __ret;
	__asm__ __volatile__("movl %%esi,%0":"=r"(__ret));
	return __ret;
}

inline static unsigned long get_edi()
{	
	unsigned long __ret;
	__asm__ __volatile__("movl %%edi,%0":"=r"(__ret));
	return __ret;
}




#endif

