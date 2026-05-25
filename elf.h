#ifndef ELF_H
#define ELF_H

#include "types.h"

#define	ELFMAG "\177ELF"
#define EI_NIDENT 16

typedef struct {
    u8  e_ident[EI_NIDENT];    /* Magic number and other info       */
    u16 e_type;                /* Object file type                  */
    u16 e_machine;             /* Architecture                      */
    u32 e_version;             /* Object file version               */
    u64 e_entry;               /* Entry point virtual address       */
    u64 e_phoff;               /* Program header table file offset  */
    u64 e_shoff;               /* Section header table file offset  */
    u32 e_flags;               /* Processor-specific flags          */
    u16 e_ehsize;              /* ELF header size in bytes          */
    u16 e_phentsize;           /* Program header table entry size   */
    u16 e_phnum;               /* Program header table entry count  */
    u16 e_shentsize;           /* Section header table entry size   */
    u16 e_shnum;               /* Section header table entry count  */
    u16 e_shstrndx;            /* Section header string table index */
} ELFHeader;

typedef struct {
    u32 sh_name;               /* Section name (string tbl index)   */
    u32 sh_type;               /* Section type                      */
    u64 sh_flags;              /* Section flags                     */
    u64 sh_addr;               /* Section virtual addr at execution */
    u64 sh_offset;             /* Section file offset               */
    u64 sh_size;               /* Section size in bytes             */
    u32 sh_link;               /* Link to another section           */
    u32 sh_info;               /* Additional section information    */
    u64 sh_addralign;          /* Section alignment                 */
    u64 sh_entsize;            /* Entry size if section holds table */
} ELFSectionHeader;

typedef struct {
    u32 st_name;               /* Symbol name (string tbl index)    */
    u8  st_info;               /* Symbol type and binding           */
    u8  st_other;              /* Symbol visibility                 */
    u16 st_shndx;              /* Section index                     */
    u64 st_value;              /* Symbol value                      */
    u64 st_size;               /* Symbol size                       */
} ELFSymbol;

typedef struct {
    u32 p_type;                /* Segment type                      */
    u32 p_flags;               /* Segment flags                     */
    u64 p_offset;              /* Segment file offset               */
    u64 p_vaddr;               /* Segment virtual address           */
    u64 p_paddr;               /* Segment physical address          */
    u64 p_filesz;              /* Segment size in file              */
    u64 p_memsz;               /* Segment size in memory            */
    u64 p_align;               /* Segment alignment                 */
} ELFProgramHeader;

#endif /* ELF_H */
