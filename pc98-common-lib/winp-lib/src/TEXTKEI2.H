#define printf cprintf
#define	SLINE  	0x95
#define	DLINE  	0xe0
#define	DLR  	DLINE 
#define	DUDR  	DLINE +1 
#define	DCROSS 	DLINE +2 
#define	DUDL  	DLINE +3 
#define	SOFTL  	0x9c
#define	HARDL  	0x98
#define FSTUD  	0xE4    
#define ENDUD  	0xE5    
#define FSTLR  	0xE6    
#define ENDLR  	0xE7    
#define DFSTLR 	0xE8    
#define DENDLR 	0xE9    
#define UD    	0x96    
#define LR    	0x95    
#define LUKAGI  0x98    
#define RUKAGI	LUKAGI +1
#define LDKAGI  LUKAGI +2
#define RDKAGI  LUKAGI +3
#define LUMARU  0x9c    
#define RUMARU	LUMARU +1
#define LDMARU  LUMARU +2
#define RDMARU  LUMARU +3
#define CROSS   0x8f      
#define LRU     0x90      
#define LRD     0x91      
#define UDL     0x92      
#define UDR     0x93      
#define UE      1
#define NAKA    2         
#define SHITA   3         
#define YES     1         
#define NIJUYES 10         
#define NO      0         
#define	CLS    	printf("\033[2J\033[1;1H")
#define	LINE20  printf("\033[3h")
#define	LINE25  printf("\033[3l")
#define	CURON   printf("\033[>5l")
#define	CUROFF  printf("\033[>5h")
